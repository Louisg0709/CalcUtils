#include <stdlib.h>
#include <stdio.h>

#include "MyDataStructures.h"

/*
Linked Lists
*/

//polynomial
void llp_Push(struct llp_Node* start, struct Polynomial* new_val){
    if(!start->val){
        start->val = new_val;
        start->length = 1;
        return;
    }
    if(start->next){
        start->length++;
        llp_Push(start->next, new_val);
        return;
    }
    start->next = malloc(sizeof(struct llp_Node));
    start->next->next = NULL;
    start->next->val = new_val;
    start->next->length = 1;
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

struct llp_GetResult llp_Pop(struct llp_Node* start){
    struct llp_GetResult result;
    if(!start->next){
        if (start->val){
            result.Code = 1;
            result.Val = start->val;
            result.Direct = 1;
        }else{
            result.Code = 0;
            result.Val = NULL;
            result.Direct = 1;
        }
        return result;
    }
    result = llp_Pop(start->next);
    if (result.Direct){
        free(start->next);
        start->next = NULL;
        result.Direct = 0;
    }
    return result;
}

struct llp_GetResult llp_Get(struct llp_Node* start, int index){
    struct llp_GetResult result;
    result.Code = 0;
    if (!start) return result;
    if(index == 0){
        if(start->val){
            result.Code = 1;
            result.Val = start->val;
        }
        return result;
    }
    if(!start->next) return result;
    return llp_Get(start->next, index-1);
}



//Int queue
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
    result.Val = root->Val;
    return result;
}

void qi_Destroy(struct qi_Node* root){
    if (root->Next){
        qi_Destroy(root->Next);
    }
    free(root);
}
