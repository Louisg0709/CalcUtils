#ifndef ALGEBRAICVALUE_H
#define ALGEBRAICVALUE_H

struct AlgebraicValue{
    float Coefficient;
    char* VarNames;
    float* indicies;
    int NumVars;
    int Simplified;
};

struct AlgebraicValue* CreateAlgebraicValue(float coefficient, char* vars, float* indicies, int num_vars);
int DestroyAlgebraicValue(struct AlgebraicValue* value);
void OutputAlgebraicValue(struct AlgebraicValue* value);
void SimplifyAlgebraicValue(struct AlgebraicValue* value);
struct AlgebraicValue* CopyValue(struct AlgebraicValue* value);
struct AlgebraicValue* MultiplyAlgebraicValues(struct AlgebraicValue* value1, struct AlgebraicValue* value2);
int ValuesCanBeAdded(struct AlgebraicValue* value1, struct AlgebraicValue* value2);

struct Expression{
    int Length;
    struct AlgebraicValue** Values;
    int Simplified;
};

struct Expression* CreateSingleValExpression(struct AlgebraicValue* val);
void DestroyExpression(struct Expression* expression);
void OutputExpression(struct Expression* expression);
void SimplifyExpression(struct Expression* expression);
struct Expression* MultiplyExpressions(struct Expression* expression, struct Expression* expression2);
struct Expression* CombineExpressions(struct Expression* e1, struct Expression* e2);
//struct Expression* EvaluateString(char* text);

#endif