#include "stdlib.h"
#include "stdio.h"

#include "LinkedList.h"

int main(){
    struct llf_Node* root = malloc(sizeof(root));
    root->next = NULL;
    root->val = NULL;  

    for (int i = 0; i<50; i++){
        float* new_float = malloc(sizeof(float));
        *new_float = (float)i;
        llf_Push(root, new_float);  
    }
    llf_Output(root);

    llf_Destroy(root);
}

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
