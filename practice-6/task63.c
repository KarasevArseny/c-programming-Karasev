#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef enum {
EXPR_NUMBER,
EXPR_VARIABLE,
EXPR_ADD,
EXPR_SUB,
EXPR_MUL
} ExprType;
typedef struct Expr Expr;
struct Expr {
ExprType type;
union {
double number;
char variable;
struct { Expr *left; Expr *right; } binary;
} data;
};
Expr* number(double v) { Expr *e = malloc(sizeof(Expr)); e->type = EXPR_NUMBER; e->data.number = v; return e; }
Expr* variable(char v) { Expr *e = malloc(sizeof(Expr)); e->type = EXPR_VARIABLE; e->data.variable = v; return e; }
Expr* add(Expr *l, Expr *r) { Expr *e = malloc(sizeof(Expr)); e->type = EXPR_ADD; e->data.binary.left = l; e->data.binary.right = r; return e; }
Expr* sub(Expr *l, Expr *r) { Expr *e = malloc(sizeof(Expr)); e->type = EXPR_SUB; e->data.binary.left = l; e->data.binary.right = r; return e; }
Expr* mul(Expr *l, Expr *r) { Expr *e = malloc(sizeof(Expr)); e->type = EXPR_MUL; e->data.binary.left = l; e->data.binary.right = r; return e; }
double get_var(char c) { return (c == 'x') ? 5 : 0; }
double eval(Expr *e) {
if (!e) return 0;
switch(e->type) {
case EXPR_NUMBER: return e->data.number;
case EXPR_VARIABLE: return get_var(e->data.variable);
case EXPR_ADD: return eval(e->data.binary.left) + eval(e->data.binary.right);
case EXPR_SUB: return eval(e->data.binary.left) - eval(e->data.binary.right);
case EXPR_MUL: return eval(e->data.binary.left) * eval(e->data.binary.right);
default: return 0;
}
}
void print(Expr *e) {
if (!e) return;
switch(e->type) {
case EXPR_NUMBER: printf("%.0f", e->data.number); break;
case EXPR_VARIABLE: printf("%c", e->data.variable); break;
case EXPR_ADD: printf("("); print(e->data.binary.left); printf("+"); print(e->data.binary.right); printf(")"); break;
case EXPR_SUB: printf("("); print(e->data.binary.left); printf("-"); print(e->data.binary.right); printf(")"); break;
case EXPR_MUL: printf("("); print(e->data.binary.left); printf("*"); print(e->data.binary.right); printf(")"); break;
}
}
int main(void) {
Expr *x = variable('x');
Expr *expr = mul(add(x, number(3)), sub(x, number(2)));    
printf("Выражение: "); print(expr); printf("\n");
printf("При x = 5: %g\n", eval(expr));    
return 0;
}