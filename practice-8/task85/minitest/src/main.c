#include <stdio.h>
#include <stdlib.h>
#include "minitest.h"

// Глобальная переменная для фреймворка
int _test_failed = 0;

// ============ ТЕСТИРУЕМЫЕ ФУНКЦИИ ============

int add(int a, int b) {
    return a + b;
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

double divide(double a, double b) {
    if (b == 0) return 0;
    return a / b;
}

char* str_duplicate(const char *s) {
    if (!s) return NULL;
    char *copy = (char*)malloc(strlen(s) + 1);
    if (copy) strcpy(copy, s);
    return copy;
}

// ============ ТЕСТЫ ============

void test_add_positive() {
    printf("add(2, 3) == 5");
    ASSERT_EQ(add(2, 3), 5);
}

void test_add_negative() {
    printf("add(-2, -3) == -5");
    ASSERT_EQ(add(-2, -3), -5);
}

void test_add_zero() {
    printf("add(5, 0) == 5");
    ASSERT_EQ(add(5, 0), 5);
}

void test_factorial_zero() {
    printf("factorial(0) == 1");
    ASSERT_EQ(factorial(0), 1);
}

void test_factorial_one() {
    printf("factorial(1) == 1");
    ASSERT_EQ(factorial(1), 1);
}

void test_factorial_five() {
    printf("factorial(5) == 120");
    ASSERT_EQ(factorial(5), 120);
}

void test_divide_normal() {
    printf("divide(10, 2) == 5");
    ASSERT_DOUBLE_EQ(divide(10, 2), 5.0, 0.001);
}

void test_divide_by_zero() {
    printf("divide(10, 0) == 0");
    ASSERT_DOUBLE_EQ(divide(10, 0), 0.0, 0.001);
}

void test_divide_fraction() {
    printf("divide(1, 3) ≈ 0.333333");
    ASSERT_DOUBLE_EQ(divide(1, 3), 0.333333, 0.001);
}

void test_str_duplicate_normal() {
    printf("str_duplicate('Hello')");
    char *s = str_duplicate("Hello");
    ASSERT_STR_EQ(s, "Hello");
    free(s);
}

void test_str_duplicate_empty() {
    printf("str_duplicate('')");
    char *s = str_duplicate("");
    ASSERT_STR_EQ(s, "");
    free(s);
}

void test_str_duplicate_null() {
    printf("str_duplicate(NULL) == NULL");
    char *s = str_duplicate(NULL);
    ASSERT_NULL(s);
}

void test_assert_true() {
    printf("ASSERT_TRUE(5 > 3)");
    ASSERT_TRUE(5 > 3);
}

void test_assert_false() {
    printf("ASSERT_FALSE(5 < 3)");
    ASSERT_FALSE(5 < 3);
}

void test_assert_neq() {
    printf("ASSERT_NEQ(5, 3)");
    ASSERT_NEQ(5, 3);
}

void test_assert_not_null() {
    printf("ASSERT_NOT_NULL('test')");
    char *s = "test";
    ASSERT_NOT_NULL(s);
}

// ============ ТЕСТЫ, КОТОРЫЕ ДОЛЖНЫ ПРОВАЛИТЬСЯ ============

void test_should_fail_eq() {
    printf("ASSERT_EQ(5, 3) - должен провалиться");
    ASSERT_EQ(5, 3);  // Этот тест провалится
}

void test_should_fail_str() {
    printf("ASSERT_STR_EQ('hello', 'world') - должен провалиться");
    ASSERT_STR_EQ("hello", "world");  // Этот тест провалится
}

// ============ ГРУППЫ ТЕСТОВ ============

void run_math_tests(TestSuite *suite) {
    RUN_TEST(*suite, test_add_positive);
    RUN_TEST(*suite, test_add_negative);
    RUN_TEST(*suite, test_add_zero);
    RUN_TEST(*suite, test_factorial_zero);
    RUN_TEST(*suite, test_factorial_one);
    RUN_TEST(*suite, test_factorial_five);
    RUN_TEST(*suite, test_divide_normal);
    RUN_TEST(*suite, test_divide_by_zero);
    RUN_TEST(*suite, test_divide_fraction);
}

void run_string_tests(TestSuite *suite) {
    RUN_TEST(*suite, test_str_duplicate_normal);
    RUN_TEST(*suite, test_str_duplicate_empty);
    RUN_TEST(*suite, test_str_duplicate_null);
}

void run_assert_tests(TestSuite *suite) {
    RUN_TEST(*suite, test_assert_true);
    RUN_TEST(*suite, test_assert_false);
    RUN_TEST(*suite, test_assert_neq);
    RUN_TEST(*suite, test_assert_not_null);
}

void run_failing_tests(TestSuite *suite) {
    RUN_TEST(*suite, test_should_fail_eq);
    RUN_TEST(*suite, test_should_fail_str);
}

// ============ ГЛАВНАЯ ФУНКЦИЯ ============

int main() {
    printf("========================================\n");
    printf("   МИНИ-ТЕСТОВЫЙ ФРЕЙМВОРК\n");
    printf("========================================\n");
    
    TestSuite math_suite;
    test_suite_init(&math_suite, "Математические функции");
    run_math_tests(&math_suite);
    test_suite_run(&math_suite);
    test_suite_summary(&math_suite);
    
    TestSuite string_suite;
    test_suite_init(&string_suite, "Строковые функции");
    run_string_tests(&string_suite);
    test_suite_run(&string_suite);
    test_suite_summary(&string_suite);
    
    TestSuite assert_suite;
    test_suite_init(&assert_suite, "Макросы утверждений");
    run_assert_tests(&assert_suite);
    test_suite_run(&assert_suite);
    test_suite_summary(&assert_suite);
    
    printf("\n========================================\n");
    printf("   ДЕМОНСТРАЦИЯ ПРОВАЛЬНЫХ ТЕСТОВ\n");
    printf("========================================\n");
    
    TestSuite failing_suite;
    test_suite_init(&failing_suite, "Ожидаемые провалы");
    run_failing_tests(&failing_suite);
    test_suite_run(&failing_suite);
    test_suite_summary(&failing_suite);
    
    printf("\n========================================\n");
    printf("   ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА\n");
    printf("========================================\n");
    
    return 0;
}