#include <stdio.h>
int main(void) {
int n = 5;
int arr[5];
printf("Enter 5 numbers: ");
for (int i = 0; i < n; i++) {
scanf("%d", &arr[i]);
}
printf("Original: ");
for (int i = 0; i < n; i++) {
printf("%d ", arr[i]);
}
printf("\n");
for (int i = 0; i < n / 2; i++) {
int temp = arr[i];
arr[i] = arr[n - 1 - i];
arr[n - 1 - i] = temp;
}
printf("Reversed: ");
for (int i = 0; i < n; i++) {
printf("%d ", arr[i]);
}
printf("\n");
return 0;
}