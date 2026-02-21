#include <stdio.h>
long long factorial_rec(int n) {
if (n == 0) {
return 1;
}
else {
return n * factorial_rec(n - 1);
}
}
int main(void) {
printf("5! = %lld\n", factorial_rec(5));    
printf("10! = %lld\n", factorial_rec(10));  
int n;
printf("\nВведите число для вычисления факториала: ");
scanf("%d", &n);    
if (n < 0) {
printf("Ошибка: Факториал определен только для неотрицательных чисел.\n");
} else {
printf("%d! = %lld\n", n, factorial_rec(n));
}    
return 0;
}