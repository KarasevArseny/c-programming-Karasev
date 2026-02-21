#include <stdio.h>
#include <math.h>
double f(double x) {
return exp(x + 2); 
}
int main(void) {
double a = 0, b = 1; 
int n = 1000; 
double h = (b - a) / n;
double sum = (f(a) + f(b)) / 2.0;
for (int i = 1; i < n; i++) {
sum += f(a + i * h);
}
double integral = h * sum;
printf("Integral of e^(x+2) from %.1f to %.1f:\n", a, b);
printf("Numerical result: %.6f\n", integral);
double exact = exp(b + 2) - exp(a + 2);
printf("Exact result: %.6f\n", exact);
printf("Error: %.6f\n", fabs(integral - exact));
return 0;
}