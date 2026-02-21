#include <stdio.h>
#include "mathlib.h"
#include "vector.h"
#include "matrix.h"

int main() {
    printf("========================================\n");
    printf("   БИБЛИОТЕКА МАТЕМАТИЧЕСКИХ ФУНКЦИЙ\n");
    printf("========================================\n\n");
    
    // 1. Базовые функции
    printf("1. БАЗОВЫЕ ФУНКЦИИ\n");
    printf("-------------------\n");
    printf("abs(-5.7) = %.2f\n", ml_abs(-5.7));
    printf("pow(2, 10) = %.0f\n", ml_pow(2, 10));
    printf("pow(2, -3) = %.3f\n", ml_pow(2, -3));
    printf("sqrt(16) = %.2f\n", ml_sqrt(16));
    printf("sqrt(2) = %.6f\n", ml_sqrt(2));
    printf("factorial(5) = %.0f\n", ml_factorial(5));
    printf("gcd(48, 18) = %d\n", ml_gcd(48, 18));
    printf("lcm(12, 18) = %d\n\n", ml_lcm(12, 18));
    
    // 2. Тригонометрия
    printf("2. ТРИГОНОМЕТРИЯ\n");
    printf("-----------------\n");
    double angle = MATHLIB_PI / 4;  // 45 градусов
    printf("sin(π/4) = %.6f (теоретически 0.707107)\n", ml_sin(angle));
    printf("cos(π/4) = %.6f (теоретически 0.707107)\n", ml_cos(angle));
    printf("tan(π/4) = %.6f (теоретически 1.0)\n", ml_tan(angle));
    printf("sin(π/2) = %.6f\n", ml_sin(MATHLIB_PI/2));
    printf("cos(π) = %.6f\n\n", ml_cos(MATHLIB_PI));
    
    // 3. Векторные операции
    printf("3. ВЕКТОРНЫЕ ОПЕРАЦИИ\n");
    printf("----------------------\n");
    
    double arr1[] = {1.0, 2.0, 3.0};
    double arr2[] = {4.0, 5.0, 6.0};
    
    Vector *v1 = vec_from_array(arr1, 3);
    Vector *v2 = vec_from_array(arr2, 3);
    Vector *v3 = vec_create(3);
    
    printf("v1 = "); vec_print(v1); printf("\n");
    printf("v2 = "); vec_print(v2); printf("\n");
    
    vec_add(v3, v1, v2);
    printf("v1 + v2 = "); vec_print(v3); printf("\n");
    
    vec_sub(v3, v2, v1);
    printf("v2 - v1 = "); vec_print(v3); printf("\n");
    
    double dot = vec_dot(v1, v2);
    printf("v1 · v2 = %.2f\n", dot);
    
    double norm = vec_norm(v2);
    printf("||v2|| = %.2f\n", norm);
    
    Vector *v_norm = vec_copy(v2);
    vec_normalize(v_norm);
    printf("нормализованный v2 = "); vec_print(v_norm); printf("\n\n");
    
    vec_free(v1);
    vec_free(v2);
    vec_free(v3);
    vec_free(v_norm);
    
    // 4. Матричные операции
    printf("4. МАТРИЧНЫЕ ОПЕРАЦИИ\n");
    printf("----------------------\n");
    
    double mat_data1[] = {1, 2, 3, 4};
    double mat_data2[] = {5, 6, 7, 8};
    
    Matrix *m1 = mat_create(2, 2);
    Matrix *m2 = mat_create(2, 2);
    Matrix *m3 = mat_create(2, 2);
    
    for (int i = 0; i < 4; i++) {
        m1->data[i] = mat_data1[i];
        m2->data[i] = mat_data2[i];
    }
    
    printf("Матрица A:\n");
    mat_print(m1);
    printf("\nМатрица B:\n");
    mat_print(m2);
    
    mat_add(m3, m1, m2);
    printf("\nA + B:\n");
    mat_print(m3);
    
    mat_mul(m3, m1, m2);
    printf("\nA * B:\n");
    mat_print(m3);
    
    double det = mat_determinant(m1);
    printf("\n|A| = %.2f\n", det);
    
    Matrix *inv = mat_inverse(m1);
    if (inv) {
        printf("\nA⁻¹:\n");
        mat_print(inv);
        
        // Проверка: A * A⁻¹ = I
        Matrix *prod = mat_create(2, 2);
        mat_mul(prod, m1, inv);
        printf("\nA * A⁻¹:\n");
        mat_print(prod);
        mat_free(prod);
        mat_free(inv);
    }
    
    mat_free(m1);
    mat_free(m2);
    mat_free(m3);
    
    return 0;
}