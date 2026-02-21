#ifndef STRUTILS_H
#define STRUTILS_H

// Кроссплатформенное объявление экспорта/импорта
#ifdef _WIN32
    #ifdef STRUTILS_EXPORTS
        #define STRUTILS_API __declspec(dllexport)
    #else
        #define STRUTILS_API __declspec(dllimport)
    #endif
#else
    #define STRUTILS_API
#endif

// Функции, возвращающие новые строки (нужно освободить через free)
STRUTILS_API char *str_reverse(const char *s);
STRUTILS_API char *str_to_upper(const char *s);
STRUTILS_API char *str_to_lower(const char *s);
STRUTILS_API char *str_trim(const char *s);

// Функция, возвращающая значение
STRUTILS_API int str_word_count(const char *s);

#endif // STRUTILS_H