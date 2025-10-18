#ifndef MYDATASTRUCTURES_H
#define MYDATASTRUCTURES_H

#include "SymbolicAlgebra.h"

//Data structures
//So far all these data structures must be allocated dynamically!!!

//linked list of floats
struct llf_Node{
    struct llf_Node* next;
    float* val; //Why did i make this a fuckign pointer
};

void llf_Output(struct llf_Node* start);
void llf_Push(struct llf_Node* start, float* new_val);
void llf_Destroy(struct llf_Node* start); 

//linked list of polynomials
struct llp_Node{
    struct llp_Node* next;
    struct Polynomial* val;
};

void llp_Push(struct llp_Node* start, struct Polynomial* new_val);
void llp_Destroy(struct llp_Node* start); 


//Queue of ints
struct qi_Node{
    struct qi_Node* Next;
    int Val;
    int ValEmpty; //The only thing that needs initialising
};

struct qi_DequeueResult{
    int Val;
    int Code; // 0 if queue is empty
    struct qi_Node* NewRoot;
};

void qi_Enqueue(struct qi_Node* qi_Root, int val);
struct qi_DequeueResult qi_Dequeue(struct qi_Node* root);
void qi_Destroy(struct qi_Node* root);

#endif