#include "mathlib.h"
#include <stdio.h>

double ml_abs(double x) {
    return x < 0 ? -x : x;
}

double ml_pow(double base, int exp) {
    if (exp == 0) return 1.0;
    
    double result = 1.0;
    int positive_exp = exp > 0 ? exp : -exp;
    
    for (int i = 0; i < positive_exp; i++) {
        result *= base;
    }
    
    return exp > 0 ? result : 1.0 / result;
}

double ml_sqrt(double x) {
    if (x < 0) {
        printf("Ошибка: sqrt от отрицательного числа %.2f\n", x);
        return 0;
    }
    if (x == 0) return 0;
    
    // Метод Ньютона
    double guess = x / 2.0;
    double prev;
    
    do {
        prev = guess;
        guess = (guess + x / guess) / 2.0;
    } while (ml_abs(guess - prev) > 1e-10);
    
    return guess;
}

double ml_factorial(int n) {
    if (n < 0) {
        printf("Ошибка: факториал отрицательного числа %d\n", n);
        return 0;
    }
    
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    
    return result;
}

int ml_gcd(int a, int b) {
    a = (int)ml_abs(a);
    b = (int)ml_abs(b);
    
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    
    return a;
}

int ml_lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    return (int)(ml_abs(a) * ml_abs(b)) / ml_gcd(a, b);
}

double ml_sin(double x) {
    // Ряд Тейлора: sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
    // Приводим x к диапазону [-π, π]
    while (x > MATHLIB_PI) x -= 2 * MATHLIB_PI;
    while (x < -MATHLIB_PI) x += 2 * MATHLIB_PI;
    
    double result = 0.0;
    double term = x;
    int sign = 1;
    
    for (int i = 1; i <= 15; i += 2) {
        result += sign * term;
        sign = -sign;
        term *= x * x / ((i + 1) * (i + 2));
    }
    
    return result;
}

double ml_cos(double x) {
    // Ряд Тейлора: cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + ...
    while (x > MATHLIB_PI) x -= 2 * MATHLIB_PI;
    while (x < -MATHLIB_PI) x += 2 * MATHLIB_PI;
    
    double result = 0.0;
    double term = 1.0;
    int sign = 1;
    
    for (int i = 0; i <= 15; i += 2) {
        result += sign * term;
        sign = -sign;
        term *= x * x / ((i + 1) * (i + 2));
    }
    
    return result;
}

double ml_tan(double x) {
    double cosx = ml_cos(x);
    if (ml_abs(cosx) < 1e-10) {
        printf("Ошибка: tan(π/2 + πk) не определён\n");
        return 0;
    }
    return ml_sin(x) / cosx;
}

double ml_deg_to_rad(double degrees) {
    return degrees * MATHLIB_PI / 180.0;
}

double ml_rad_to_deg(double radians) {
    return radians * 180.0 / MATHLIB_PI;
}