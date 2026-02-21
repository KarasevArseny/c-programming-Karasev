#include "storage.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#else
#include <unistd.h>
#endif

const char* storage_get_data_dir(void) {
    static char path[256] = "";
    if (path[0] == '\0') {
#ifdef _WIN32
        snprintf(path, sizeof(path), "%s\\finance_data", getenv("APPDATA"));
#else
        snprintf(path, sizeof(path), "%s/.finance", getenv("HOME"));
#endif
    }
    return path;
}

int storage_ensure_data_dir(void) {
    const char *dir = storage_get_data_dir();
    
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
#ifdef _WIN32
        return mkdir(dir);
#else
        return mkdir(dir, 0700);
#endif
    }
    return 0;
}

int storage_save(const FinanceData *data, const char *filename) {
    if (!data || !filename) return -1;
    
    // Создаём директорию для данных
    storage_ensure_data_dir();
    
    // Формируем полный путь
    char fullpath[512];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", storage_get_data_dir(), filename);
    
    FILE *file = fopen(fullpath, "wb");
    if (!file) return -2;
    
    // Сохраняем количество счетов и сами счета
    fwrite(&data->account_count, sizeof(int), 1, file);
    fwrite(data->accounts, sizeof(Account), data->account_count, file);
    
    // Сохраняем количество категорий
    fwrite(&data->category_count, sizeof(int), 1, file);
    for (int i = 0; i < data->category_count; i++) {
        fwrite(data->categories[i], MAX_CATEGORY_NAME, 1, file);
    }
    
    // Сохраняем транзакции
    fwrite(&data->transaction_count, sizeof(int), 1, file);
    fwrite(&data->next_transaction_id, sizeof(int), 1, file);
    fwrite(data->transactions, sizeof(Transaction), data->transaction_count, file);
    
    fclose(file);
    return 0;
}

int storage_load(FinanceData *data, const char *filename) {
    if (!data || !filename) return -1;
    
    // Очищаем текущие данные
    finance_cleanup(data);
    finance_init(data);
    
    // Формируем полный путь
    char fullpath[512];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", storage_get_data_dir(), filename);
    
    FILE *file = fopen(fullpath, "rb");
    if (!file) return -2;
    
    // Загружаем счета
    fread(&data->account_count, sizeof(int), 1, file);
    fread(data->accounts, sizeof(Account), data->account_count, file);
    
    // Загружаем категории
    fread(&data->category_count, sizeof(int), 1, file);
    for (int i = 0; i < data->category_count; i++) {
        fread(data->categories[i], MAX_CATEGORY_NAME, 1, file);
    }
    
    // Загружаем транзакции
    fread(&data->transaction_count, sizeof(int), 1, file);
    fread(&data->next_transaction_id, sizeof(int), 1, file);
    
    if (data->transaction_count > 0) {
        data->transactions = malloc(data->transaction_count * sizeof(Transaction));
        if (!data->transactions) {
            fclose(file);
            return -3;
        }
        data->transaction_capacity = data->transaction_count;
        fread(data->transactions, sizeof(Transaction), data->transaction_count, file);
    }
    
    fclose(file);
    return 0;
}

int storage_export_csv(const FinanceData *data, const char *filename,
                      time_t start, time_t end) {
    if (!data || !filename) return -1;
    
    // Формируем полный путь
    char fullpath[512];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", storage_get_data_dir(), filename);
    
    FILE *file = fopen(fullpath, "w");
    if (!file) return -2;
    
    // Заголовок CSV
    fprintf(file, "ID,Type,Amount,Category,Description,Account,Date\n");
    
    // Фильтруем и записываем транзакции за период
    for (int i = 0; i < data->transaction_count; i++) {
        Transaction *t = &data->transactions[i];
        if (t->date >= start && t->date <= end) {
            char date_str[20];
            utils_format_date(t->date, date_str, sizeof(date_str));
            
            fprintf(file, "%d,%s,%.2f,%s,%s,%s,%s\n",
                    t->id,
                    t->type == TRANSACTION_INCOME ? "INCOME" : "EXPENSE",
                    t->amount,
                    t->category,
                    t->description,
                    t->account,
                    date_str);
        }
    }
    
    fclose(file);
    return 0;
}

int storage_export_transactions_csv(const Transaction *transactions, int count,
                                    const char *filename) {
    if (!transactions || count <= 0 || !filename) return -1;
    
    // Формируем полный путь
    char fullpath[512];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", storage_get_data_dir(), filename);
    
    FILE *file = fopen(fullpath, "w");
    if (!file) return -2;
    
    // Заголовок CSV
    fprintf(file, "ID,Type,Amount,Category,Description,Account,Date\n");
    
    for (int i = 0; i < count; i++) {
        const Transaction *t = &transactions[i];
        char date_str[20];
        utils_format_date(t->date, date_str, sizeof(date_str));
        
        fprintf(file, "%d,%s,%.2f,%s,%s,%s,%s\n",
                t->id,
                t->type == TRANSACTION_INCOME ? "INCOME" : "EXPENSE",
                t->amount,
                t->category,
                t->description,
                t->account,
                date_str);
    }
    
    fclose(file);
    return 0;
}