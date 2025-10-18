#include <stdlib.h>
#include <stdio.h>

#include "MyDataStructures.h"

/*
Linked Lists
*/
//float

void llf_Output(struct llf_Node* start){
    if(start->val){
        printf("%f\n", *start->val);
    }
    if(start->next){
        llf_Output(start->next);
    }
}

void llf_Push(struct llf_Node* start, float* new_val){
    if(!start->val){
        start->val = new_val;
        return;
    }
    if(start->next){
        llf_Push(start->next, new_val);
        return;
    }
    start->next = malloc(sizeof(struct llf_Node));
    start->next->next = NULL;
    start->next->val = new_val;
}

void llf_Destroy(struct llf_Node* start){
    if(start->next){
        llf_Destroy(start->next);
    }
    if(start->val){
        free(start->val);
    }
    if(start){
        free(start);
    }
}

//polynomial
void llp_Push(struct llp_Node* start, struct Polynomial* new_val){
    if(!start->val){
        start->val = new_val;
        return;
    }
    if(start->next){
        llp_Push(start->next, new_val);
        return;
    }
    start->next = malloc(sizeof(struct llp_Node));
    start->next->next = NULL;
    start->next->val = new_val;
}

void llp_Destroy(struct llp_Node* start){
    if(start->next){
        llp_Destroy(start->next);
    }
    if(start->val){
        DestroyPolynomial(start->val);
    }
    if(start){
        free(start);
    }
}

void qi_Enqueue(struct qi_Node* root, int val){
    if (root->ValEmpty){
        root->Val = val;
        root->ValEmpty = 0;
        return;
    }
    if (root->Next){
        qi_Enqueue(root->Next, val);
        return;
    }
    root->Next = malloc(sizeof(struct qi_Node));
    root->Next->Val = val;
    root->Next->ValEmpty = 0;
    root->Next->Next = NULL;
}
struct qi_DequeueResult qi_Dequeue(struct qi_Node* root){
    struct qi_DequeueResult result;
    if(root->ValEmpty){
        result.Code = 0;
        result.Val = 0;
        result.NewRoot = root;
        return result;
    }
    result.Code = 1;
    result.Val = root->Val;
    if(root->Next){
        result.NewRoot = root->Next;
        free(root);
        return result;
    }
    root->ValEmpty = 1;
    result.NewRoot = root;
    return result;
}

void qi_Destroy(struct qi_Node* root){
    if (root->Next){
        qi_Destroy(root->Next);
    }
    free(root);
}
