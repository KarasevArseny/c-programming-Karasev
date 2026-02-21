#include <stdio.h>
#include <math.h>
int main(void) {
double pi = 3.14159;
double end = 2 * pi;
printf("Table of sin(x) from 0 to 2π (2*3.14159 ≈ %.5f)\n", end);
printf("================================================\n");
for (double x = 0; x <= end + 0.001; x += 0.1) {
double rounded_x = round(x * 100) / 100;
printf("x = %.2f, sin(x) = %.4f\n", rounded_x, sin(x));
}
return 0;
}