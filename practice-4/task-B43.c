#include <stdio.h>
#include <math.h>  
double power(double base, int exp) {
double result = 1.0;
if (exp < 0) {
base = 1.0 / base;
exp = -exp;
}
for (int i = 0; i < exp; i++) {
result *= base;
}    
return result;
}
int main(void) {
printf("=== Возведение числа в целую степень ===\n\n");
printf("Тестовые вычисления:\n");
printf("2^10 = %.0lf\n", power(2, 10));        
printf("2^(-3) = %.3lf\n", power(2, -3));      
printf("5^0 = %.0lf\n", power(5, 0));          
printf("3^5 = %.0lf\n", power(3, 5));          
printf("10^(-2) = %.3lf\n", power(10, -2));    
printf("1.5^3 = %.3lf\n", power(1.5, 3));      
printf("4^(-0.5) = %.3lf\n", power(4, -0.5));      
return 0;
}