#include "data.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void finance_init(FinanceData *data) {
    if (!data) return;
    
    data->transactions = NULL;
    data->transaction_count = 0;
    data->transaction_capacity = 0;
    data->next_transaction_id = 1;
    
    data->account_count = 0;
    data->category_count = 0;
}

void finance_cleanup(FinanceData *data) {
    if (!data) return;
    
    if (data->transactions) {
        free(data->transactions);
        data->transactions = NULL;
    }
    
    data->transaction_count = 0;
    data->transaction_capacity = 0;
}

int finance_add_account(FinanceData *data, const char *name, double initial_balance) {
    if (!data || !name || data->account_count >= MAX_ACCOUNTS) {
        return -1;
    }
    
    // Проверяем, существует ли уже такой счёт
    for (int i = 0; i < data->account_count; i++) {
        if (strcmp(data->accounts[i].name, name) == 0) {
            return -2; // Счёт уже существует
        }
    }
    
    Account *acc = &data->accounts[data->account_count];
    utils_safe_strcpy(acc->name, name, MAX_ACCOUNT_NAME);
    acc->balance = initial_balance;
    data->account_count++;
    
    return 0;
}

int finance_remove_account(FinanceData *data, const char *name) {
    if (!data || !name) return -1;
    
    int index = -1;
    for (int i = 0; i < data->account_count; i++) {
        if (strcmp(data->accounts[i].name, name) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) return -2; // Не найден
    
    // Проверяем, есть ли транзакции на этом счёте
    for (int i = 0; i < data->transaction_count; i++) {
        if (strcmp(data->transactions[i].account, name) == 0) {
            return -3; // Есть транзакции, нельзя удалить
        }
    }
    
    // Сдвигаем оставшиеся счета
    for (int i = index; i < data->account_count - 1; i++) {
        data->accounts[i] = data->accounts[i + 1];
    }
    data->account_count--;
    
    return 0;
}

Account* finance_find_account(FinanceData *data, const char *name) {
    if (!data || !name) return NULL;
    
    for (int i = 0; i < data->account_count; i++) {
        if (strcmp(data->accounts[i].name, name) == 0) {
            return &data->accounts[i];
        }
    }
    
    return NULL;
}

int finance_add_category(FinanceData *data, const char *name) {
    if (!data || !name || data->category_count >= MAX_CATEGORIES) {
        return -1;
    }
    
    // Проверяем, существует ли уже такая категория
    for (int i = 0; i < data->category_count; i++) {
        if (strcmp(data->categories[i], name) == 0) {
            return -2;
        }
    }
    
    utils_safe_strcpy(data->categories[data->category_count], name, MAX_CATEGORY_NAME);
    data->category_count++;
    
    return 0;
}

int finance_remove_category(FinanceData *data, const char *name) {
    if (!data || !name) return -1;
    
    int index = -1;
    for (int i = 0; i < data->category_count; i++) {
        if (strcmp(data->categories[i], name) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) return -2;
    
    // Проверяем, есть ли транзакции с этой категорией
    for (int i = 0; i < data->transaction_count; i++) {
        if (strcmp(data->transactions[i].category, name) == 0) {
            return -3;
        }
    }
    
    // Сдвигаем оставшиеся категории
    for (int i = index; i < data->category_count - 1; i++) {
        utils_safe_strcpy(data->categories[i], data->categories[i + 1], MAX_CATEGORY_NAME);
    }
    data->category_count--;
    
    return 0;
}

int finance_category_exists(const FinanceData *data, const char *name) {
    if (!data || !name) return 0;
    
    for (int i = 0; i < data->category_count; i++) {
        if (strcmp(data->categories[i], name) == 0) {
            return 1;
        }
    }
    
    return 0;
}

int finance_add_transaction(FinanceData *data, TransactionType type,
                           double amount, const char *category,
                           const char *description, const char *account,
                           time_t date) {
    if (!data || !category || !description || !account) return -1;
    if (amount <= 0) return -2;
    
    // Проверяем существование счёта
    Account *acc = finance_find_account(data, account);
    if (!acc) return -3;
    
    // Проверяем существование категории (если указана)
    if (strlen(category) > 0 && !finance_category_exists(data, category)) {
        return -4;
    }
    
    // Расширяем массив при необходимости
    if (data->transaction_count >= data->transaction_capacity) {
        int new_capacity = data->transaction_capacity == 0 ? 10 : data->transaction_capacity * 2;
        Transaction *new_trans = realloc(data->transactions, new_capacity * sizeof(Transaction));
        if (!new_trans) return -5;
        
        data->transactions = new_trans;
        data->transaction_capacity = new_capacity;
    }
    
    // Создаём транзакцию
    Transaction *t = &data->transactions[data->transaction_count];
    t->id = data->next_transaction_id++;
    t->type = type;
    t->amount = amount;
    utils_safe_strcpy(t->category, category, MAX_CATEGORY_NAME);
    utils_safe_strcpy(t->description, description, MAX_DESCRIPTION);
    utils_safe_strcpy(t->account, account, MAX_ACCOUNT_NAME);
    t->date = date;
    
    // Обновляем баланс счёта
    if (type == TRANSACTION_INCOME) {
        acc->balance += amount;
    } else {
        acc->balance -= amount;
    }
    
    data->transaction_count++;
    return t->id;
}

int finance_edit_transaction(FinanceData *data, int id, TransactionType type,
                            double amount, const char *category,
                            const char *description, const char *account,
                            time_t date) {
    if (!data) return -1;
    
    Transaction *t = finance_get_transaction(data, id);
    if (!t) return -2;
    
    // Восстанавливаем старый баланс
    Account *old_acc = finance_find_account(data, t->account);
    if (old_acc) {
        if (t->type == TRANSACTION_INCOME) {
            old_acc->balance -= t->amount;
        } else {
            old_acc->balance += t->amount;
        }
    }
    
    // Обновляем данные
    t->type = type;
    t->amount = amount;
    utils_safe_strcpy(t->category, category, MAX_CATEGORY_NAME);
    utils_safe_strcpy(t->description, description, MAX_DESCRIPTION);
    utils_safe_strcpy(t->account, account, MAX_ACCOUNT_NAME);
    t->date = date;
    
    // Применяем новый баланс
    Account *new_acc = finance_find_account(data, account);
    if (new_acc) {
        if (type == TRANSACTION_INCOME) {
            new_acc->balance += amount;
        } else {
            new_acc->balance -= amount;
        }
    }
    
    return 0;
}

int finance_delete_transaction(FinanceData *data, int id) {
    if (!data) return -1;
    
    int index = -1;
    for (int i = 0; i < data->transaction_count; i++) {
        if (data->transactions[i].id == id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) return -2;
    
    // Восстанавливаем баланс счёта
    Transaction *t = &data->transactions[index];
    Account *acc = finance_find_account(data, t->account);
    if (acc) {
        if (t->type == TRANSACTION_INCOME) {
            acc->balance -= t->amount;
        } else {
            acc->balance += t->amount;
        }
    }
    
    // Сдвигаем оставшиеся транзакции
    for (int i = index; i < data->transaction_count - 1; i++) {
        data->transactions[i] = data->transactions[i + 1];
    }
    
    data->transaction_count--;
    return 0;
}

Transaction* finance_get_transaction(const FinanceData *data, int id) {
    if (!data) return NULL;
    
    for (int i = 0; i < data->transaction_count; i++) {
        if (data->transactions[i].id == id) {
            return &data->transactions[i];
        }
    }
    
    return NULL;
}

int finance_filter_by_date(const FinanceData *data, time_t start, time_t end,
                          Transaction **result, int max_results) {
    if (!data || !result) return 0;
    
    int count = 0;
    for (int i = 0; i < data->transaction_count && count < max_results; i++) {
        if (data->transactions[i].date >= start && data->transactions[i].date <= end) {
            result[count++] = &data->transactions[i];
        }
    }
    
    return count;
}

int finance_filter_by_category(const FinanceData *data, const char *category,
                              Transaction **result, int max_results) {
    if (!data || !category || !result) return 0;
    
    int count = 0;
    for (int i = 0; i < data->transaction_count && count < max_results; i++) {
        if (strcmp(data->transactions[i].category, category) == 0) {
            result[count++] = &data->transactions[i];
        }
    }
    
    return count;
}

int finance_filter_by_account(const FinanceData *data, const char *account,
                             Transaction **result, int max_results) {
    if (!data || !account || !result) return 0;
    
    int count = 0;
    for (int i = 0; i < data->transaction_count && count < max_results; i++) {
        if (strcmp(data->transactions[i].account, account) == 0) {
            result[count++] = &data->transactions[i];
        }
    }
    
    return count;
}

double finance_get_balance(const FinanceData *data) {
    if (!data) return 0;
    
    double total = 0;
    for (int i = 0; i < data->account_count; i++) {
        total += data->accounts[i].balance;
    }
    return total;
}

double finance_get_balance_by_account(const FinanceData *data, const char *account) {
    if (!data || !account) return 0;
    
    Account *acc = finance_find_account((FinanceData*)data, account);
    return acc ? acc->balance : 0;
}

double finance_get_income_total(const FinanceData *data, time_t start, time_t end) {
    if (!data) return 0;
    
    double total = 0;
    for (int i = 0; i < data->transaction_count; i++) {
        Transaction *t = &data->transactions[i];
        if (t->type == TRANSACTION_INCOME && t->date >= start && t->date <= end) {
            total += t->amount;
        }
    }
    
    return total;
}

double finance_get_expense_total(const FinanceData *data, time_t start, time_t end) {
    if (!data) return 0;
    
    double total = 0;
    for (int i = 0; i < data->transaction_count; i++) {
        Transaction *t = &data->transactions[i];
        if (t->type == TRANSACTION_EXPENSE && t->date >= start && t->date <= end) {
            total += t->amount;
        }
    }
    
    return total;
}

void finance_get_category_stats(const FinanceData *data, time_t start, time_t end,
                               char categories[][MAX_CATEGORY_NAME], 
                               double amounts[], int *count) {
    if (!data || !categories || !amounts || !count) return;
    
    *count = 0;
    
    // Собираем суммы по категориям
    for (int i = 0; i < data->transaction_count; i++) {
        Transaction *t = &data->transactions[i];
        if (t->type == TRANSACTION_EXPENSE && t->date >= start && t->date <= end) {
            int found = -1;
            for (int j = 0; j < *count; j++) {
                if (strcmp(categories[j], t->category) == 0) {
                    found = j;
                    break;
                }
            }
            
            if (found >= 0) {
                amounts[found] += t->amount;
            } else if (*count < MAX_CATEGORIES) {
                utils_safe_strcpy(categories[*count], t->category, MAX_CATEGORY_NAME);
                amounts[*count] = t->amount;
                (*count)++;
            }
        }
    }
}

// Функции сортировки
static int cmp_date_asc(const void *a, const void *b) {
    const Transaction *ta = *(const Transaction**)a;
    const Transaction *tb = *(const Transaction**)b;
    return (ta->date > tb->date) - (ta->date < tb->date);
}

static int cmp_date_desc(const void *a, const void *b) {
    return -cmp_date_asc(a, b);
}

static int cmp_amount_asc(const void *a, const void *b) {
    const Transaction *ta = *(const Transaction**)a;
    const Transaction *tb = *(const Transaction**)b;
    return (ta->amount > tb->amount) - (ta->amount < tb->amount);
}

static int cmp_amount_desc(const void *a, const void *b) {
    return -cmp_amount_asc(a, b);
}

static int cmp_category_asc(const void *a, const void *b) {
    const Transaction *ta = *(const Transaction**)a;
    const Transaction *tb = *(const Transaction**)b;
    return strcmp(ta->category, tb->category);
}

static int cmp_category_desc(const void *a, const void *b) {
    return -strcmp((*(const Transaction**)a)->category, 
                   (*(const Transaction**)b)->category);
}

void finance_sort_by_date(Transaction *transactions, int count, int ascending) {
    Transaction **ptrs = malloc(count * sizeof(Transaction*));
    for (int i = 0; i < count; i++) ptrs[i] = &transactions[i];
    
    qsort(ptrs, count, sizeof(Transaction*), 
          ascending ? cmp_date_asc : cmp_date_desc);
    
    // Переставляем элементы
    Transaction temp;
    for (int i = 0; i < count; i++) {
        if (ptrs[i] != &transactions[i]) {
            temp = *ptrs[i];
            *ptrs[i] = transactions[i];
            transactions[i] = temp;
        }
    }
    
    free(ptrs);
}

void finance_sort_by_amount(Transaction *transactions, int count, int ascending) {
    Transaction **ptrs = malloc(count * sizeof(Transaction*));
    for (int i = 0; i < count; i++) ptrs[i] = &transactions[i];
    
    qsort(ptrs, count, sizeof(Transaction*), 
          ascending ? cmp_amount_asc : cmp_amount_desc);
    
    Transaction temp;
    for (int i = 0; i < count; i++) {
        if (ptrs[i] != &transactions[i]) {
            temp = *ptrs[i];
            *ptrs[i] = transactions[i];
            transactions[i] = temp;
        }
    }
    
    free(ptrs);
}

void finance_sort_by_category(Transaction *transactions, int count, int ascending) {
    Transaction **ptrs = malloc(count * sizeof(Transaction*));
    for (int i = 0; i < count; i++) ptrs[i] = &transactions[i];
    
    qsort(ptrs, count, sizeof(Transaction*), 
          ascending ? cmp_category_asc : cmp_category_desc);
    
    Transaction temp;
    for (int i = 0; i < count; i++) {
        if (ptrs[i] != &transactions[i]) {
            temp = *ptrs[i];
            *ptrs[i] = transactions[i];
            transactions[i] = temp;
        }
    }
    
    free(ptrs);
}