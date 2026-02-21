#include <stdio.h>
#include <math.h>
typedef int (*Generator)(int index);
void array_generate(int *arr, int size, Generator gen) {
for (int i = 0; i < size; i++) arr[i] = gen(i);
}
int fib(int n) {
if (n <= 1) return n;
int a = 0, b = 1, c;
for (int i = 2; i <= n; i++) { c = a + b; a = b; b = c; }
return b;
}
int fact(int n) {
int r = 1;
for (int i = 1; i <= n; i++) r *= i;
return r;
}
int tri(int n) { return n * (n + 1) / 2; }
int sq(int n) { return n * n; }
int prime(int n) {
int count = 0, num = 2;
while (1) {
int is_prime = 1;
for (int i = 2; i <= sqrt(num); i++) {
if (num % i == 0) { is_prime = 0; break; }
}
if (is_prime) {
if (count == n) return num;
count++;
}
num++;
}
}
int main(void) {
const int N = 10;
int arr[N];    
printf("=== Генератор последовательностей ===\n\n");    
array_generate(arr, N, fib);
printf("Фибоначчи:  "); for (int i = 0; i < N; i++) printf("%d ", arr[i]); printf("\n");    
array_generate(arr, N, fact);
printf("Факториал:  "); for (int i = 0; i < N; i++) printf("%d ", arr[i]); printf("\n");    
array_generate(arr, N, tri);
printf("Треугольные:"); for (int i = 0; i < N; i++) printf("%d ", arr[i]); printf("\n");    
array_generate(arr, N, sq);
printf("Квадраты:   "); for (int i = 0; i < N; i++) printf("%d ", arr[i]); printf("\n");    
array_generate(arr, N, prime);
printf("Простые:    "); for (int i = 0; i < N; i++) printf("%d ", arr[i]); printf("\n");    
return 0;
}