#include <stdio.h>
int findMax(int *arr, int size) {
if (size <= 0) {
printf("Ошибка: массив пустой!\n");
return -1;
}
int max = arr[0];
for (int i = 1; i < size; i++) {
if (arr[i] > max) {
max = arr[i];  
}
}
return max;
}
int main(void) {
printf("=== Функция findMax() для поиска максимального элемента ===\n\n");
int arr1[] = {5, 2, 9, 1, 7};
int size1 = sizeof(arr1) / sizeof(arr1[0]);  
printf("Массив 1: ");
for (int i = 0; i < size1; i++) {
printf("%d ", arr1[i]);
}
printf("\n");
printf("Максимальный элемент: %d\n\n", findMax(arr1, size1));
return 0;
}