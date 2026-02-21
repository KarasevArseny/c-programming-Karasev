#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "strlib.h"

// Простые тесты без фреймворка
void test_core() {
    printf("Тестирование базовых операций...\n");
    
    String *s = str_new("Hello");
    assert(s != NULL);
    assert(str_length(s) == 5);
    assert(strcmp(str_cstr(s), "Hello") == 0);
    
    str_append(s, " World");
    assert(strcmp(str_cstr(s), "Hello World") == 0);
    
    str_prepend(s, "Greeting: ");
    assert(strcmp(str_cstr(s), "Greeting: Hello World") == 0);
    
    str_clear(s);
    assert(str_length(s) == 0);
    assert(strcmp(str_cstr(s), "") == 0);
    
    str_free(s);
    printf("✓ Базовые операции прошли тест\n");
}

void test_search() {
    printf("Тестирование поиска...\n");
    
    String *s = str_new("The quick brown fox jumps over the lazy dog");
    
    assert(str_find(s, "fox") == 16);
    assert(str_find(s, "dog") == 40);
    assert(str_find(s, "cat") == -1);
    
    assert(str_find_last(s, "the") == 31);
    assert(str_contains(s, "brown") == true);
    assert(str_contains(s, "red") == false);
    
    assert(str_starts_with(s, "The") == true);
    assert(str_starts_with(s, "the") == false);
    
    assert(str_ends_with(s, "dog") == true);
    assert(str_ends_with(s, "cat") == false);
    
    assert(str_count(s, "o") == 4);
    
    str_free(s);
    printf("✓ Поиск прошёл тест\n");
}

void test_format() {
    printf("Тестирование форматирования...\n");
    
    String *s = str_new("  Hello World  ");
    
    String *upper = str_to_upper(s);
    assert(strcmp(str_cstr(upper), "  HELLO WORLD  ") == 0);
    
    String *lower = str_to_lower(s);
    assert(strcmp(str_cstr(lower), "  hello world  ") == 0);
    
    String *trimmed = str_trim(s);
    assert(strcmp(str_cstr(trimmed), "Hello World") == 0);
    
    String *trim_left = str_trim_left(s);
    assert(strcmp(str_cstr(trim_left), "Hello World  ") == 0);
    
    String *trim_right = str_trim_right(s);
    assert(strcmp(str_cstr(trim_right), "  Hello World") == 0);
    
    str_free(upper);
    str_free(lower);
    str_free(trimmed);
    str_free(trim_left);
    str_free(trim_right);
    str_free(s);
    
    printf("✓ Форматирование прошло тест\n");
}

void test_split_join() {
    printf("Тестирование разделения и соединения...\n");
    
    String *s = str_new("apple,banana,cherry,date");
    
    size_t count;
    String **parts = str_split(s, ',', &count);
    
    assert(count == 4);
    assert(strcmp(str_cstr(parts[0]), "apple") == 0);
    assert(strcmp(str_cstr(parts[1]), "banana") == 0);
    assert(strcmp(str_cstr(parts[2]), "cherry") == 0);
    assert(strcmp(str_cstr(parts[3]), "date") == 0);
    
    String *joined = str_join(parts, count, " | ");
    assert(strcmp(str_cstr(joined), "apple | banana | cherry | date") == 0);
    
    str_free_array(parts, count);
    str_free(joined);
    str_free(s);
    
    printf("✓ Разделение и соединение прошли тест\n");
}

void test_substring() {
    printf("Тестирование подстрок...\n");
    
    String *s = str_new("Hello World");
    
    String *sub1 = str_substring(s, 0, 5);
    assert(strcmp(str_cstr(sub1), "Hello") == 0);
    
    String *sub2 = str_substring(s, 6, 5);
    assert(strcmp(str_cstr(sub2), "World") == 0);
    
    String *sub3 = str_substring(s, 4, 10);
    assert(strcmp(str_cstr(sub3), "o World") == 0);
    
    String *repeated = str_repeat("Ha", 3);
    assert(strcmp(str_cstr(repeated), "HaHaHa") == 0);
    
    String *reversed = str_reverse(s);
    assert(strcmp(str_cstr(reversed), "dlroW olleH") == 0);
    
    str_free(sub1);
    str_free(sub2);
    str_free(sub3);
    str_free(repeated);
    str_free(reversed);
    str_free(s);
    
    printf("✓ Подстроки прошли тест\n");
}

void test_replace() {
    printf("Тестирование замены...\n");
    
    String *s = str_new("The quick brown fox jumps over the lazy fox");
    
    str_replace(s, "fox", "cat");
    assert(strcmp(str_cstr(s), "The quick brown cat jumps over the lazy cat") == 0);
    
    str_replace(s, "brown", "red");
    assert(strcmp(str_cstr(s), "The quick red cat jumps over the lazy cat") == 0);
    
    str_replace(s, "cat", "dog");
    assert(strcmp(str_cstr(s), "The quick red dog jumps over the lazy dog") == 0);
    
    str_free(s);
    
    printf("✓ Замена прошла тест\n");
}

void test_comparison() {
    printf("Тестирование сравнения...\n");
    
    String *a = str_new("Hello");
    String *b = str_new("Hello");
    String *c = str_new("hello");
    String *d = str_new("World");
    
    assert(str_compare(a, b) == 0);
    assert(str_compare(a, c) != 0);
    assert(str_compare(a, d) < 0);
    
    assert(str_equals(a, b) == true);
    assert(str_equals(a, c) == false);
    
    assert(str_equals_ignore_case(a, c) == true);
    assert(str_equals_ignore_case(a, d) == false);
    
    str_free(a);
    str_free(b);
    str_free(c);
    str_free(d);
    
    printf("✓ Сравнение прошло тест\n");
}

int main() {
    printf("========================================\n");
    printf("   ТЕСТИРОВАНИЕ БИБЛИОТЕКИ STRLIB\n");
    printf("========================================\n\n");
    
    test_core();
    test_search();
    test_format();
    test_split_join();
    test_substring();
    test_replace();
    test_comparison();
    
    printf("\n✅ Все тесты пройдены успешно!\n");
    
    return 0;
}