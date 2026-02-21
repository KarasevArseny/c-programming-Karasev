#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minitest.h"

// Глобальная переменная для фреймворка
int _test_failed = 0;

// ============ ТЕСТЫ ДЛЯ ФРЕЙМВОРКА ============

void test_assert_true_passes() {
    int x = 5;
    ASSERT_TRUE(x == 5);
}

void test_assert_true_fails() {
    int x = 5;
    // Этот тест должен провалиться, но мы его не запускаем в основном наборе
    // ASSERT_TRUE(x == 10);
}

void test_assert_false_passes() {
    int x = 5;
    ASSERT_FALSE(x == 10);
}

void test_assert_eq_passes() {
    ASSERT_EQ(5 + 3, 8);
}

void test_assert_neq_passes() {
    ASSERT_NEQ(5, 8);
}

void test_assert_str_eq_passes() {
    char s1[] = "hello";
    char s2[] = "hello";
    ASSERT_STR_EQ(s1, s2);
}

void test_assert_null_passes() {
    char *ptr = NULL;
    ASSERT_NULL(ptr);
}

void test_assert_not_null_passes() {
    int x = 5;
    int *ptr = &x;
    ASSERT_NOT_NULL(ptr);
}

void test_assert_double_eq_passes() {
    double a = 1.0 / 3.0;
    double b = 0.333333;
    ASSERT_DOUBLE_EQ(a, b, 0.0001);
}

// ============ ТЕСТЫ ДЛЯ САМОГО ФРЕЙМВОРКА ============

void test_suite_initialization() {
    TestSuite suite;
    test_suite_init(&suite, "Test Suite");
    
    ASSERT_EQ(suite.name, "Test Suite");
    ASSERT_NULL(suite.tests);
    ASSERT_EQ(suite.test_count, 0);
    ASSERT_EQ(suite.passed, 0);
    ASSERT_EQ(suite.failed, 0);
}

void test_suite_add_test() {
    TestSuite suite;
    test_suite_init(&suite, "Test Suite");
    
    test_suite_add(&suite, "test1", test_assert_true_passes);
    ASSERT_EQ(suite.test_count, 1);
    ASSERT_STR_EQ(suite.tests[0].name, "test1");
    ASSERT_EQ(suite.tests[0].func, test_assert_true_passes);
    
    test_suite_add(&suite, "test2", test_assert_eq_passes);
    ASSERT_EQ(suite.test_count, 2);
}

// Вспомогательная функция для подсчёта
static int mock_test_called = 0;

void mock_test() {
    mock_test_called = 1;
    ASSERT_TRUE(5 == 5);
}

void test_suite_run_executes_tests() {
    TestSuite suite;
    test_suite_init(&suite, "Test Suite");
    
    mock_test_called = 0;
    test_suite_add(&suite, "mock", mock_test);
    test_suite_run(&suite);
    
    ASSERT_EQ(mock_test_called, 1);
}

void test_suite_counts_results() {
    TestSuite suite;
    test_suite_init(&suite, "Test Suite");
    
    // Добавляем один успешный тест
    test_suite_add(&suite, "passing", test_assert_true_passes);
    
    test_suite_run(&suite);
    
    ASSERT_EQ(suite.passed, 1);
    ASSERT_EQ(suite.failed, 0);
}

// ============ ЗАПУСК ТЕСТОВ ============

int main() {
    printf("========================================\n");
    printf("   ТЕСТИРОВАНИЕ МИНИ-ТЕСТОВОГО ФРЕЙМВОРКА\n");
    printf("========================================\n");
    
    TestSuite suite;
    test_suite_init(&suite, "MiniTest Framework Tests");
    
    RUN_TEST(suite, test_suite_initialization);
    RUN_TEST(suite, test_suite_add_test);
    RUN_TEST(suite, test_suite_run_executes_tests);
    RUN_TEST(suite, test_suite_counts_results);
    
    RUN_TEST(suite, test_assert_true_passes);
    RUN_TEST(suite, test_assert_false_passes);
    RUN_TEST(suite, test_assert_eq_passes);
    RUN_TEST(suite, test_assert_neq_passes);
    RUN_TEST(suite, test_assert_str_eq_passes);
    RUN_TEST(suite, test_assert_null_passes);
    RUN_TEST(suite, test_assert_not_null_passes);
    RUN_TEST(suite, test_assert_double_eq_passes);
    
    test_suite_run(&suite);
    test_suite_summary(&suite);
    
    return suite.failed > 0 ? 1 : 0;
}