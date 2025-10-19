#ifndef MYDATASTRUCTURES_H
#define MYDATASTRUCTURES_H

#include "SymbolicAlgebra.h"

//Data structures
//So far all these data structures must be allocated dynamically!!!

//linked list of polynomials
struct llp_Node{
    struct llp_Node* next;
    struct Polynomial* val;
    int length;
};

struct llp_GetResult{
    struct Polynomial* Val;
    int Code;
    int Direct;
};

void llp_Push(struct llp_Node* start, struct Polynomial* new_val);
void llp_Destroy(struct llp_Node* start); 
struct llp_GetResult llp_Pop(struct llp_Node* start);
struct llp_GetResult llp_Get(struct llp_Node* start, int index);



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