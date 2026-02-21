#include <stdio.h>
#include "mathutils.h"

int main(void) {
    printf("=== ТЕСТИРОВАНИЕ БИБЛИОТЕКИ MATHUTILS ===\n\n");
    
    // Тест факториала
    printf("factorial(0) = %d\n", factorial(0));
    printf("factorial(1) = %d\n", factorial(1));
    printf("factorial(5) = %d\n", factorial(5));
    printf("factorial(10) = %d\n\n", factorial(10));
    
    // Тест чисел Фибоначчи
    printf("fibonacci(0) = %d\n", fibonacci(0));
    printf("fibonacci(1) = %d\n", fibonacci(1));
    printf("fibonacci(5) = %d\n", fibonacci(5));
    printf("fibonacci(10) = %d\n", fibonacci(10));
    printf("fibonacci(15) = %d\n\n", fibonacci(15));
    
    // Тест НОД
    printf("gcd(48, 18) = %d\n", gcd(48, 18));
    printf("gcd(100, 35) = %d\n", gcd(100, 35));
    printf("gcd(17, 19) = %d\n", gcd(17, 19));
    printf("gcd(-12, 18) = %d\n\n", gcd(-12, 18));
    
    // Тест НОК
    printf("lcm(4, 6) = %d\n", lcm(4, 6));
    printf("lcm(12, 18) = %d\n", lcm(12, 18));
    printf("lcm(7, 11) = %d\n", lcm(7, 11));
    printf("lcm(0, 5) = %d\n\n", lcm(0, 5));
    
    // Тест простых чисел
    printf("Простые числа:\n");
    for (int i = 1; i <= 30; i++) {
        if (is_prime(i)) {
            printf("%d ", i);
        }
    }
    printf("\n\n");
    
    printf("is_prime(17) = %d\n", is_prime(17));
    printf("is_prime(18) = %d\n", is_prime(18));
    printf("is_prime(97) = %d\n", is_prime(97));
    printf("is_prime(100) = %d\n", is_prime(100));
    
    return 0;
}