#include <stdio.h>
#include <math.h>
double f(double x) {
return sin(x);
}
int main(void) {
double a = 0, b = M_PI;    
int n = 1000;              
double h = (b - a) / n;    
double sum = (f(a) + f(b)) / 2.0;
for (int i = 1; i < n; i++) {
sum += f(a + i * h);
}
double integral = h * sum;
double exact = 2.0;
printf("Integral of sin(x) from 0 to π:\n");
printf("Numerical result: %.10f\n", integral);
printf("Exact result:     %.10f\n", exact);
printf("Error:            %.10f\n", fabs(integral - exact));
printf("\nAnalytical verification:\n");
printf("-cos(π) = %.6f\n", -cos(M_PI));
printf("-cos(0) = %.6f\n", -cos(0));
printf("Result: -cos(π) - (-cos(0)) = %.2f\n", -cos(M_PI) - (-cos(0)));
return 0;
}