#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

time_t utils_parse_date(const char *date_str) {
    if (!date_str || strlen(date_str) == 0) {
        return time(NULL);
    }
    
    struct tm tm = {0};
    if (sscanf(date_str, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) == 3) {
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        return mktime(&tm);
    }
    
    return time(NULL);
}

void utils_format_date(time_t timestamp, char *buffer, size_t size) {
    struct tm *tm_info = localtime(&timestamp);
    strftime(buffer, size, "%Y-%m-%d %H:%M", tm_info);
}

int utils_is_same_day(time_t t1, time_t t2) {
    struct tm *tm1 = localtime(&t1);
    struct tm *tm2 = localtime(&t2);
    
    return (tm1->tm_year == tm2->tm_year &&
            tm1->tm_mon == tm2->tm_mon &&
            tm1->tm_mday == tm2->tm_mday);
}

int utils_is_same_month(time_t t1, time_t t2) {
    struct tm *tm1 = localtime(&t1);
    struct tm *tm2 = localtime(&t2);
    
    return (tm1->tm_year == tm2->tm_year &&
            tm1->tm_mon == tm2->tm_mon);
}

int utils_is_same_year(time_t t1, time_t t2) {
    struct tm *tm1 = localtime(&t1);
    struct tm *tm2 = localtime(&t2);
    
    return (tm1->tm_year == tm2->tm_year);
}

void utils_format_currency(double amount, char *buffer, size_t size) {
    if (amount >= 0) {
        snprintf(buffer, size, "+%.2f ₽", amount);
    } else {
        snprintf(buffer, size, "-%.2f ₽", -amount);
    }
}

void utils_format_percent(double percent, char *buffer, size_t size) {
    snprintf(buffer, size, "%.1f%%", percent);
}

int utils_is_valid_amount(double amount) {
    return amount > 0 && amount < 1e12;  // Не больше триллиона
}

int utils_is_valid_category(const char *category) {
    if (!category || strlen(category) == 0) return 0;
    
    for (int i = 0; category[i]; i++) {
        if (!isalnum(category[i]) && category[i] != ' ' && 
            category[i] != '-' && category[i] != '_') {
            return 0;
        }
    }
    return 1;
}

int utils_is_valid_account(const char *account) {
    if (!account || strlen(account) == 0) return 0;
    
    for (int i = 0; account[i]; i++) {
        if (!isalnum(account[i]) && account[i] != ' ' && account[i] != '-') {
            return 0;
        }
    }
    return 1;
}

void utils_safe_strcpy(char *dest, const char *src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) return;
    
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

int utils_file_exists(const char *filename) {
    struct stat st;
    return (stat(filename, &st) == 0);
}

time_t utils_current_time(void) {
    return time(NULL);
}

void utils_clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}