#include <stdio.h>
#include <stdlib.h>

#include "AlgebraicValue.h"

struct AlgebraicValue* CreateAlgebraicValue(float coefficient, char* vars, float* indicies, int num_vars){
    struct AlgebraicValue* new_val = malloc(sizeof(struct AlgebraicValue));
    //should probably add check that malloc succeeded here
    new_val->Coefficient = coefficient;
    new_val->VarNames = vars;
    new_val->indicies = indicies;
    new_val->NumVars = num_vars;
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
    //Make new array without duplicates and sum the indicies
    printf("stage2");
    char* unique_vars = malloc(sizeof(char)*total);
    float* unique_indicies = malloc(sizeof(float)*total);
    int unique_count = 0; 
    for(int i = 0; i <total; i++){
        int found = 0;
        for (int x = 0; x<unique_count; x++){
            if (unique_vars[x] == all_vars[i]) {found = 1;}
        }
        if (!found){
            unique_vars[unique_count] = all_vars[i];
            float indicie_sum = all_indicies[i];
            for (int x = i+1; x < total; x++){
                if (all_vars[i] == all_vars[x]){indicie_sum+=all_indicies[x];}
            }
            unique_indicies[unique_count] = indicie_sum;
            unique_count += 1;
        }
    }
    //remove unused space
    free(all_vars);
    free(all_indicies);
    char* final_vars = malloc(sizeof(char) * unique_count);
    float* final_indicies = malloc(sizeof(float) * unique_count);
    for (int i = 0; i < unique_count; i++){
        final_vars[i] = unique_vars[i];
        final_indicies[i] = unique_indicies[i];
    }
    free(unique_vars);
    free(unique_indicies);
    return CreateAlgebraicValue(value1->Coefficient * value1->Coefficient, final_vars, final_indicies, unique_count);
}

void OutputAlgebraicValue(struct AlgebraicValue* value){
    printf("%.2f", value->Coefficient);
    for (int i = 0; i < value->NumVars; i++){
        printf("*%c^%.2f",value->VarNames[i], value->indicies[i]);
    }
}

struct Expression* SimplifyExpression(struct Expression* expression){

}


int main(){
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
}