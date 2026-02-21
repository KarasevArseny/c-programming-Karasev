#include <stdio.h>
typedef int (*Predicate)(int);
int linear_search(const int *arr, size_t size, int target) {
for (size_t i = 0; i < size; i++) 
if (arr[i] == target) return i;
return -1;
}
int binary_search(const int *arr, size_t size, int target) {
int left = 0, right = size - 1;
while (left <= right) {
int mid = left + (right - left) / 2;
if (arr[mid] == target) return mid;
if (arr[mid] < target) left = mid + 1;
else right = mid - 1;
}
return -1;
}
size_t count_if(const int *arr, size_t size, Predicate pred) {
size_t count = 0;
for (size_t i = 0; i < size; i++) 
if (pred(arr[i])) count++;
return count;
}
int all_of(const int *arr, size_t size, Predicate pred) {
for (size_t i = 0; i < size; i++) 
if (!pred(arr[i])) return 0;
return 1;
}
int any_of(const int *arr, size_t size, Predicate pred) {
for (size_t i = 0; i < size; i++) 
if (pred(arr[i])) return 1;
return 0;
}
int is_even(int x) { return x % 2 == 0; }
int is_negative(int x) { return x < 0; }
int is_greater_than_10(int x) { return x > 10; }
int main(void) {
int arr[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
size_t n = sizeof(arr) / sizeof(arr[0]);    
printf("Массив: ");
for (size_t i = 0; i < n; i++) printf("%d ", arr[i]);
printf("\n\n");    
printf("linear_search(10): найден на позиции %d\n", linear_search(arr, n, 10));
printf("binary_search(14): найден на позиции %d\n", binary_search(arr, n, 14));
printf("count_if(is_greater_than_10): %zu\n", count_if(arr, n, is_greater_than_10));
printf("all_of(is_even): %s\n", all_of(arr, n, is_even) ? "true" : "false");
printf("any_of(is_negative): %s\n", any_of(arr, n, is_negative) ? "true" : "false");
return 0;
}