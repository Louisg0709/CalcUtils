#include <stdio.h>
#include <stdlib.h>
#include "MyDataStructures.h"

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
}
