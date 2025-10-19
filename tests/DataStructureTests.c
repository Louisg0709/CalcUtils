#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MyDataStructures.h"
#include "SymbolicAlgebra.h"

int main(){
    printf("Testing queue...\n");
    struct qi_Node* qi_test = malloc(sizeof(struct qi_Node));
    qi_test->Next = NULL;
    qi_test->ValEmpty = 1;
    for (int i = 0; i < 50; i++){
        printf("Enqueueing %d\n", i);
        qi_Enqueue(qi_test, i);
    }

    printf("Dequeueing items\n");
    int dequeing = 1;
    while(dequeing){
        struct qi_DequeueResult result;
        result = qi_Dequeue(qi_test);
        if (result.Code){
            printf("%d dequeued\n", result.Val);
            qi_test = result.NewRoot;
        }else{
            dequeing = 0;
        }
    }
    qi_Destroy(qi_test);

    printf("\nTesting linked list...\n");
    struct llp_Node* llp_test = malloc(sizeof(struct llp_Node));
    llp_test->length = 0;
    llp_test->next = NULL;
    llp_test->val = NULL;

    for (int i = 0; i < 15; i++){
        printf("Adding polynomial %d to the list\n", i);
        llp_Push(llp_test,CreateAlgebraicValue((float)i,NULL, 0));
    }

    char buffer[200];
    memset(buffer, 0, sizeof buffer);

    struct llp_GetResult get_res;
    get_res = llp_Get(llp_test, 4);
    if(PolynomialToString(get_res.Val, buffer, 200).ResultCode){
        printf("Index 4: %s\n", buffer);
    }
    memset(buffer, 0, sizeof buffer);

    get_res = llp_Get(llp_test, 9);
    if(PolynomialToString(get_res.Val, buffer, 200).ResultCode){
        printf("Index 9: %s\n", buffer);
    }
    memset(buffer, 0, sizeof buffer);

    get_res = llp_Get(llp_test, 14);
    if(PolynomialToString(get_res.Val, buffer, 200).ResultCode){
        printf("Index 14: %s\n", buffer);
    }
    memset(buffer, 0, sizeof buffer);

    printf("Popping items\n");
    for (int i = 0; i < 15; i++){
        get_res = llp_Pop(llp_test);
        if(PolynomialToString(get_res.Val, buffer, 200).ResultCode){
            printf("Popping polynomial %s from list\n(index %d)\n", buffer, 14-i);
        }
        memset(buffer, 0, sizeof buffer);
    }
    llp_Destroy(llp_test);
}
