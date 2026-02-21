#ifndef STRLIB_H
#define STRLIB_H

#include <stddef.h>
#include <stdbool.h>

// Динамическая строка
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String;

// === Core ===
String* str_new(const char *init);
String* str_new_capacity(size_t capacity);
void str_free(String *s);
String* str_copy(const String *s);
size_t str_length(const String *s);
const char* str_cstr(const String *s);
bool str_empty(const String *s);
void str_clear(String *s);

// === Modification ===
void str_append(String *s, const char *text);
void str_append_char(String *s, char c);
void str_prepend(String *s, const char *text);
void str_insert(String *s, size_t pos, const char *text);
void str_erase(String *s, size_t pos, size_t len);
void str_replace(String *s, const char *old, const char *new_str);

// === Search ===
int str_find(const String *s, const char *substr);
int str_find_last(const String *s, const char *substr);
int str_find_char(const String *s, char c);
bool str_contains(const String *s, const char *substr);
bool str_starts_with(const String *s, const char *prefix);
bool str_ends_with(const String *s, const char *suffix);
size_t str_count(const String *s, const char *substr);

// === Format ===
String* str_to_upper(const String *s);
String* str_to_lower(const String *s);
String* str_trim(const String *s);
String* str_trim_left(const String *s);
String* str_trim_right(const String *s);
String** str_split(const String *s, char delimiter, size_t *count);
String* str_join(String **parts, size_t count, const char *separator);
String* str_substring(const String *s, size_t start, size_t len);
String* str_repeat(const char *text, size_t times);
String* str_reverse(const String *s);

// === Comparison ===
int str_compare(const String *a, const String *b);
bool str_equals(const String *a, const String *b);
bool str_equals_ignore_case(const String *a, const String *b);

// === Utility ===
void str_free_array(String **arr, size_t count);

#endif // STRLIB_H