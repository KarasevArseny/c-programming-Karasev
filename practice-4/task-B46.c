#include <stdio.h>
int findMaxIndex(int *arr, int size) {
if (size <= 0) {
printf("Ошибка: массив пустой!\n");
return -1;
}
int max_index = 0;
for (int i = 1; i < size; i++) {
if (arr[i] > arr[max_index]) {
max_index = i;  
}
}
return max_index;
}
int main(void) {
printf("=== Функция findMaxIndex() для поиска индекса максимального элемента ===\n\n");
int arr[] = {5, 2, 9, 1, 7};
int size = sizeof(arr) / sizeof(arr[0]);
printf("Массив: ");
for (int i = 0; i < size; i++) {
printf("%d ", arr[i]);
}
printf("\n");
int max_index = findMaxIndex(arr, size);
printf("Индекс максимального элемента: %d\n", max_index);
printf("Значение максимального элемента: arr[%d] = %d\n\n", max_index, arr[max_index]);
return 0;
}