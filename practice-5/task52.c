#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void matrix_random(int rows, int cols, int m[rows][cols], int min, int max) {
for (int i = 0; i < rows; i++) {
for (int j = 0; j < cols; j++) {
m[i][j] = min + rand() % (max - min + 1);
}
}
}
void matrix_print(int rows, int cols, int m[rows][cols]) {
for (int i = 0; i < rows; i++) {
for (int j = 0; j < cols; j++) {
printf("%4d ", m[i][j]);
}
printf("\n");
}
}
void matrix_transpose(int rows, int cols, int src[rows][cols], int dst[cols][rows]) {
for (int i = 0; i < rows; i++) {
for (int j = 0; j < cols; j++) {
dst[j][i] = src[i][j];
}
}
}
int matrix_multiply(int r1, int c1, int a[r1][c1],
int r2, int c2, int b[r2][c2],
int result[r1][c2]) {
if (c1 != r2) {
printf("Ошибка: число столбцов матрицы A (%d) не равно числу строк матрицы B (%d)\n", c1, r2);
return -1;
}
for (int i = 0; i < r1; i++) {
for (int j = 0; j < c2; j++) {
result[i][j] = 0;
}
}
for (int i = 0; i < r1; i++) {
for (int j = 0; j < c2; j++) {
for (int k = 0; k < c1; k++) {
result[i][j] += a[i][k] * b[k][j];
}
}
}    
return 0;
}
int matrix_trace(int n, int m[n][n]) {
int trace = 0;
for (int i = 0; i < n; i++) {
trace += m[i][i];
}
return trace;
}
int main(void) {
srand(time(NULL));    
printf("=== Набор функций для работы с матрицами ===\n\n");
printf("Пример 1: Случайная матрица 3x3\n");
int mat1[3][3];
matrix_random(3, 3, mat1, 1, 10);
matrix_print(3, 3, mat1);
printf("След матрицы: %d\n\n", matrix_trace(3, mat1));
printf("Пример 2: Транспонирование матрицы 2x3\n");
int a[2][3] = {{1, 2, 3}, {4, 5, 6}};
int a_trans[3][2];    
printf("Исходная матрица A (2x3):\n");
matrix_print(2, 3, a);    
matrix_transpose(2, 3, a, a_trans);
printf("Транспонированная матрица (3x2):\n");
matrix_print(3, 2, a_trans);
printf("\n");
printf("Пример 3: Умножение матриц A (2x3) × B (3x2)\n");    
int A[2][3] = {{1, 2, 3}, {4, 5, 6}};
int B[3][2] = {{7, 8}, {9, 10}, {11, 12}};
int C[2][2];    
printf("Матрица A (2x3):\n");
matrix_print(2, 3, A);
printf("\nМатрица B (3x2):\n");
matrix_print(3, 2, B);    
int result = matrix_multiply(2, 3, A, 3, 2, B, C);
if (result == 0) {
printf("\nA × B (2x2):\n");
matrix_print(2, 2, C);
}
printf("\n");
printf("Пример 4: Полная демонстрация\n");
int n = 4;
int square[4][4];
printf("Случайная квадратная матрица %dx%d:\n", n, n);
matrix_random(n, n, square, 1, 20);
matrix_print(n, n, square);
printf("След матрицы: %d\n\n", matrix_trace(n, square));
int square_trans[4][4];
matrix_transpose(n, n, square, square_trans);
printf("Транспонированная матрица:\n");
matrix_print(n, n, square_trans);
int product[4][4];
result = matrix_multiply(n, n, square, n, n, square_trans, product);
if (result == 0) {
printf("\nИсходная × Транспонированная:\n");
matrix_print(n, n, product);
}
return 0;
}