#include <stdio.h>
int main(void) {
int numbers[5];
int sum = 0;
float average;
printf("Enter 5 numbers: ");
for (int i = 0; i < 5; i++) {
scanf("%d", &numbers[i]);
sum += numbers[i];
}
average = (float)sum / 5;
printf("\nYou entered: ");
for (int i = 0; i < 5; i++) {
printf("%d", numbers[i]);
if (i < 4) {
printf(" ");
}
}
printf("\nAverage: %.2f\n", average);    
return 0;
}