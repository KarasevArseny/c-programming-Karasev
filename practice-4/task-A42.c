#include <stdio.h>
int main(void) {
int a = 10;
int *ptr = &a;
printf("До: a = %d\n", a);
*ptr = 25;
printf("После: a = %d\n", a);
return 0;
}