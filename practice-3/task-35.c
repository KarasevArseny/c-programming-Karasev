#include <stdio.h>
int main(void) {
int numbers[10];
printf("Enter 10 numbers: ");
for (int i = 0; i < 10; i++) {
scanf("%d", &numbers[i]);
}
int min = numbers[0];
int max = numbers[0];
for (int i = 1; i < 10; i++) {
if (numbers[i] < min) {
min = numbers[i];
}
if (numbers[i] > max) {
max = numbers[i];
}
}
printf("Minimum: %d\n", min);
printf("Maximum: %d\n", max);
return 0;
}