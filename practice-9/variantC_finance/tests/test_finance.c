#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "storage.h"
#include "utils.h"

void test_account_operations() {
    printf("Тестирование операций со счетами...\n");
    
    FinanceData data;
    finance_init(&data);
    
    // Добавление счетов
    assert(finance_add_account(&data, "Наличные", 1000) == 0);
    assert(finance_add_account(&data, "Карта", 500) == 0);
    assert(finance_add_account(&data, "Наличные", 0) == -2); // Дубликат
    
    assert(data.account_count == 2);
    
    // Поиск счёта
    Account *acc = finance_find_account(&data, "Наличные");
    assert(acc != NULL);
    assert(strcmp(acc->name, "Наличные") == 0);
    assert(acc->balance == 1000);
    
    // Удаление счёта
    assert(finance_remove_account(&data, "Наличные") == 0);
    assert(data.account_count == 1);
    assert(finance_find_account(&data, "Наличные") == NULL);
    
    finance_cleanup(&data);
    printf("✓ Операции со счетами прошли тест\n");
}

void test_category_operations() {
    printf("Тестирование операций с категориями...\n");
    
    FinanceData data;
    finance_init(&data);
    
    // Добавление категорий
    assert(finance_add_category(&data, "Продукты") == 0);
    assert(finance_add_category(&data, "Транспорт") == 0);
    assert(finance_add_category(&data, "Продукты") == -2); // Дубликат
    
    assert(data.category_count == 2);
    
    // Проверка существования
    assert(finance_category_exists(&data, "Продукты") == 1);
    assert(finance_category_exists(&data, "Развлечения") == 0);
    
    // Удаление категории
    assert(finance_remove_category(&data, "Продукты") == 0);
    assert(data.category_count == 1);
    assert(finance_category_exists(&data, "Продукты") == 0);
    
    finance_cleanup(&data);
    printf("✓ Операции с категориями прошли тест\n");
}

void test_transaction_operations() {
    printf("Тестирование операций с транзакциями...\n");
    
    FinanceData data;
    finance_init(&data);
    
    finance_add_account(&data, "Наличные", 1000);
    finance_add_category(&data, "Продукты");
    
    // Добавление транзакции
    time_t now = time(NULL);
    int id = finance_add_transaction(&data, TRANSACTION_EXPENSE, 500, 
                                     "Продукты", "Супермаркет", "Наличные", now);
    assert(id > 0);
    assert(data.transaction_count == 1);
    
    // Проверка баланса счёта
    Account *acc = finance_find_account(&data, "Наличные");
    assert(acc->balance == 500); // 1000 - 500 = 500
    
    // Получение транзакции
    Transaction *t = finance_get_transaction(&data, id);
    assert(t != NULL);
    assert(t->type == TRANSACTION_EXPENSE);
    assert(t->amount == 500);
    assert(strcmp(t->category, "Продукты") == 0);
    
    // Редактирование транзакции
    assert(finance_edit_transaction(&data, id, TRANSACTION_INCOME, 600,
                                    "Продукты", "Возврат", "Наличные", now) == 0);
    
    // Проверка нового баланса
    assert(acc->balance == 1600); // 1000 + 600 = 1600
    
    // Удаление транзакции
    assert(finance_delete_transaction(&data, id) == 0);
    assert(data.transaction_count == 0);
    assert(finance_get_transaction(&data, id) == NULL);
    
    // Проверка восстановления баланса
    assert(acc->balance == 1000);
    
    finance_cleanup(&data);
    printf("✓ Операции с транзакциями прошли тест\n");
}

void test_filter_and_stats() {
    printf("Тестирование фильтрации и статистики...\n");
    
    FinanceData data;
    finance_init(&data);
    
    finance_add_account(&data, "Наличные", 0);
    finance_add_category(&data, "Продукты");
    finance_add_category(&data, "Транспорт");
    
    time_t now = time(NULL);
    time_t yesterday = now - 24 * 60 * 60;
    time_t last_week = now - 7 * 24 * 60 * 60;
    
    finance_add_transaction(&data, TRANSACTION_INCOME, 10000, "Продукты",
                           "Зарплата", "Наличные", last_week);
    finance_add_transaction(&data, TRANSACTION_EXPENSE, 500, "Продукты",
                           "Магазин", "Наличные", yesterday);
    finance_add_transaction(&data, TRANSACTION_EXPENSE, 300, "Транспорт",
                           "Такси", "Наличные", now);
    
    // Фильтрация по дате
    Transaction *results[10];
    int count = finance_filter_by_date(&data, yesterday, now, results, 10);
    assert(count == 2); // Две последние транзакции
    
    // Фильтрация по категории
    count = finance_filter_by_category(&data, "Продукты", results, 10);
    assert(count == 2);
    
    count = finance_filter_by_category(&data, "Транспорт", results, 10);
    assert(count == 1);
    
    // Статистика
    double income = finance_get_income_total(&data, last_week, now);
    double expense = finance_get_expense_total(&data, last_week, now);
    assert(income == 10000);
    assert(expense == 800); // 500 + 300
    
    char cats[MAX_CATEGORIES][MAX_CATEGORY_NAME];
    double amounts[MAX_CATEGORIES];
    int cat_count;
    
    finance_get_category_stats(&data, last_week, now, cats, amounts, &cat_count);
    assert(cat_count == 2);
    
    finance_cleanup(&data);
    printf("✓ Фильтрация и статистика прошли тест\n");
}

void test_sorting() {
    printf("Тестирование сортировки...\n");
    
    FinanceData data;
    finance_init(&data);
    
    finance_add_account(&data, "Наличные", 0);
    finance_add_category(&data, "Продукты");
    
    time_t now = time(NULL);
    
    finance_add_transaction(&data, TRANSACTION_INCOME, 100, "Продукты",
                           "A", "Наличные", now - 3*24*60*60);
    finance_add_transaction(&data, TRANSACTION_INCOME, 300, "Продукты",
                           "B", "Наличные", now);
    finance_add_transaction(&data, TRANSACTION_INCOME, 200, "Продукты",
                           "C", "Наличные", now - 1*24*60*60);
    
    // Сортировка по дате
    finance_sort_by_date(data.transactions, data.transaction_count, 1); // по возрастанию
    assert(data.transactions[0].amount == 100); // Самая старая
    assert(data.transactions[2].amount == 300); // Самая новая
    
    finance_sort_by_amount(data.transactions, data.transaction_count, 1);
    assert(data.transactions[0].amount == 100);
    assert(data.transactions[2].amount == 300);
    
    finance_cleanup(&data);
    printf("✓ Сортировка прошла тест\n");
}

void test_utils() {
    printf("Тестирование вспомогательных функций...\n");
    
    // Валидация
    assert(utils_is_valid_amount(100) == 1);
    assert(utils_is_valid_amount(0) == 0);
    assert(utils_is_valid_amount(-10) == 0);
    
    assert(utils_is_valid_category("Продукты") == 1);
    assert(utils_is_valid_category("Продукты123") == 1);
    assert(utils_is_valid_category("Продукты!@#") == 0);
    
    // Форматирование даты
    time_t now = time(NULL);
    char buffer[20];
    utils_format_date(now, buffer, sizeof(buffer));
    assert(strlen(buffer) > 0);
    
    // Форматирование валюты
    utils_format_currency(1234.56, buffer, sizeof(buffer));
    assert(strstr(buffer, "1234.56") != NULL);
    
    printf("✓ Вспомогательные функции прошли тест\n");
}

int main() {
    printf("========================================\n");
    printf("   ТЕСТИРОВАНИЕ СИСТЕМЫ УЧЁТА ФИНАНСОВ\n");
    printf("========================================\n\n");
    
    test_account_operations();
    test_category_operations();
    test_transaction_operations();
    test_filter_and_stats();
    test_sorting();
    test_utils();
    
    printf("\n✅ Все тесты пройдены успешно!\n");
    
    return 0;
}