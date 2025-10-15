#include <stdlib.h>
#include <stdio.h>

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
    if(AlgebraicValueToString(&TestVal, buffer, sizeof(char)*200)){
        printf("%s", buffer);
    }
    printf("\n%s", buffer);
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

char IndexToChar(int i){
    if (i>=0 && i< 26){return 'a'+i;}
    else if (i>=26 && i < 52){return 'A'+i-26;}
    else{return '?';}
}