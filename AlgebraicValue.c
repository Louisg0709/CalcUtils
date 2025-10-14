#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AlgebraicValue.h"

int main(){
    //Test multipling values
    //Create 2xy
    char* vars1 = malloc(sizeof(char) * 2);
    vars1[0] = 'x';
    vars1[1] = 'y';
    float* indicies1 = malloc(sizeof(float) * 2);
    indicies1[0] = 1;
    indicies1[1] = 1;
    struct AlgebraicValue* val1 = CreateAlgebraicValue(2,vars1,indicies1,2);
    OutputAlgebraicValue(val1);
    printf("\n");

    //Create 3x^2w^6
    char* vars2 = malloc(sizeof(char) * 2);
    vars2[0] = 'x';
    vars2[1] = 'w';
    float* indicies2 = malloc(sizeof(float) * 2);
    indicies2[0] = 2;
    indicies2[1] = 6;
    struct AlgebraicValue* val2 = CreateAlgebraicValue(3,vars2,indicies2,2);
    OutputAlgebraicValue(val2);
    printf("\n");

    struct AlgebraicValue* result = MultiplyAlgebraicValues(val1, val2);
    printf("Result: ");
    OutputAlgebraicValue(result);
    printf("\n");

    //Create expression
    struct Expression* eVal1 = CreateSingleValExpression(val1);
    OutputExpression(eVal1);
    printf("\n");
    struct Expression* eVal2 = CreateSingleValExpression(val2);
    OutputExpression(eVal2);
    printf("\n");
    struct Expression* eValS = CombineExpressions(eVal1, eVal2);
    OutputExpression(eValS);

    struct Expression* eValSSquare = MultiplyExpressions(eValS,eValS);
    printf("\n");
    OutputExpression(eValSSquare);

    DestroyExpression(eValSSquare);
    DestroyExpression(eValS);
    DestroyExpression(eVal2);
    DestroyExpression(eVal1);
    DestroyAlgebraicValue(result);
}

struct AlgebraicValue* CreateAlgebraicValue(float coefficient, char* vars, float* indicies, int num_vars){
    struct AlgebraicValue* new_val = malloc(sizeof(struct AlgebraicValue));
    //should probably add check that malloc succeeded here
    new_val->Coefficient = coefficient;
    new_val->VarNames = vars;
    new_val->indicies = indicies;
    new_val->NumVars = num_vars;
    new_val->Simplified = 0;

    SimplifyAlgebraicValue(new_val);
    return new_val;
}

int DestroyAlgebraicValue(struct AlgebraicValue* value){
    free(value->indicies);
    free(value->VarNames);
    free(value);
    return 1;
}

struct AlgebraicValue* MultiplyAlgebraicValues(struct AlgebraicValue* value1, struct AlgebraicValue* value2){
    //Copy everything into one array
    int total = value1->NumVars + value2->NumVars;
    char* all_vars = malloc(sizeof(char)*total);
    float* all_indicies = malloc(sizeof(float)*total);
    for (int i = 0; i < value1->NumVars; i++){
        all_vars[i] = value1->VarNames[i];
        all_indicies[i] = value1->indicies[i];
    }
    for (int i = 0; i < value2->NumVars; i++){
        all_vars[value1->NumVars + i] = value2->VarNames[i];
        all_indicies[value1->NumVars + i] = value2->indicies[i];
    }
    
    return CreateAlgebraicValue(value1->Coefficient*value2->Coefficient, all_vars, all_indicies, total);
}

void OutputAlgebraicValue(struct AlgebraicValue* value){
    printf("%.2f", value->Coefficient);
    for (int i = 0; i < value->NumVars; i++){
        printf("*%c^%.2f",value->VarNames[i], value->indicies[i]);
    }
}

void SimplifyExpression(struct Expression* expression){
    struct AlgebraicValue* unique_values = malloc(sizeof(struct AlgebraicValue) * expression->Length);
    unique_values[0] = *expression->Values[0];
    int unique_count = 1;
    for (int i = 1; i <expression->Length; i++){
        int duplicate = 0;
        for (int x = 0; x < unique_count; x++){
            if (ValuesCanBeAdded(&unique_values[x], expression->Values[i])){
                unique_values[x].Coefficient += expression->Values[i]->Coefficient;
            }
        }
        if(!duplicate){
            unique_values[unique_count] = *expression->Values[i];
            unique_count++;
        }
        DestroyAlgebraicValue(expression->Values[i]);
    }
    struct AlgebraicValue** new_values = malloc(sizeof(struct AlgebraicValue*) * unique_count);
    for (int i = 0; i <unique_count; i++){
        new_values[i] = malloc(sizeof(struct AlgebraicValue));
        new_values[i] = CopyValue(&unique_values[i]);
    }
    free(unique_values);
    expression->Values = new_values;
    expression->Length = unique_count;
    expression->Simplified = 1;
}

void SimplifyAlgebraicValue(struct AlgebraicValue* value){

    //remove duplicates (coppied this from the multiply function)
    //Make new array without duplicates and sum the indicies
    char* unique_vars = malloc(sizeof(char)*value->NumVars);
    float* unique_indicies = malloc(sizeof(float)*value->NumVars);
    int unique_count = 0; 
    for(int i = 0; i <value->NumVars; i++){
        int found = 0;
        for (int x = 0; x<unique_count; x++){
            if (unique_vars[x] == value->VarNames[i]) {found = 1;}
        }
        if (!found){
            unique_vars[unique_count] = value->VarNames[i];
            float indicie_sum = value->indicies[i];
            for (int x = i+1; x < value->NumVars; x++){
                if (value->VarNames[i] == value->VarNames[x]){indicie_sum+=value->indicies[x];}
            }
            unique_indicies[unique_count] = indicie_sum;
            unique_count += 1;
        }
    }
    //remove unused space
    free(value->VarNames);
    free(value->indicies);
    char* final_vars = malloc(sizeof(char) * unique_count);
    float* final_indicies = malloc(sizeof(float) * unique_count);
    for (int i = 0; i < unique_count; i++){
        final_vars[i] = unique_vars[i];
        final_indicies[i] = unique_indicies[i];
    }
    free(unique_vars);
    free(unique_indicies);

    //Now sort arrays using bubble sort to enable easy comparisons
    int swap_made = 1;
    while(swap_made){
        swap_made = 0;
        for (int i = 0; i < unique_count - 1; i++){
            if (final_vars[i] > final_vars[i+1]){
                char char_temp = final_vars[i];
                final_vars[i] = final_vars[i+1];
                final_vars[i+1] = char_temp;

                float float_temp = final_indicies[i];
                final_indicies[i] = final_indicies[i+1];
                final_indicies[i+1] = float_temp;

                swap_made = 1;
            }
        }
    }
    value->VarNames = final_vars;
    value->indicies = final_indicies;
    value->NumVars = unique_count;
    value->Simplified = 1;
}

int ValuesCanBeAdded(struct AlgebraicValue* value1, struct AlgebraicValue* value2){
    if (!value1->Simplified){SimplifyAlgebraicValue(value1);}
    if (!value2->Simplified){SimplifyAlgebraicValue(value2);}
    if (value1->NumVars != value2->NumVars){return 0;}
    int same = 1;
    for (int i = 0; i < value1->NumVars; i++){
        if(value1->VarNames[i] != value2->VarNames[i]){same = 0;}
        if(value1->indicies[i] != value2->indicies[i]){same = 0;}
    }
    return same;
}   

struct Expression* MultiplyExpressions(struct Expression* expression1, struct Expression* expression2){
    int total_size = expression1->Length*expression2->Length;
    struct Expression* result = malloc(sizeof(struct Expression));
    result->Length = total_size;
    result->Values = malloc(sizeof(struct AlgebraicValue*) * total_size);
    int count = 0;
    for (int x = 0; x < expression1->Length; x++){for (int y = 0; y < expression2->Length; y++){
        result->Values[count] = MultiplyAlgebraicValues(expression1->Values[x],expression2->Values[y]);
        count+=1;
    }}
    SimplifyExpression(result);
    return result;
}

void DestroyExpression(struct Expression* expression){
    for (int i = 0; i < expression->Length; i++){free(expression->Values[i]);}
    free(expression);
}

void OutputExpression(struct Expression* expression)
{
    OutputAlgebraicValue(expression->Values[0]);
    for (int i = 1; i < expression->Length; i++){
        printf("+");
        OutputAlgebraicValue(expression->Values[i]);
    }
}

struct Expression* CombineExpressions(struct Expression* e1, struct Expression* e2){
    struct Expression* result = malloc(sizeof(struct Expression));
    result->Length = e1->Length + e2->Length;
    result->Values = malloc(sizeof(struct AlgebraicValue*)*result->Length);
    for (int i = 0; i < e1->Length; i++){
        result->Values[i] = malloc(sizeof(struct AlgebraicValue));
        result->Values[i] = CopyValue(e1->Values[i]);
    }
    for (int i = 0; i < e2->Length; i++){
        result->Values[e1->Length+i] = malloc(sizeof(struct AlgebraicValue));
        result->Values[e1->Length+i] = CopyValue(e2->Values[i]);
    }
    SimplifyExpression(result);
    return(result);
}

struct AlgebraicValue* CopyValue(struct AlgebraicValue* value){
    char* copied_vars = malloc(sizeof(char)*value->NumVars);
    float* copied_indicies = malloc(sizeof(float)*value->NumVars);
    for (int i = 0; i < value->NumVars; i++){
        copied_vars[i] = value->VarNames[i];
        copied_indicies[i] = value->indicies[i];
    }
    return CreateAlgebraicValue(value->Coefficient, copied_vars, copied_indicies, value->NumVars);
}

struct Expression* CreateSingleValExpression(struct AlgebraicValue* val){
    struct Expression* new = malloc(sizeof(struct Expression));
    new->Length = 1;
    new->Simplified = 1;
    new->Values = malloc(sizeof(struct AlgebraicValue*));
    new->Values[0] = val;
    return new;
}

//In progress
/*
struct Expression* EvaluateString(char* text){
    struct Expression* result = malloc(sizeof(struct Expression));
    result->Length = 0;

    int len = strlen(text);
    int pointy = 0;

    struct Expression* new = malloc(sizeof(struct Expression));
    new->Length = 0;
    while(pointy<len){
        
    }
}
*/
