# MiniTest - Мини-тестовый фреймворк для C

Простой и удобный фреймворк для модульного тестирования на языке C.

## Особенности

- Легковесный и простой в использовании
- Набор макросов для различных утверждений
- Группировка тестов в сьюты
- Понятный вывод результатов
- Цветовое оформление (✅ ❌)
- Header-only библиотека (достаточно подключить один файл)

## Структура проекта
minitest/
├── include/
│ └── minitest.h # Основной заголовочный файл
├── src/
│ └── main.c # Демонстрация использования
├── tests/
│ └── test_minitest.c # Тесты самого фреймворка
├── Makefile
├── meson.build
└── README.md


## Макросы для утверждений

| Макрос | Описание |
|--------|----------|
| `ASSERT_TRUE(cond)` | Проверяет, что условие истинно |
| `ASSERT_FALSE(cond)` | Проверяет, что условие ложно |
| `ASSERT_EQ(a, b)` | Проверяет равенство двух значений |
| `ASSERT_NEQ(a, b)` | Проверяет неравенство |
| `ASSERT_STR_EQ(a, b)` | Проверяет равенство строк |
| `ASSERT_NULL(ptr)` | Проверяет, что указатель NULL |
| `ASSERT_NOT_NULL(ptr)` | Проверяет, что указатель не NULL |
| `ASSERT_DOUBLE_EQ(a, b, eps)` | Проверяет равенство double с погрешностью |

## Пример использования

```c
#include "minitest.h"

// Тестируемая функция
int add(int a, int b) {
    return a + b;
}

// Тест
void test_add() {
    ASSERT_EQ(add(2, 3), 5);
    ASSERT_EQ(add(-1, 1), 0);
    ASSERT_NEQ(add(5, 5), 9);
}

int main() {
    TestSuite suite;
    test_suite_init(&suite, "Math Tests");
    
    RUN_TEST(suite, test_add);
    
    test_suite_run(&suite);
    test_suite_summary(&suite);
    
    return suite.failed > 0 ? 1 : 0;
}