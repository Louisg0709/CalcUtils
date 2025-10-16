#ifndef SYMBOLIC_ALGEBRA_H
#define SYMBOLIC_ALGEBRA_H

//Utility structs
struct ToStringResult{
    int ResultCode; //0 - fail, 1 - success
    int Pointer;
};
//for passing in variables and their indicies to the create algebraic value function
struct VarData{
    char Var;
    float Val;
};

struct AlgebraicValue{
    float Coefficient;
    float Indicies[52];
};
//AlgebraicValue functions
struct AlgebraicValue* MultiplyAlgebraicValues(struct AlgebraicValue* val1, struct AlgebraicValue* val2);
void MultiplyAlgebraicValuesToPtr(struct AlgebraicValue* val1, struct AlgebraicValue* val2, struct AlgebraicValue* result);

struct ToStringResult AlgebraicValueToString(struct AlgebraicValue* value, char* string, size_t length); //Returns 0 if function fails

struct Polynomial{
    struct AlgebraicValue* Values;
    int NumValues;
    int Simplified;
};

//Polynomial functions
struct Polynomial* CreateAlgebraicValue(float coefficient, struct VarData* var_data, int num_vars);
void DestroyPolynomial(struct Polynomial* val);

int SimplifyPolynomial(struct Polynomial* polynomial);
struct Polynomial* AddPolynomials(struct Polynomial* poly1, struct Polynomial* poly2);
struct Polynomial* MultiplyPolynomials(struct Polynomial* poly1, struct Polynomial* poly2);

struct Polynomial* InterpretValue(char* buffer, int length);
struct Polynomial* InterpretExpandedPolynomial(char* expression);
struct Polynomial* InterpretBrackets(char* expression);

struct ToStringResult PolynomialToString(struct Polynomial* poly, char* buffer, size_t length);

//Linked list
//float
struct llf_Node{
    struct llf_Node* next;
    float* val;
};

void llf_Output(struct llf_Node* start);
void llf_Push(struct llf_Node* start, float* new_val);
void llf_Destroy(struct llf_Node* start); 

//polynomial
struct llp_Node{
    struct llp_Node* next;
    struct Polynomial* val;
};

void llp_Push(struct llp_Node* start, struct Polynomial* new_val);
void llp_Destroy(struct llp_Node* start); 


//Utilities
int CharToIndex(char val);
char IndexToChar(int i);

int CompareFloatArrays(float* arr, float* arr2, int len);
void InitialiseFloatArrayToZero(float* arr, int len);

float ExtractFloatFromString(char* buffer, int end_index);



#endif