#include <stdio.h>
#include <stdlib.h>
#include "strutils.h"

int main(void) {
    const char *test = "  Hello   World  ";
    
    printf("=== ТЕСТИРОВАНИЕ БИБЛИОТЕКИ STRUTILS ===\n\n");
    
    printf("Original string: \"%s\"\n", test);
    printf("Length: %zu\n\n", strlen(test));
    
    // Тест реверса
    char *reversed = str_reverse(test);
    printf("Reversed: \"%s\"\n", reversed ? reversed : "NULL");
    
    // Тест верхнего регистра
    char *upper = str_to_upper(test);
    printf("Upper case: \"%s\"\n", upper ? upper : "NULL");
    
    // Тест нижнего регистра
    char *lower = str_to_lower(test);
    printf("Lower case: \"%s\"\n", lower ? lower : "NULL");
    
    // Тест обрезки пробелов
    char *trimmed = str_trim(test);
    printf("Trimmed: \"%s\"\n", trimmed ? trimmed : "NULL");
    
    // Тест подсчёта слов
    int words = str_word_count(test);
    printf("Word count: %d\n\n", words);
    
    // Дополнительные тесты
    printf("=== ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ ===\n\n");
    
    const char *test_cases[] = {
        "",
        "Hello",
        "   Multiple   spaces   between   words   ",
        "\t\n\r\v\f",
        "OneWord",
        "  Leading and trailing spaces  ",
        NULL
    };
    
    for (int i = 0; test_cases[i] != NULL; i++) {
        const char *tc = test_cases[i];
        char *rev = str_reverse(tc);
        char *trim = str_trim(tc);
        int wc = str_word_count(tc);
        
        printf("Original: \"%s\"\n", tc);
        printf("  Reversed: \"%s\"\n", rev ? rev : "NULL");
        printf("  Trimmed: \"%s\"\n", trim ? trim : "NULL");
        printf("  Words: %d\n\n", wc);
        
        // Освобождаем память
        free(rev);
        free(trim);
    }
    
    // Освобождаем память от основных тестов
    free(reversed);
    free(upper);
    free(lower);
    free(trimmed);
    
    return 0;
}