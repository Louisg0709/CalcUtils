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
        if (value->VarNames[i] >= 'a' && value->VarNames[i] <= 'z') {
            if (value->indicies[i] == 1.0f){
                printf("*%c", value->VarNames[i]);
            } else {
                printf("*%c^%.2f", value->VarNames[i], value->indicies[i]);
            }
        } else {
            printf("*[INVALID_VAR]^%.2f", value->indicies[i]);
        }
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
            unique_count++;
        }
        DestroyAlgebraicValue(expression->Values[i]);
    }
    struct AlgebraicValue** new_values = malloc(sizeof(struct AlgebraicValue*) * unique_count);
    for (int i = 0; i <unique_count; i++){
        new_values[i] = CopyValue(&unique_values[i]);
    }
    free(unique_values);
    expression->Values = new_values;
    expression->Length = unique_count;
    expression->Simplified = 1;
}

void SimplifyAlgebraicValue(struct AlgebraicValue* value) {
    if (value == NULL) return;
    if (value->NumVars <= 0) {
        /* ensure arrays are empty but valid */
        free(value->VarNames);
        free(value->indicies);
        value->VarNames = NULL;
        value->indicies = NULL;
        value->NumVars = 0;
        value->Simplified = 1;
        return;
    }

    /* Temporary buffers sized to the original number of variables. */
    char* unique_vars = malloc(sizeof(char) * value->NumVars);
    float* unique_indicies = malloc(sizeof(float) * value->NumVars);
    int unique_count = 0;

    /* Build unique_vars and summed exponents. */
    for (int i = 0; i < value->NumVars; ++i) {
        char current_var = value->VarNames[i];

        /* Check whether current_var is already in unique_vars. */
        int already_added = 0;
        for (int j = 0; j < unique_count; ++j) {
            if (unique_vars[j] == current_var) {
                already_added = 1;
                break;
            }
        }
        if (already_added) continue;

        /* Sum all occurrences of current_var across the original arrays. */
        float sum = 0.0f;
        for (int j = 0; j < value->NumVars; ++j) {
            if (value->VarNames[j] == current_var) {
                sum += value->indicies[j];
            }
        }

        unique_vars[unique_count] = current_var;
        unique_indicies[unique_count] = sum;
        unique_count++;
    }

    /* Free the old arrays that value owned. */
    free(value->VarNames);
    free(value->indicies);

    /* Allocate arrays sized to the unique count (could be zero). */
    char* final_vars = NULL;
    float* final_indicies = NULL;
    if (unique_count > 0) {
        final_vars = malloc(sizeof(char) * unique_count);
        final_indicies = malloc(sizeof(float) * unique_count);
        for (int i = 0; i < unique_count; ++i) {
            final_vars[i] = unique_vars[i];
            final_indicies[i] = unique_indicies[i];
        }
    }

    free(unique_vars);
    free(unique_indicies);

    /* Sort final arrays (alphabetical) while keeping exponents aligned. */
    if (unique_count > 1) {
        int swap_made = 1;
        while (swap_made) {
            swap_made = 0;
            for (int i = 0; i < unique_count - 1; ++i) {
                if (final_vars[i] > final_vars[i + 1]) {
                    char ctmp = final_vars[i];
                    final_vars[i] = final_vars[i + 1];
                    final_vars[i + 1] = ctmp;
                    float ftmp = final_indicies[i];
                    final_indicies[i] = final_indicies[i + 1];
                    final_indicies[i + 1] = ftmp;
                    swap_made = 1;
                }
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
    for (int i = 0; i < expression->Length; i++){DestroyAlgebraicValue(expression->Values[i]);}
    free(expression->Values);
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
    for (int i = 0; i < e1->Length; i++) { result->Values[i] = CopyValue(e1->Values[i]); }
    for (int i = 0; i < e2->Length; i++) {result->Values[e1->Length + i] = CopyValue(e2->Values[i]);}
    SimplifyExpression(result);
    return(result);
}

struct AlgebraicValue* CopyValue(struct AlgebraicValue* value){
    if (!value->Simplified) {SimplifyAlgebraicValue(value);}
    char* copied_vars = malloc(sizeof(char) * value->NumVars);
    float* copied_indicies = malloc(sizeof(float) * value->NumVars);
    for (int i = 0; i < value->NumVars; i++) {
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
