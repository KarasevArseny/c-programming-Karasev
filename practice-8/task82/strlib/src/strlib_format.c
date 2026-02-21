#include "strlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

String* str_to_upper(const String *s) {
    if (!s) return NULL;
    
    String *result = str_copy(s);
    if (!result) return NULL;
    
    for (size_t i = 0; i < result->length; i++) {
        result->data[i] = toupper(result->data[i]);
    }
    
    return result;
}

String* str_to_lower(const String *s) {
    if (!s) return NULL;
    
    String *result = str_copy(s);
    if (!result) return NULL;
    
    for (size_t i = 0; i < result->length; i++) {
        result->data[i] = tolower(result->data[i]);
    }
    
    return result;
}

String* str_trim_left(const String *s) {
    if (!s) return str_new("");
    
    const char *start = s->data;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    
    return str_new(start);
}

String* str_trim_right(const String *s) {
    if (!s) return str_new("");
    
    const char *end = s->data + s->length - 1;
    while (end >= s->data && isspace((unsigned char)*end)) {
        end--;
    }
    
    size_t len = end - s->data + 1;
    char *buffer = (char*)malloc(len + 1);
    if (!buffer) return NULL;
    
    strncpy(buffer, s->data, len);
    buffer[len] = '\0';
    
    String *result = str_new(buffer);
    free(buffer);
    return result;
}

String* str_trim(const String *s) {
    if (!s) return str_new("");
    
    const char *start = s->data;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    
    if (*start == '\0') return str_new("");
    
    const char *end = s->data + s->length - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    
    size_t len = end - start + 1;
    char *buffer = (char*)malloc(len + 1);
    if (!buffer) return NULL;
    
    strncpy(buffer, start, len);
    buffer[len] = '\0';
    
    String *result = str_new(buffer);
    free(buffer);
    return result;
}

String** str_split(const String *s, char delimiter, size_t *count) {
    if (!s || !count) return NULL;
    
    // Первый проход - подсчёт количества частей
    size_t parts = 1;
    for (size_t i = 0; i < s->length; i++) {
        if (s->data[i] == delimiter) {
            parts++;
        }
    }
    
    // Выделяем память под массив строк
    String **result = (String**)malloc(parts * sizeof(String*));
    if (!result) return NULL;
    
    // Второй проход - разделение
    size_t part_idx = 0;
    size_t start = 0;
    
    for (size_t i = 0; i <= s->length; i++) {
        if (i == s->length || s->data[i] == delimiter) {
            size_t len = i - start;
            char *buffer = (char*)malloc(len + 1);
            if (!buffer) {
                // Очистка при ошибке
                for (size_t j = 0; j < part_idx; j++) {
                    str_free(result[j]);
                }
                free(result);
                return NULL;
            }
            
            strncpy(buffer, s->data + start, len);
            buffer[len] = '\0';
            result[part_idx] = str_new(buffer);
            free(buffer);
            
            part_idx++;
            start = i + 1;
        }
    }
    
    *count = parts;
    return result;
}

String* str_join(String **parts, size_t count, const char *separator) {
    if (!parts || count == 0) return str_new("");
    
    size_t total_len = 0;
    size_t sep_len = separator ? strlen(separator) : 0;
    
    // Вычисляем общую длину
    for (size_t i = 0; i < count; i++) {
        if (parts[i]) {
            total_len += parts[i]->length;
        }
    }
    total_len += (count - 1) * sep_len;
    
    String *result = str_new_capacity(total_len + 1);
    
    for (size_t i = 0; i < count; i++) {
        if (parts[i]) {
            str_append(result, parts[i]->data);
        }
        if (i < count - 1 && separator) {
            str_append(result, separator);
        }
    }
    
    return result;
}

String* str_substring(const String *s, size_t start, size_t len) {
    if (!s) return NULL;
    if (start >= s->length) return str_new("");
    
    if (start + len > s->length) {
        len = s->length - start;
    }
    
    char *buffer = (char*)malloc(len + 1);
    if (!buffer) return NULL;
    
    strncpy(buffer, s->data + start, len);
    buffer[len] = '\0';
    
    String *result = str_new(buffer);
    free(buffer);
    return result;
}

String* str_repeat(const char *text, size_t times) {
    if (!text) return NULL;
    
    size_t text_len = strlen(text);
    String *result = str_new_capacity(text_len * times + 1);
    
    for (size_t i = 0; i < times; i++) {
        str_append(result, text);
    }
    
    return result;
}

String* str_reverse(const String *s) {
    if (!s) return NULL;
    
    String *result = str_new_capacity(s->capacity);
    
    for (size_t i = s->length; i > 0; i--) {
        str_append_char(result, s->data[i - 1]);
    }
    
    return result;
}

int str_compare(const String *a, const String *b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    
    return strcmp(a->data, b->data);
}

bool str_equals(const String *a, const String *b) {
    return str_compare(a, b) == 0;
}

bool str_equals_ignore_case(const String *a, const String *b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    
    if (a->length != b->length) return false;
    
    for (size_t i = 0; i < a->length; i++) {
        if (tolower(a->data[i]) != tolower(b->data[i])) {
            return false;
        }
    }
    
    return true;
}

void str_free_array(String **arr, size_t count) {
    if (!arr) return;
    
    for (size_t i = 0; i < count; i++) {
        if (arr[i]) {
            str_free(arr[i]);
        }
    }
    
    free(arr);
}