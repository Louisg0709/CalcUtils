#ifndef ALGEBRAICVALUE_H
#define ALGEBRAICVALUE_H

struct AlgebraicValue{
    float Coefficient;
    char* VarNames;
    float* indicies;
    int NumVars;
};

struct AlgebraicValue* CreateAlgebraicValue(float coefficient, char* vars, float* indicies, int num_vars);
int DestroyAlgebraicValue(struct AlgebraicValue* value);
void OutputAlgebraicValue(struct AlgebraicValue* value);

struct AlgebraicValue* MultiplyAlgebraicValues(struct AlgebraicValue* value1, struct AlgebraicValue* value2);
int ValuesCanBeAdded(struct AlgebraicValue* value1, struct AlgebraicValue* value2);

struct Expression{
    int Length;
    struct AlgebraicValue** Values;
};

//struct Expression* CombineExpressions(struct Expression* expressions, int num); 
struct Expression* SimplifyExpression(struct Expression* expression);
//struct Expression* MultiplyExpressions(struct Expression* expressions, int num)

#endif