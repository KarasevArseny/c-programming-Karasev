#include <stdio.h>
#include <math.h>
typedef struct {
double min;
double max;
double sum;
double average;
double variance;
} ArrayStats;
ArrayStats calculate_stats(const double *arr, size_t size) {
ArrayStats stats = {0};
if (size == 0 || arr == NULL) {
stats.min = stats.max = stats.sum = stats.average = stats.variance = 0.0;
return stats;
}
stats.min = arr[0];
stats.max = arr[0];
stats.sum = arr[0];
for (size_t i = 1; i < size; i++) {
if (arr[i] < stats.min) {
stats.min = arr[i];
}
if (arr[i] > stats.max) {
stats.max = arr[i];
}
stats.sum += arr[i];
}
stats.average = stats.sum / size;
double variance_sum = 0.0;
for (size_t i = 0; i < size; i++) {
double diff = arr[i] - stats.average;
variance_sum += diff * diff;
}
stats.variance = variance_sum / size;    
return stats;
}
int main(void) {
printf("=== Статистика массива чисел с плавающей точкой ===\n\n");
double arr[] = {2.5, 4.0, 1.5, 3.0, 5.0};
size_t size = sizeof(arr) / sizeof(arr[0]);   
printf("Массив: ");
for (size_t i = 0; i < size; i++) {
printf("%.2f ", arr[i]);
}
printf("\n\n");    
ArrayStats stats = calculate_stats(arr, size);    
printf("Результаты:\n");
printf("Min:      %.2f\n", stats.min);
printf("Max:      %.2f\n", stats.max);
printf("Sum:      %.2f\n", stats.sum);
printf("Average:  %.2f\n", stats.average);
printf("Variance: %.2f\n", stats.variance);
return 0;
}