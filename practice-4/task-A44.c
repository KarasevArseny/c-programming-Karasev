#include <stdio.h>
int main(void) {
int a = 5, b = 10;
int *pa = &a, *pb = &b;
printf("До: a = %d, b = %d\n", a, b);
int temp = *pa;
*pa = *pb;
*pb = temp;
printf("После: a = %d, b = %d\n", a, b);
return 0;
}