#include <stdio.h>
void findMinMax(int *arr, int size, int *min, int *max) {
if (size <= 0) {
printf("Ошибка: массив пустой!\n");
*min = *max = 0;
return;
}
*min = arr[0];
*max = arr[0];
for (int i = 1; i < size; i++) {
if (arr[i] < *min) {
*min = arr[i];
}
if (arr[i] > *max) {
*max = arr[i];
}
}
}
int main(void) {
printf("=== Функция findMinMax() для одновременного поиска минимума и максимума ===\n\n");
int arr[] = {5, 2, 9, 1, 7};
int size = sizeof(arr) / sizeof(arr[0]);
int min, max;    
printf("Массив: ");
for (int i = 0; i < size; i++) {
printf("%d ", arr[i]);
}
printf("\n");    
findMinMax(arr, size, &min, &max);    
printf("Минимум: %d\n", min);
printf("Максимум: %d\n", max);
printf("Размах: %d\n\n", max - min);    
return 0;
}