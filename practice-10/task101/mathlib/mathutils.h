#ifndef MATHUTILS_H
#define MATHUTILS_H

// Кроссплатформенное объявление экспорта/импорта
#ifdef _WIN32
    #ifdef MATHUTILS_EXPORTS
        #define MATHUTILS_API __declspec(dllexport)
    #else
        #define MATHUTILS_API __declspec(dllimport)
    #endif
#else
    #define MATHUTILS_API
#endif

// Объявления функций
MATHUTILS_API int factorial(int n);
MATHUTILS_API int fibonacci(int n);
MATHUTILS_API int gcd(int a, int b);
MATHUTILS_API int lcm(int a, int b);
MATHUTILS_API int is_prime(int n);

#endif // MATHUTILS_H