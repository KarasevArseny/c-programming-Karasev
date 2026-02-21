#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "mathlib.h"
#include "vector.h"
#include "matrix.h"

// Простые тесты без фреймворка
void test_basic_functions() {
    printf("Тестирование базовых функций...\n");
    
    assert(ml_abs(5) == 5);
    assert(ml_abs(-5) == 5);
    assert(ml_abs(0) == 0);
    
    assert(ml_pow(2, 3) == 8);
    assert(ml_pow(2, 0) == 1);
    assert(fabs(ml_pow(2, -3) - 0.125) < 1e-10);
    
    assert(fabs(ml_sqrt(16) - 4) < 1e-10);
    assert(fabs(ml_sqrt(2) * ml_sqrt(2) - 2) < 1e-10);
    
    assert(ml_factorial(0) == 1);
    assert(ml_factorial(5) == 120);
    
    assert(ml_gcd(48, 18) == 6);
    assert(ml_gcd(17, 19) == 1);
    
    assert(ml_lcm(12, 18) == 36);
    
    printf("✓ Базовые функции прошли тест\n");
}

void test_trig_functions() {
    printf("Тестирование тригонометрических функций...\n");
    
    double pi = MATHLIB_PI;
    
    assert(fabs(ml_sin(0) - 0) < 1e-10);
    assert(fabs(ml_sin(pi/2) - 1) < 1e-6);
    assert(fabs(ml_sin(pi) - 0) < 1e-10);
    
    assert(fabs(ml_cos(0) - 1) < 1e-10);
    assert(fabs(ml_cos(pi/2) - 0) < 1e-10);
    assert(fabs(ml_cos(pi) + 1) < 1e-10);
    
    assert(fabs(ml_tan(0) - 0) < 1e-10);
    assert(fabs(ml_tan(pi/4) - 1) < 1e-6);
    
    assert(fabs(ml_deg_to_rad(180) - pi) < 1e-10);
    assert(fabs(ml_rad_to_deg(pi) - 180) < 1e-10);
    
    printf("✓ Тригонометрические функции прошли тест\n");
}

void test_vector_functions() {
    printf("Тестирование векторных функций...\n");
    
    double arr1[] = {1, 2, 3};
    double arr2[] = {4, 5, 6};
    
    Vector *v1 = vec_from_array(arr1, 3);
    Vector *v2 = vec_from_array(arr2, 3);
    Vector *v3 = vec_create(3);
    
    assert(v1->size == 3);
    assert(v1->data[0] == 1);
    assert(v1->data[1] == 2);
    assert(v1->data[2] == 3);
    
    vec_add(v3, v1, v2);
    assert(v3->data[0] == 5);
    assert(v3->data[1] == 7);
    assert(v3->data[2] == 9);
    
    vec_sub(v3, v2, v1);
    assert(v3->data[0] == 3);
    assert(v3->data[1] == 3);
    assert(v3->data[2] == 3);
    
    double dot = vec_dot(v1, v2);
    assert(fabs(dot - 32) < 1e-10);
    
    Vector *v_copy = vec_copy(v1);
    assert(v_copy->data[0] == v1->data[0]);
    assert(v_copy->data[1] == v1->data[1]);
    assert(v_copy->data[2] == v1->data[2]);
    
    vec_normalize(v_copy);
    assert(fabs(vec_norm(v_copy) - 1.0) < 1e-10);
    
    vec_free(v1);
    vec_free(v2);
    vec_free(v3);
    vec_free(v_copy);
    
    printf("✓ Векторные функции прошли тест\n");
}

void test_matrix_functions() {
    printf("Тестирование матричных функций...\n");
    
    Matrix *m1 = mat_create(2, 2);
    Matrix *m2 = mat_create(2, 2);
    Matrix *m3 = mat_create(2, 2);
    
    double data1[] = {1, 2, 3, 4};
    double data2[] = {5, 6, 7, 8};
    
    for (int i = 0; i < 4; i++) {
        m1->data[i] = data1[i];
        m2->data[i] = data2[i];
    }
    
    mat_add(m3, m1, m2);
    assert(m3->data[0] == 6);
    assert(m3->data[1] == 8);
    assert(m3->data[2] == 10);
    assert(m3->data[3] == 12);
    
    mat_mul(m3, m1, m2);
    assert(m3->data[0] == 1*5 + 2*7);
    assert(m3->data[1] == 1*6 + 2*8);
    assert(m3->data[2] == 3*5 + 4*7);
    assert(m3->data[3] == 3*6 + 4*8);
    
    Matrix *ident = mat_identity(3);
    assert(ident->rows == 3);
    assert(ident->cols == 3);
    assert(ident->data[0] == 1);
    assert(ident->data[4] == 1);
    assert(ident->data[8] == 1);
    mat_free(ident);
    
    Matrix *trans = mat_transpose(m1);
    assert(trans->data[0] == 1);
    assert(trans->data[1] == 3);
    assert(trans->data[2] == 2);
    assert(trans->data[3] == 4);
    mat_free(trans);
    
    double det = mat_determinant(m1);
    assert(fabs(det - (1*4 - 2*3)) < 1e-10);
    
    mat_free(m1);
    mat_free(m2);
    mat_free(m3);
    
    printf("✓ Матричные функции прошли тест\n");
}

int main() {
    printf("========================================\n");
    printf("   ТЕСТИРОВАНИЕ БИБЛИОТЕКИ MATHLIB\n");
    printf("========================================\n\n");
    
    test_basic_functions();
    test_trig_functions();
    test_vector_functions();
    test_matrix_functions();
    
    printf("\n✅ Все тесты пройдены успешно!\n");
    
    return 0;
}