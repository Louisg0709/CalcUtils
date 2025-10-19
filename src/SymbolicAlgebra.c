#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "SymbolicAlgebra.h"
#include "MyDataStructures.h"

/*
AlgebraicValueFunctions
*/

struct AlgebraicValue* MultiplyAlgebraicValues(struct AlgebraicValue* val1, struct AlgebraicValue* val2){
    struct AlgebraicValue* result = malloc(sizeof(struct AlgebraicValue));
    MultiplyAlgebraicValuesToPtr(val1, val2, result);
    return result;
}

void MultiplyAlgebraicValuesToPtr(struct AlgebraicValue* val1, struct AlgebraicValue* val2, struct AlgebraicValue* result){
    for (int i = 0; i < 52; i++){
        result->Indicies[i] = val1->Indicies[i] + val2->Indicies[i];
    }
    result->Coefficient = val1->Coefficient * val2->Coefficient;
}


struct ToStringResult AlgebraicValueToString(struct AlgebraicValue* value, char* buffer , size_t length){
    struct ToStringResult data;
    data.Pointer = 0;
    int written = 0;

    int indicie_found = 0;
    for (int i = 0; i < 52; i++){
        if(value->Indicies[i]!=0){indicie_found = 1;}
    }

    if (value->Coefficient == 0){
        written = snprintf(buffer, length, "0");
        if (written<0 || written >= length - data.Pointer){
            data.Pointer+=written;
            data.ResultCode = 0;
            return data;
        }
        data.Pointer+=written;
        data.ResultCode = 1;
        return data;
    }

    if (value->Coefficient!=1 || !indicie_found){
        //In future could make function to truncate 0s from floats
        written = snprintf(buffer, length, "%.2f", value->Coefficient); 
        if (written<0 || written>= length - data.Pointer){
            data.Pointer += written;
            data.ResultCode = 0;
            return data;
        }
    }
    data.Pointer+=written;
    for (int i = 0; i < 52; i++){
        if (value->Indicies[i] != 0){
            char var = IndexToChar(i);
            written = snprintf(buffer+data.Pointer, length-data.Pointer, "%c", var);
            if (written<0 || written>= length - data.Pointer){
                data.Pointer+=written;
                data.ResultCode = 0;
                return data;
            }
            data.Pointer+=written;
            if (value->Indicies[i] != 1){
                written = snprintf(buffer+data.Pointer, length-data.Pointer, "^%.2f", value->Indicies[i]);
                if (written<0 || written>= length - data.Pointer){
                    data.Pointer += written;
                    data.ResultCode = 0;
                    return data;
                }
                data.Pointer+=written;
            }
        }
    }
    data.ResultCode = 1;
    return data;
}

/*
Polynomial functions
*/

struct Polynomial* CreatePolynomial(struct AlgebraicValue* vals, int num){
    struct Polynomial* result = malloc(sizeof(struct Polynomial));
    
    result->Simplified = 0;
    result->NumValues = num;
    result->Values = malloc(sizeof(struct AlgebraicValue)*num);

    for (int i = 0; i < num; i++){
        result->Values[i] = vals[i];
    }

    SimplifyPolynomial(result);
    return result;
}


struct Polynomial* CreateAlgebraicValue(float coefficient, struct VarData* var_data, int num_vars){
    struct Polynomial* result = malloc(sizeof(struct Polynomial));

    result->NumValues = 1;
    result->Values = malloc(sizeof(struct AlgebraicValue));

    result->Values[0].Coefficient = coefficient;
    for (int i = 0; i<52; i++){result->Values[0].Indicies[i] = 0.0;}

    for (int i = 0; i<num_vars; i++){
        int index = CharToIndex(var_data[i].Var);
        if (index!=-1){ //Variable is ignored if char is invalid
            result->Values[0].Indicies[index] += var_data[i].Val;
        }
    }

    result->Simplified = 1;
    return result;
}

void DestroyPolynomial(struct Polynomial* val){
    free(val->Values);
    free(val);
}

int SimplifyPolynomial(struct Polynomial* polynomial){
    if (polynomial->Simplified) {return 0;}

    if (polynomial->NumValues == 1 || polynomial->NumValues == 0){
        polynomial->Simplified = 1;
        return 0;
    }

    struct AlgebraicValue* unique_values = malloc(sizeof(struct AlgebraicValue)* polynomial->NumValues);
    unique_values[0] = polynomial->Values[0];
    int num_unique = 1;
    for (int i = 1; i<polynomial->NumValues; i++){
        int unique = 1;
        for (int x = 0; x<num_unique; x++){
            if(CompareFloatArrays(unique_values[x].Indicies, polynomial->Values[i].Indicies, 52)){
                unique = 0;
                unique_values[x].Coefficient += polynomial->Values[i].Coefficient;
            }
        }
        if (unique){
            unique_values[num_unique] = polynomial->Values[i];
            num_unique++;
        }
    }
    free(polynomial->Values);
    polynomial->Values = unique_values;
    polynomial->NumValues = num_unique;
    polynomial->Simplified = 1;
    return 1;
}

struct Polynomial* AddPolynomials(struct Polynomial* poly1, struct Polynomial* poly2){
    struct Polynomial* result = malloc(sizeof(struct Polynomial));
    result->Simplified = 0;
    result->NumValues = poly1->NumValues + poly2->NumValues;
    result->Values = malloc(sizeof(struct AlgebraicValue)*result->NumValues);

    for(int i = 0; i < poly1->NumValues; i++){
        result->Values[i] = poly1->Values[i];
    }
    for(int i = 0; i < poly2->NumValues; i++){
        result->Values[i+poly1->NumValues] = poly2->Values[i];
    }

    SimplifyPolynomial(result);
    return result;
}

struct Polynomial* MultiplyPolynomials(struct Polynomial* poly1, struct Polynomial* poly2){
    struct Polynomial* result = malloc(sizeof(struct Polynomial));
    result->Simplified = 0;
    result->NumValues = poly1->NumValues * poly2->NumValues;
    result->Values = malloc(sizeof(struct AlgebraicValue)*result->NumValues);

    int count = 0;
    for (int x = 0; x < poly1->NumValues; x++){
        for (int y = 0; y < poly2->NumValues; y++){
            MultiplyAlgebraicValuesToPtr(&poly1->Values[x], &poly2->Values[y], &result->Values[count]);
            count++;
        }
    }

    SimplifyPolynomial(result);
    return result;
}

struct ToStringResult PolynomialToString(struct Polynomial* poly, char* buffer, size_t length){
    struct ToStringResult data;
    data.Pointer = 0;
    int written;

    if(poly->NumValues == 0){
        written = snprintf(buffer, length, "0");
        if (written<0 || written >= length - data.Pointer){
            data.Pointer+=written;
            data.ResultCode = 0;
            return data;
        }
        data.Pointer+=written;
        data.ResultCode = 1;
        return data;
    }

    struct ToStringResult sub = AlgebraicValueToString(&poly->Values[0], buffer + data.Pointer, length - data.Pointer);
    if (!sub.ResultCode){return sub;}
    data.Pointer += sub.Pointer;

    for (int i = 1; i < poly->NumValues; i++) {
        if(poly->Values[i].Coefficient > 0){
            written = snprintf(buffer + data.Pointer, length - data.Pointer, "%c", '+');
            if (written < 0 || written >= (int)(length - data.Pointer)) {
                data.ResultCode = 0;
                return data;
            }
            data.Pointer += written;
        }
        sub = AlgebraicValueToString(&poly->Values[i], buffer + data.Pointer, length - data.Pointer);
        if (!sub.ResultCode) return sub;
        data.Pointer += sub.Pointer;
    }


    data.ResultCode = 1;
    return data;
}

struct Polynomial* InterpretValue(char* buffer, int length){
    if (length <= 0) return NULL;

    float coefficient = 1;
    float vars[52];
    InitialiseFloatArrayToZero(vars, 52);

    int pointer = 0;

    //Get coefficient
    if(isdigit(buffer[0]) || buffer[0] == '-' || buffer[0] == '+'){
        int coefficient_len = 1;

        int point_not_found = 1;
        int getting_coefficient_len = 1;
        while (getting_coefficient_len && coefficient_len<length){
            if(isdigit(buffer[coefficient_len])){
                coefficient_len ++;
            }else if (buffer[coefficient_len] == '.' && point_not_found){
                point_not_found = 0;
                coefficient_len++;
            }else{
                getting_coefficient_len = 0;
            }
        }
        coefficient = ExtractFloatFromString(buffer, coefficient_len-1);
        pointer += coefficient_len;
    }
    
    //Current plan is:
    //1. Iterate through buffer and whenever a letter is find write the index to a queue of ints
    //2. For each letter check the indexes between itself and the next letter (or the end of the string if the queue is empty)
    //3. Maybe add the read float logic into a new function
    //4. Use letter to get index in vars and add float that gets read to the index.

    int first_var_index = pointer;

    struct qi_Node* var_indexes = malloc(sizeof(struct qi_Node));
    var_indexes->Next = NULL;
    var_indexes->ValEmpty=1;
    
    int num_vars = 0;
    while (pointer<length){
        if(isalpha(buffer[pointer])){
            qi_Enqueue(var_indexes, pointer);
            num_vars++;
        }
        pointer++;
    }

    struct VarData* var_arr = malloc(sizeof(struct VarData)*num_vars);
    for (int i = 0; i < num_vars; i++){
        struct qi_DequeueResult result = qi_Dequeue(var_indexes);
        var_arr[i].Var = buffer[result.Val];
        if (result.NewRoot != var_indexes){
            var_indexes = result.NewRoot;
            if(result.Code == 0) return NULL;
            if(i == 0){
                if(result.Val!=first_var_index) return NULL;
            }
            
            if (var_indexes != NULL){
                if (buffer[result.Val+1] == '^'){
                    var_arr[i].Val = ExtractFloatFromString(buffer+result.Val+2, var_indexes->Val-1);
                }else{
                    var_arr[i].Val = 1;
                }
            }
        }else{
            if(result.Val+2<length){
                if (buffer[result.Val+1] == '^'){
                    var_arr[i].Val = ExtractFloatFromString(buffer+result.Val + 2, length-1);
                }else{
                    var_arr[i].Val = 1;
                }
            }else{
                var_arr[i].Val = 1;
            }
        }
    }
    free(var_indexes);

    struct Polynomial* new_polynomial = CreateAlgebraicValue(coefficient, var_arr, num_vars);
    free(var_arr);
    return new_polynomial;
}

/*
Utilities
*/

int CharToIndex(char val){
    int int_val = (int)val;
    if (int_val>=97 && int_val < 123){return int_val-97;}
    if (int_val>=65 && int_val < 91){return int_val- 65 + 26;}
    return -1; //Invalid input
}

char IndexToChar(int i){
    if (i>=0 && i< 26){return 'a'+i;}
    if (i>=26 && i < 52){return 'A'+i-26;}
    return '?'; //Invalid index
}

int CompareFloatArrays(float* arr, float* arr2, int len){
    for (int i = 0; i < len; i++){
        if (arr[i] != arr2[i]){return 0;}
    }
    return 1;
}

void InitialiseFloatArrayToZero(float* arr, int len){
    for (int i = 0; i < len; i++){
        arr[i] = 0;
    }
}

float ExtractFloatFromString(char* buffer, int end_index){
    //Handle special cases first ('-' or '+' with no numbers)
    if (end_index == 0){
        if (buffer[0] == '+') return 1.0;
        if (buffer[0] == '-') return -1.0;
    }

    char* endptr;
    char saved = buffer[end_index+1];
    buffer[end_index+1] = '\0'; //Nullterminate string 
    float val = strtof(buffer, &endptr);
    buffer[end_index+1] = saved; //Restore buffers original state
    return val;
}
