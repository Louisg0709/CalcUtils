#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct llf_Node{
    struct llf_Node* next;
    float* val;
};

void llf_Output(struct llf_Node* start);
void llf_Push(struct llf_Node* start, float* new_val);
void llf_Destroy(struct llf_Node* start); 

#endif