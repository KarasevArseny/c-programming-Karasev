#include <stdio.h>
int main(void) {
int x = 42;
int *p = &x;
printf("sizeof(p) = %zu\n", sizeof(p));
printf("sizeof(*p) = %zu\n", sizeof(*p));
printf("sizeof(x) = %zu\n", sizeof(x));
return 0;
}