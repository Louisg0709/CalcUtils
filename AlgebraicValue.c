#include <stdio.h>
#include <stdlib.h>

#include "AlgebraicValue.h"

int main(){
    //Test multipling values
    char* vars1 = malloc(sizeof(char) * 2);
    vars1[0] = 'x';
    vars1[1] = 'y';
    float* indicies1 = malloc(sizeof(float) * 2);
    indicies1[0] = 1;
    indicies1[1] = 1;
    struct AlgebraicValue* val1 = CreateAlgebraicValue(2,vars1,indicies1,2);
    OutputAlgebraicValue(val1);
    printf("\n");
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
    DestroyAlgebraicValue(val1);
    DestroyAlgebraicValue(val2);
    
    //test adding values
    

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
    
    struct AlgebraicValue* result = CreateAlgebraicValue(value1->Coefficient*value2->Coefficient, all_vars, all_indicies, total);
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
                duplicate = 1;
            }
        }
        if(!duplicate){
            unique_values[unique_count] = *expression->Values[i];
        }
        DestroyAlgebraicValue(expression->Values[i]);
    }
    struct AlgebraicValue** new_values = malloc(sizeof(struct AlgebraicValue*) * unique_count);
    for (int i = 0; i <unique_count; i++){
        new_values[i] = malloc(sizeof(struct AlgebraicValue));
        *new_values[i] = unique_values[i];
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
    value->VarNames = final_vars;
    value->indicies = final_indicies;
    value->NumVars = unique_count;

    //Now sort arrays using bubble sort to enable easy comparisons
    int swap_made = 1;
    while(swap_made){
        swap_made = 0;
        for (int i = 0; i < unique_count - 1; i++){
            if (unique_vars[i] > unique_vars[i+1]){
                char char_temp = unique_vars[i];
                unique_vars[i] = unique_vars[i+1];
                unique_vars[i+1] = char_temp;

                float float_temp = unique_indicies[i];
                unique_indicies[i] = unique_indicies[i+1];
                unique_indicies[i+1] = float_temp;

                int swap_made = 1;
            }
        }
    }
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

