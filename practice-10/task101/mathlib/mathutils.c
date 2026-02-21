#include "mathutils.h"
#include <stddef.h>

// Факториал числа n (n!)
int factorial(int n) {
    if (n < 0) return -1; // Ошибка: факториал отрицательного числа не определён
    
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// N-е число Фибоначчи
int fibonacci(int n) {
    if (n < 0) return -1; // Ошибка: отрицательный индекс
    
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    int a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// Наибольший общий делитель (алгоритм Евклида)
int gcd(int a, int b) {
    // Работаем с абсолютными значениями
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Наименьшее общее кратное
int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    
    // Используем формулу: LCM(a,b) = |a*b| / GCD(a,b)
    int gcd_value = gcd(a, b);
    return (a / gcd_value) * b;  // Умножаем в таком порядке, чтобы избежать переполнения
}

// Проверка числа на простоту
int is_prime(int n) {
    if (n <= 1) return 0;      // 0 и 1 не простые
    if (n <= 3) return 1;      // 2 и 3 простые
    if (n % 2 == 0 || n % 3 == 0) return 0; // Чётные и кратные 3 не простые
    
    // Проверяем делители вида 6k±1 до √n
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }
    
    return 1;
}