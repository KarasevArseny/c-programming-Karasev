#include <stdio.h>
void swap(int *a, int *b) {
int temp = *a;
*a = *b;
*b = temp;
}
void sortArrayBubble(int *arr, int size) {
for (int i = 0; i < size - 1; i++) {
for (int j = 0; j < size - i - 1; j++) {
if (arr[j] > arr[j + 1]) {
swap(&arr[j], &arr[j + 1]);
}
}
}
}
int main(void) {
printf("=== Сортировка массива по возрастанию (пузырьковая сортировка) ===\n\n");   
int arr[] = {64, 34, 25, 12, 22};
int size = sizeof(arr) / sizeof(arr[0]);    
printf("Исходный массив: ");
for (int i = 0; i < size; i++) {
printf("%d ", arr[i]);
}
printf("\n");    
sortArrayBubble(arr, size);   
printf("Отсортированный массив: ");
for (int i = 0; i < size; i++) {
printf("%d ", arr[i]);
}
printf("\n");    
return 0;
}