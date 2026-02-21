#include <stdio.h>
long long factorial(int n) {
long long result = 1;
for (int i = 1; i <= n; i++) {
result *= i;
}
return result;
}
int main(void) {
int n;
printf("5! = %lld\n", factorial(5));    
printf("10! = %lld\n", factorial(10));  
printf("\nВведите число для вычисления факториала: ");
scanf("%d", &n);
if (n < 0) {
printf("Ошибка: Факториал определен только для неотрицательных чисел.\n");
} else {
printf("%d! = %lld\n", n, factorial(n));
} 
return 0;
}