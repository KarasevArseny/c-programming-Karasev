#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <time.h>

// Работа с датами
time_t utils_parse_date(const char *date_str);
void utils_format_date(time_t timestamp, char *buffer, size_t size);
int utils_is_same_day(time_t t1, time_t t2);
int utils_is_same_month(time_t t1, time_t t2);
int utils_is_same_year(time_t t1, time_t t2);

// Форматирование чисел
void utils_format_currency(double amount, char *buffer, size_t size);
void utils_format_percent(double percent, char *buffer, size_t size);

// Валидация
int utils_is_valid_amount(double amount);
int utils_is_valid_category(const char *category);
int utils_is_valid_account(const char *account);

// Безопасное копирование строк
void utils_safe_strcpy(char *dest, const char *src, size_t dest_size);

// Проверка существования файла
int utils_file_exists(const char *filename);

// Получение текущего времени
time_t utils_current_time(void);

// Очистка буфера ввода
void utils_clear_input_buffer(void);

#endif // UTILS_H