#include <stdio.h>
void swap(int *a, int *b) {
int temp = *a;
*a = *b;        
*b = temp;      
}
int main(void) {
printf("=== Функция swap() для обмена значений ===\n\n");
int x = 5, y = 10;
printf("Пример 1:\n");
printf("До swap: x = %d, y = %d\n", x, y);
swap(&x, &y);
printf("После swap: x = %d, y = %d\n\n", x, y);
int a = 100, b = 200;
printf("Пример 2:\n");
printf("До swap: a = %d, b = %d\n", a, b);
swap(&a, &b);
printf("После swap: a = %d, b = %d\n\n", a, b);
int m = 7, n = 7;
printf("Пример 3 (одинаковые значения):\n");
printf("До swap: m = %d, n = %d\n", m, n);
swap(&m, &n);
printf("После swap: m = %d, n = %d\n\n", m, n);    
return 0;
}