#ifndef MATHLIB_H
#define MATHLIB_H

// Константы
#define MATHLIB_PI 3.14159265358979323846
#define MATHLIB_E 2.71828182845904523536

// Базовые функции
double ml_abs(double x);
double ml_pow(double base, int exp);
double ml_sqrt(double x);
double ml_factorial(int n);
int ml_gcd(int a, int b);
int ml_lcm(int a, int b);

// Тригонометрия (в радианах)
double ml_sin(double x);
double ml_cos(double x);
double ml_tan(double x);

// Преобразования
double ml_deg_to_rad(double degrees);
double ml_rad_to_deg(double radians);

#endif // MATHLIB_H