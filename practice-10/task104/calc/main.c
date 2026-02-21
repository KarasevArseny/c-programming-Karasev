#include <stdio.h>
#include "calc.h"

int main(void) {
    printf("========================================\n");
    printf("   ТЕСТИРОВАНИЕ БИБЛИОТЕКИ CALC\n");
    printf("========================================\n\n");
    
    printf("add(10, 5) = %d\n", add(10, 5));
    printf("subtract(10, 5) = %d\n", subtract(10, 5));
    printf("multiply(10, 5) = %d\n", multiply(10, 5));
    printf("divide(10, 5) = %d\n", divide(10, 5));
    printf("power(2, 10) = %d\n", power(2, 10));
    
    printf("\n========================================\n");
    printf("   ТЕСТИРОВАНИЕ ЗАВЕРШЕНО\n");
    printf("========================================\n");
    
    return 0;
}