#include <stdio.h>
#include <stdlib.h>
#include "mathutils.h"
#include "strutils.h"

int main(void) {
    printf("========================================\n");
    printf("   ТЕСТИРОВАНИЕ БИБЛИОТЕК\n");
    printf("========================================\n\n");
    
    // Тестирование математических функций
    printf("=== MATHUTILS ===\n");
    printf("factorial(5) = %d\n", factorial(5));
    printf("fibonacci(10) = %d\n", fibonacci(10));
    printf("gcd(48, 18) = %d\n", gcd(48, 18));
    printf("lcm(4, 6) = %d\n", lcm(4, 6));
    printf("is_prime(17) = %d\n", is_prime(17));
    printf("is_prime(18) = %d\n", is_prime(18));
    
    // Тестирование строковых функций
    printf("\n=== STRUTILS ===\n");
    const char *test = "  Hello World  ";
    
    char *rev = str_reverse(test);
    char *upper = str_to_upper(test);
    char *lower = str_to_lower(test);
    char *trimmed = str_trim(test);
    int words = str_word_count(test);
    
    printf("Original: \"%s\"\n", test);
    printf("Reversed: \"%s\"\n", rev ? rev : "NULL");
    printf("Upper: \"%s\"\n", upper ? upper : "NULL");
    printf("Lower: \"%s\"\n", lower ? lower : "NULL");
    printf("Trimmed: \"%s\"\n", trimmed ? trimmed : "NULL");
    printf("Words: %d\n", words);
    
    // Освобождение памяти
    free(rev);
    free(upper);
    free(lower);
    free(trimmed);
    
    printf("\n========================================\n");
    printf("   ТЕСТИРОВАНИЕ ЗАВЕРШЕНО\n");
    printf("========================================\n");
    
    return 0;
}