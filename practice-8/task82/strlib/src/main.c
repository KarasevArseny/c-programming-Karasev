#include <stdio.h>
#include "strlib.h"

void print_string(const char *label, const String *s) {
    printf("%s: \"%s\" (длина=%zu, ёмкость=%zu)\n", 
           label, str_cstr(s), str_length(s), s->capacity);
}

int main() {
    printf("========================================\n");
    printf("   БИБЛИОТЕКА ДЛЯ РАБОТЫ СО СТРОКАМИ\n");
    printf("========================================\n\n");
    
    // 1. Создание и базовые операции
    printf("1. БАЗОВЫЕ ОПЕРАЦИИ\n");
    printf("-------------------\n");
    
    String *s1 = str_new("Hello");
    String *s2 = str_new(" World");
    
    print_string("s1", s1);
    print_string("s2", s2);
    
    str_append(s1, str_cstr(s2));
    print_string("s1 после append", s1);
    
    str_prepend(s1, "Greeting: ");
    print_string("s1 после prepend", s1);
    
    str_insert(s1, 9, "beautiful ");
    print_string("s1 после insert", s1);
    
    str_erase(s1, 0, 10);
    print_string("s1 после erase", s1);
    
    printf("\n");
    
    // 2. Поиск
    printf("2. ПОИСК\n");
    printf("--------\n");
    
    String *text = str_new("The quick brown fox jumps over the lazy dog");
    print_string("text", text);
    
    printf("Поиск 'fox': позиция %d\n", str_find(text, "fox"));
    printf("Поиск 'dog': позиция %d\n", str_find(text, "dog"));
    printf("Поиск 'cat': позиция %d\n", str_find(text, "cat"));
    
    printf("Содержит 'brown'? %s\n", str_contains(text, "brown") ? "да" : "нет");
    printf("Начинается с 'The'? %s\n", str_starts_with(text, "The") ? "да" : "нет");
    printf("Заканчивается на 'dog'? %s\n", str_ends_with(text, "dog") ? "да" : "нет");
    printf("Количество 'o': %zu\n", str_count(text, "o"));
    
    printf("\n");
    
    // 3. Форматирование
    printf("3. ФОРМАТИРОВАНИЕ\n");
    printf("-----------------\n");
    
    String *upper = str_to_upper(text);
    String *lower = str_to_lower(text);
    
    print_string("Верхний регистр", upper);
    print_string("Нижний регистр", lower);
    
    String *spaced = str_new("  \t  Hello World  \n  ");
    print_string("Строка с пробелами", spaced);
    
    String *trimmed = str_trim(spaced);
    print_string("После trim", trimmed);
    
    String *trim_left = str_trim_left(spaced);
    print_string("После trim_left", trim_left);
    
    String *trim_right = str_trim_right(spaced);
    print_string("После trim_right", trim_right);
    
    printf("\n");
    
    // 4. Разделение и соединение
    printf("4. РАЗДЕЛЕНИЕ И СОЕДИНЕНИЕ\n");
    printf("--------------------------\n");
    
    String *csv = str_new("apple,banana,cherry,date");
    print_string("CSV строка", csv);
    
    size_t part_count;
    String **parts = str_split(csv, ',', &part_count);
    
    printf("Разделение на %zu частей:\n", part_count);
    for (size_t i = 0; i < part_count; i++) {
        printf("  [%zu] \"%s\"\n", i, str_cstr(parts[i]));
    }
    
    String *joined = str_join(parts, part_count, " | ");
    print_string("После соединения с ' | '", joined);
    
    str_free_array(parts, part_count);
    
    printf("\n");
    
    // 5. Подстроки и повторение
    printf("5. ПОДСТРОКИ И ПОВТОРЕНИЕ\n");
    printf("-------------------------\n");
    
    String *sub = str_substring(text, 4, 10);
    print_string("Подстрока (позиция 4, длина 10)", sub);
    
    String *repeated = str_repeat("Ha", 5);
    print_string("Повторение 'Ha' 5 раз", repeated);
    
    String *reversed = str_reverse(text);
    print_string("Реверс строки", reversed);
    
    printf("\n");
    
    // 6. Замена
    printf("6. ЗАМЕНА\n");
    printf("---------\n");
    
    String *replace_test = str_new("The quick brown fox jumps over the lazy fox");
    print_string("До замены", replace_test);
    
    str_replace(replace_test, "fox", "cat");
    print_string("После замены 'fox' на 'cat'", replace_test);
    
    str_replace(replace_test, "brown", "red");
    print_string("После замены 'brown' на 'red'", replace_test);
    
    printf("\n");
    
    // 7. Сравнение
    printf("7. СРАВНЕНИЕ\n");
    printf("------------\n");
    
    String *a = str_new("Hello");
    String *b = str_new("Hello");
    String *c = str_new("hello");
    
    printf("a = \"Hello\", b = \"Hello\", c = \"hello\"\n");
    printf("str_equals(a, b): %s\n", str_equals(a, b) ? "да" : "нет");
    printf("str_equals(a, c): %s\n", str_equals(a, c) ? "да" : "нет");
    printf("str_equals_ignore_case(a, c): %s\n", 
           str_equals_ignore_case(a, c) ? "да" : "нет");
    
    // Очистка памяти
    str_free(s1);
    str_free(s2);
    str_free(text);
    str_free(upper);
    str_free(lower);
    str_free(spaced);
    str_free(trimmed);
    str_free(trim_left);
    str_free(trim_right);
    str_free(csv);
    str_free(joined);
    str_free(sub);
    str_free(repeated);
    str_free(reversed);
    str_free(replace_test);
    str_free(a);
    str_free(b);
    str_free(c);
    
    return 0;
}