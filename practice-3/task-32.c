#include <stdio.h>
int main(void) {
int n;
double sum = 0.0;
printf("Enter n: ");
scanf("%d", &n);
if (n <= 0) {
printf("Error: n must be a positive integer\n");
return 1;
}
for (int i = 1; i <= n; i++) {
sum += 1.0 / i;
}
printf("S = ");
for (int i = 1; i <= n; i++) {
printf("1/%d", i);
if (i < n) {
printf(" + ");
}
}
printf(" = %.4f\n", sum);
return 0;
}