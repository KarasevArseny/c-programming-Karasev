#ifndef DATA_H
#define DATA_H

#include <stddef.h>
#include <time.h>

// Максимальные размеры
#define MAX_CATEGORY_NAME 50
#define MAX_ACCOUNT_NAME 50
#define MAX_DESCRIPTION 200
#define MAX_CATEGORIES 50
#define MAX_ACCOUNTS 10

// Типы транзакций
typedef enum {
    TRANSACTION_INCOME,
    TRANSACTION_EXPENSE
} TransactionType;

// Структура транзакции
typedef struct {
    int id;
    TransactionType type;
    double amount;
    char category[MAX_CATEGORY_NAME];
    char description[MAX_DESCRIPTION];
    time_t date;
    char account[MAX_ACCOUNT_NAME];
} Transaction;

// Структура счёта
typedef struct {
    char name[MAX_ACCOUNT_NAME];
    double balance;
} Account;

// Основная структура данных
typedef struct {
    Transaction *transactions;
    int transaction_count;
    int transaction_capacity;
    int next_transaction_id;
    
    Account accounts[MAX_ACCOUNTS];
    int account_count;
    
    char categories[MAX_CATEGORIES][MAX_CATEGORY_NAME];
    int category_count;
} FinanceData;

// Инициализация и очистка
void finance_init(FinanceData *data);
void finance_cleanup(FinanceData *data);

// Операции со счетами
int finance_add_account(FinanceData *data, const char *name, double initial_balance);
int finance_remove_account(FinanceData *data, const char *name);
Account* finance_find_account(FinanceData *data, const char *name);

// Операции с категориями
int finance_add_category(FinanceData *data, const char *name);
int finance_remove_category(FinanceData *data, const char *name);
int finance_category_exists(const FinanceData *data, const char *name);

// Операции с транзакциями
int finance_add_transaction(FinanceData *data, TransactionType type, 
                            double amount, const char *category, 
                            const char *description, const char *account,
                            time_t date);
int finance_edit_transaction(FinanceData *data, int id, TransactionType type,
                             double amount, const char *category,
                             const char *description, const char *account,
                             time_t date);
int finance_delete_transaction(FinanceData *data, int id);
Transaction* finance_get_transaction(const FinanceData *data, int id);

// Поиск и фильтрация
int finance_filter_by_date(const FinanceData *data, time_t start, time_t end,
                           Transaction **result, int max_results);
int finance_filter_by_category(const FinanceData *data, const char *category,
                               Transaction **result, int max_results);
int finance_filter_by_account(const FinanceData *data, const char *account,
                              Transaction **result, int max_results);

// Статистика
double finance_get_balance(const FinanceData *data);
double finance_get_balance_by_account(const FinanceData *data, const char *account);
double finance_get_income_total(const FinanceData *data, time_t start, time_t end);
double finance_get_expense_total(const FinanceData *data, time_t start, time_t end);
void finance_get_category_stats(const FinanceData *data, time_t start, time_t end,
                                char categories[][MAX_CATEGORY_NAME], 
                                double amounts[], int *count);

// Сортировка
void finance_sort_by_date(Transaction *transactions, int count, int ascending);
void finance_sort_by_amount(Transaction *transactions, int count, int ascending);
void finance_sort_by_category(Transaction *transactions, int count, int ascending);

#endif // DATA_H