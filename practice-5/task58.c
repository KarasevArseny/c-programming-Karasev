#include <stdio.h>
typedef int (*IntFunc)(int);
int apply_composition(int x, IntFunc f, IntFunc g) {
return f(g(x));
}
int apply_chain(int x, IntFunc *funcs, size_t count) {
int result = x;
for (size_t i = 0; i < count; i++) {
result = funcs[i](result);
}
return result;
}
int add1(int x) { return x + 1; }
int mul2(int x) { return x * 2; }
int square(int x) { return x * x; }
int negate(int x) { return -x; }
int main(void) {
int x = 5;    
printf("x = %d\n\n", x);
int comp = apply_composition(x, add1, mul2);
printf("(add1 ∘ mul2)(%d) = %d\n", x, comp);
IntFunc chain[] = {add1, mul2, square, negate};
size_t count = sizeof(chain) / sizeof(chain[0]);    
int result = apply_chain(x, chain, count);
printf("Цепочка: add1 → mul2 → square → negate = %d\n", result);    
return 0;
}