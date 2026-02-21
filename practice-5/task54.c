#include <stdio.h>
typedef int (*Transform)(int);
typedef int (*Predicate)(int);
typedef int (*Reducer)(int, int);
void array_map(int *arr, size_t size, Transform func) {
for (size_t i = 0; i < size; i++) arr[i] = func(arr[i]);
}
size_t array_filter(const int *src, size_t size, int *dst, Predicate pred) {
size_t count = 0;
for (size_t i = 0; i < size; i++) 
if (pred(src[i])) dst[count++] = src[i];
return count;
}
int array_reduce(const int *arr, size_t size, int initial, Reducer func) {
int result = initial;
for (size_t i = 0; i < size; i++) result = func(result, arr[i]);
return result;
}
int square(int x) { return x * x; }
int negate(int x) { return -x; }
int double_val(int x) { return x * 2; }
int abs_val(int x) { return x < 0 ? -x : x; }
int is_positive(int x) { return x > 0; }
int is_even(int x) { return x % 2 == 0; }
int sum(int a, int b) { return a + b; }
int product(int a, int b) { return a * b; }
int max(int a, int b) { return a > b ? a : b; }
int main(void) {
int arr[] = {1, -2, 3, -4, 5, -6, 7, -8, 9, -10};
size_t n = sizeof(arr) / sizeof(arr[0]);
int temp[100];    
printf("Исходный: ");
for (size_t i = 0; i < n; i++) printf("%d ", arr[i]);
int mapped[n];
for (size_t i = 0; i < n; i++) mapped[i] = arr[i];
array_map(mapped, n, square);
printf("\nПосле map(square): ");
for (size_t i = 0; i < n; i++) printf("%d ", mapped[i]);
size_t pos_count = array_filter(arr, n, temp, is_positive);
printf("\nПосле filter(is_positive): ");
for (size_t i = 0; i < pos_count; i++) printf("%d ", temp[i]);
int s = array_reduce(arr, n, 0, sum);
printf("\nReduce(sum, 0): %d\n", s);
size_t pos = array_filter(arr, n, temp, is_positive);
array_map(temp, pos, square);
int r = array_reduce(temp, pos, 0, sum);
printf("Комбинация (positive → square → sum): %d\n", r);    
return 0;
}