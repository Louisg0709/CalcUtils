#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "SymbolicAlgebra.h"
#include "MyDataStructures.h"

int main(){
    char buffer[200];
    memset(buffer, 0, sizeof buffer);
    scanf("%s", buffer); //Apparently deprecated :-(
    //Really ought to find out the alternative to scan f.
    struct Polynomial* test = InterpretValue(buffer, 200);
    memset(buffer, 0, sizeof buffer);
    if((PolynomialToString(test, buffer, 200)).ResultCode){
        printf("%s\n", buffer);
    }
    DestroyPolynomial(test);
}