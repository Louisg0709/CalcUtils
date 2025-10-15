#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SymbolicAlgebra.h"

int main(){
    struct AlgebraicValue TestVal;
    TestVal.Coefficient = 0;
    //Setting indexes of array manually because c cant do it well for some reason
    for (int i = 0; i<52; i++){TestVal.Indicies[i] = 0.0;}
    TestVal.Indicies[0] = 2;
    TestVal.Indicies[1] = 1;
    TestVal.Indicies[2] = 3;
    char buffer[200]; 
    if(AlgebraicValueToString(&TestVal, buffer, sizeof(char)*200)){printf("%s\n", buffer);}
    memset(buffer, 0, sizeof buffer);
    struct AlgebraicValue* TestVal2 = MultiplyAlgebraicValues(&TestVal,&TestVal);
    if(AlgebraicValueToString(TestVal2, buffer, sizeof(char)*200)){printf("%s\n", buffer);}
    free(TestVal2);
}

struct AlgebraicValue* MultiplyAlgebraicValues(struct AlgebraicValue* val1, struct AlgebraicValue* val2){
    struct AlgebraicValue* result = malloc(sizeof(struct AlgebraicValue));
    for (int i = 0; i < 52; i++){
        result->Indicies[i] = val1->Indicies[i] + val2->Indicies[i];
    }
    result->Coefficient = val1->Coefficient * val2->Coefficient;
    return result;
}

int AlgebraicValueToString(struct AlgebraicValue* value, char* buffer , size_t length){
    int pointer = 0;
    //In future could make function to truncate 0s from floats
    int written = snprintf(buffer, length, "%.2f", value->Coefficient); 
    if (written<0 || written>= length - pointer){return 0;}
    pointer+=written;
    for (int i = 0; i < 52; i++){
        if (value->Indicies[i] != 0){
            char var = IndexToChar(i);
            written = snprintf(buffer+pointer, length-pointer, "%s", &var);
            if (written<0 || written>= length - pointer){return 0;}
            pointer+=written;
            if (value->Indicies[i] != 1){
                written = snprintf(buffer+pointer, length-pointer, "^%.2f", value->Indicies[i]);
                if (written<0 || written>= length - pointer){return 0;}
                pointer+=written;
            }
        }
    }
    return 1;
}

struct Polynomial* CreateAlgebraicValue(float coefficient, struct VarData* var_data, int num_vars){
    struct Polynomial* result = malloc(sizeof(struct Polynomial));

    result->NumValues = 1;
    result->Values = malloc(sizeof(struct AlgebraicValue));
    result->Values[0].Coefficient = coefficient;

    for (int i = 0; i<num_vars; i++){
        int index = CharToIndex(var_data[i].Var);
        if (index!=-1){ //Variable is ignored if char is invalid
            result->Values[0].Indicies[CharToIndex(var_data[i].Var)] = var_data->Val;
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

    int num_unique = 1;
    for(int i = 1; i < polynomial->NumValues; i++){
        int unique = 1;
        for (int x = 0; x < i; x++){
            if(CompareFloatArrays(polynomial->Values[i].Indicies, polynomial->Values[x].Indicies, 52)){
                unique = 0;
            }
        }
        if (unique){num_unique++;}
    }

    struct AlgebraicValue* new_values = malloc(sizeof(struct AlgebraicValue) * num_unique);
    new_values[0].Coefficient = polynomial->Values[0].Coefficient;
    for (int i = 0; i  < 52; i++){
        new_values[0].Indicies[i] = polynomial->Values[0].Indicies[i];
    }
    int count = 1;
    for (int x = 1; x <polynomial->NumValues; x++){
        int unique = 1;
        for (int y = 0; y < count; y++){
            if (CompareFloatArrays(polynomial->Values[x].Indicies, new_values[y].Indicies,52)){
                unique = 0;
            }
        if(unique){
            new_values[count].Coefficient = polynomial->Values[x].Coefficient;
            for (int i = 0; i < 52; i++){
                new_values[count].Indicies[i] = polynomial->Values[x].Indicies[i];
            }
            count++;
        }
        } 
    }
    polynomial->NumValues = num_unique;
    free(polynomial->Values);
    polynomial->Values = new_values;
    polynomial->Simplified = 1;
    return 1;

}

int CharToIndex(char val){
    int int_val = (int)val;
    if (int_val>=97 && int_val < 123){return int_val-97;}
    if (int_val>=65 && int_val < 91){return - 65 + 26;}
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