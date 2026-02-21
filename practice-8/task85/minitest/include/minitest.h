#ifndef MINITEST_H
#define MINITEST_H

#include <stdio.h>
#include <string.h>
#include <math.h>

// ============ СТРУКТУРЫ ДЛЯ ТЕСТОВ ============

typedef void (*TestFunc)(void);

typedef struct {
    const char *name;
    TestFunc func;
} TestCase;

typedef struct {
    const char *name;
    TestCase *tests;
    int test_count;
    int passed;
    int failed;
} TestSuite;

// Глобальная переменная для отслеживания неудач в текущем тесте
extern int _test_failed;

// ============ МАКРОСЫ ДЛЯ УТВЕРЖДЕНИЙ ============

#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            printf("  ❌ FAILED: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            printf("  ❌ FAILED: %s:%d: %s != %s (%d != %d)\n", \
                   __FILE__, __LINE__, #a, #b, (int)(a), (int)(b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_NEQ(a, b) \
    do { \
        if ((a) == (b)) { \
            printf("  ❌ FAILED: %s:%d: %s == %s (%d == %d)\n", \
                   __FILE__, __LINE__, #a, #b, (int)(a), (int)(b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_STR_EQ(a, b) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            printf("  ❌ FAILED: %s:%d: \"%s\" != \"%s\"\n", \
                   __FILE__, __LINE__, (a), (b)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

#define ASSERT_NULL(ptr) ASSERT_TRUE((ptr) == NULL)

#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != NULL)

#define ASSERT_DOUBLE_EQ(a, b, eps) \
    do { \
        double _diff = (a) - (b); \
        if (_diff < 0) _diff = -_diff; \
        if (_diff > (eps)) { \
            printf("  ❌ FAILED: %s:%d: %f != %f (eps=%f)\n", \
                   __FILE__, __LINE__, (double)(a), (double)(b), (eps)); \
            _test_failed = 1; \
            return; \
        } \
    } while(0)

// ============ ФУНКЦИИ ДЛЯ РАБОТЫ С ТЕСТАМИ ============

void test_suite_init(TestSuite *suite, const char *name) {
    suite->name = name;
    suite->tests = NULL;
    suite->test_count = 0;
    suite->passed = 0;
    suite->failed = 0;
}

void test_suite_add(TestSuite *suite, const char *name, TestFunc func) {
    if (suite->test_count == 0) {
        suite->tests = (TestCase*)malloc(sizeof(TestCase));
    } else {
        suite->tests = (TestCase*)realloc(suite->tests, 
                                          (suite->test_count + 1) * sizeof(TestCase));
    }
    
    suite->tests[suite->test_count].name = name;
    suite->tests[suite->test_count].func = func;
    suite->test_count++;
}

void test_suite_run(TestSuite *suite) {
    printf("\n🔍 Запуск тестов: %s\n", suite->name);
    printf("========================================\n");
    
    suite->passed = 0;
    suite->failed = 0;
    
    for (int i = 0; i < suite->test_count; i++) {
        TestCase *test = &suite->tests[i];
        printf("\n📋 Тест #%d: %s\n", i + 1, test->name);
        printf("  ");
        
        _test_failed = 0;
        test->func();
        
        if (_test_failed) {
            printf("  ❌ ТЕСТ ПРОВАЛЕН\n");
            suite->failed++;
        } else {
            printf("  ✅ ТЕСТ ПРОЙДЕН\n");
            suite->passed++;
        }
    }
}

void test_suite_summary(const TestSuite *suite) {
    printf("\n========================================\n");
    printf("📊 РЕЗУЛЬТАТЫ ТЕСТОВ: %s\n", suite->name);
    printf("  Всего тестов: %d\n", suite->test_count);
    printf("  ✅ Пройдено:   %d\n", suite->passed);
    printf("  ❌ Провалено:  %d\n", suite->failed);
    printf("========================================\n");
    
    if (suite->failed == 0) {
        printf("✨ ВСЕ ТЕСТЫ ПРОЙДЕНЫ!\n");
    } else {
        printf("⚠️  НЕКОТОРЫЕ ТЕСТЫ НЕ ПРОЙДЕНЫ\n");
    }
}

// Макрос для добавления теста
#define RUN_TEST(suite, test_func) \
    test_suite_add(&suite, #test_func, test_func)

#endif // MINITEST_H