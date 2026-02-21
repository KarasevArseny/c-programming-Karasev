#ifndef STORAGE_H
#define STORAGE_H

#include "data.h"

// Сохранение и загрузка
int storage_save(const FinanceData *data, const char *filename);
int storage_load(FinanceData *data, const char *filename);

// Экспорт в CSV
int storage_export_csv(const FinanceData *data, const char *filename,
                       time_t start, time_t end);
int storage_export_transactions_csv(const Transaction *transactions, int count,
                                    const char *filename);

// Вспомогательные функции
const char* storage_get_data_dir(void);
int storage_ensure_data_dir(void);

#endif // STORAGE_H