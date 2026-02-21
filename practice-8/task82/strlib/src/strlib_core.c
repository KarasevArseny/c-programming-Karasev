#include "strlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Внутренняя функция для расширения строки
static void ensure_capacity(String *s, size_t additional) {
    if (s->length + additional + 1 > s->capacity) {
        s->capacity = s->length + additional + 1;
        if (s->capacity < 16) s->capacity = 16;
        s->data = (char*)realloc(s->data, s->capacity);
    }
}

String* str_new(const char *init) {
    String *s = (String*)malloc(sizeof(String));
    if (!s) return NULL;
    
    if (init) {
        s->length = strlen(init);
        s->capacity = s->length + 1;
        s->data = (char*)malloc(s->capacity);
        if (!s->data) {
            free(s);
            return NULL;
        }
        strcpy(s->data, init);
    } else {
        s->length = 0;
        s->capacity = 16;
        s->data = (char*)malloc(s->capacity);
        if (!s->data) {
            free(s);
            return NULL;
        }
        s->data[0] = '\0';
    }
    
    return s;
}

String* str_new_capacity(size_t capacity) {
    String *s = (String*)malloc(sizeof(String));
    if (!s) return NULL;
    
    s->length = 0;
    s->capacity = capacity < 1 ? 1 : capacity;
    s->data = (char*)malloc(s->capacity);
    
    if (!s->data) {
        free(s);
        return NULL;
    }
    
    s->data[0] = '\0';
    return s;
}

void str_free(String *s) {
    if (s) {
        free(s->data);
        free(s);
    }
}

String* str_copy(const String *s) {
    if (!s) return NULL;
    return str_new(s->data);
}

size_t str_length(const String *s) {
    return s ? s->length : 0;
}

const char* str_cstr(const String *s) {
    return s ? s->data : NULL;
}

bool str_empty(const String *s) {
    return s ? s->length == 0 : true;
}

void str_clear(String *s) {
    if (s) {
        s->length = 0;
        s->data[0] = '\0';
    }
}

// === Modification ===

void str_append(String *s, const char *text) {
    if (!s || !text) return;
    
    size_t text_len = strlen(text);
    ensure_capacity(s, text_len);
    
    strcpy(s->data + s->length, text);
    s->length += text_len;
}

void str_append_char(String *s, char c) {
    if (!s) return;
    
    ensure_capacity(s, 1);
    
    s->data[s->length] = c;
    s->length++;
    s->data[s->length] = '\0';
}

void str_prepend(String *s, const char *text) {
    if (!s || !text) return;
    
    size_t text_len = strlen(text);
    ensure_capacity(s, text_len);
    
    // Сдвигаем существующий текст вправо
    memmove(s->data + text_len, s->data, s->length + 1);
    
    // Копируем новый текст в начало
    memcpy(s->data, text, text_len);
    s->length += text_len;
}

void str_insert(String *s, size_t pos, const char *text) {
    if (!s || !text) return;
    if (pos > s->length) pos = s->length;
    
    size_t text_len = strlen(text);
    ensure_capacity(s, text_len);
    
    // Сдвигаем часть после pos
    memmove(s->data + pos + text_len, s->data + pos, s->length - pos + 1);
    
    // Вставляем новый текст
    memcpy(s->data + pos, text, text_len);
    s->length += text_len;
}

void str_erase(String *s, size_t pos, size_t len) {
    if (!s) return;
    if (pos >= s->length) return;
    
    if (pos + len > s->length) {
        len = s->length - pos;
    }
    
    // Сдвигаем оставшуюся часть
    memmove(s->data + pos, s->data + pos + len, s->length - pos - len + 1);
    s->length -= len;
}

void str_replace(String *s, const char *old, const char *new_str) {
    if (!s || !old || !new_str) return;
    
    String *result = str_new_capacity(s->capacity);
    const char *p = s->data;
    size_t old_len = strlen(old);
    size_t new_len = strlen(new_str);
    
    while (*p) {
        if (strncmp(p, old, old_len) == 0) {
            str_append(result, new_str);
            p += old_len;
        } else {
            str_append_char(result, *p);
            p++;
        }
    }
    
    // Заменяем содержимое
    free(s->data);
    s->data = result->data;
    s->length = result->length;
    s->capacity = result->capacity;
    free(result); // Не освобождаем data, так как она уже используется
}