Практическая работа 10: Создание библиотек
Студент: Карасев Арсений Алексеевич
Группа: 1зб_ИВТ-25
Дата: 22 февраля 2026 г.

Задание 10.1: Библиотека математических функций (MathUtils)
Постановка задачи
Создать библиотеку mathutils с набором математических функций:

factorial(n) — факториал числа

fibonacci(n) — n-е число Фибоначчи

gcd(a, b) — НОД двух чисел

lcm(a, b) — НОК двух чисел

is_prime(n) — проверка на простое число

Требования
Создать статическую библиотеку libmathutils.a

Создать динамическую библиотеку libmathutils.so (Linux) или mathutils.dll (Windows)

Написать Makefile с целями: static, shared, clean

Заголовочный файл mathutils.h

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

MATHUTILS_API int factorial(int n);
MATHUTILS_API int fibonacci(int n);
MATHUTILS_API int gcd(int a, int b);
MATHUTILS_API int lcm(int a, int b);
MATHUTILS_API int is_prime(int n);

#endif // MATHUTILS_H

Реализация mathutils.c

#include "mathutils.h"
#include <stddef.h>

int factorial(int n) {
    if (n < 0) return -1;
    
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int fibonacci(int n) {
    if (n < 0) return -1;
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

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int gcd_value = gcd(a, b);
    return (a / gcd_value) * b;
}

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }
    return 1;
}

Тестовая программа main.c

#include <stdio.h>
#include "mathutils.h"

int main(void) {
    printf("=== ТЕСТИРОВАНИЕ БИБЛИОТЕКИ MATHUTILS ===\n\n");
    
    printf("factorial(5) = %d\n", factorial(5));
    printf("fibonacci(10) = %d\n", fibonacci(10));
    printf("gcd(48, 18) = %d\n", gcd(48, 18));
    printf("lcm(4, 6) = %d\n", lcm(4, 6));
    printf("is_prime(17) = %d\n", is_prime(17));
    printf("is_prime(18) = %d\n", is_prime(18));
    
    return 0;
}
Результаты работы
screenshots/task-10-1.png

Задание 10.2: Библиотека для работы со строками (StrUtils)
Постановка задачи
Создать библиотеку strutils с функциями обработки строк:

str_reverse(s) — реверс строки (новая память)

str_to_upper(s) — в верхний регистр (новая память)

str_to_lower(s) — в нижний регистр (новая память)

str_trim(s) — удаление пробелов по краям (новая память)

str_word_count(s) — количество слов

Важно: Функции, возвращающие char*, выделяют память через malloc. Вызывающий код должен освободить её через free.

Заголовочный файл strutils.h

#ifndef STRUTILS_H
#define STRUTILS_H

#ifdef _WIN32
    #ifdef STRUTILS_EXPORTS
        #define STRUTILS_API __declspec(dllexport)
    #else
        #define STRUTILS_API __declspec(dllimport)
    #endif
#else
    #define STRUTILS_API
#endif

STRUTILS_API char *str_reverse(const char *s);
STRUTILS_API char *str_to_upper(const char *s);
STRUTILS_API char *str_to_lower(const char *s);
STRUTILS_API char *str_trim(const char *s);
STRUTILS_API int str_word_count(const char *s);

#endif // STRUTILS_H

Реализация strutils.c

#include "strutils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *allocate_string_copy(const char *s, size_t len) {
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    if (s) {
        memcpy(result, s, len);
        result[len] = '\0';
    } else {
        result[0] = '\0';
    }
    return result;
}

char *str_reverse(const char *s) {
    if (!s) return NULL;
    
    size_t len = strlen(s);
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = s[len - 1 - i];
    }
    result[len] = '\0';
    return result;
}

char *str_to_upper(const char *s) {
    if (!s) return NULL;
    
    size_t len = strlen(s);
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = toupper(s[i]);
    }
    result[len] = '\0';
    return result;
}

char *str_to_lower(const char *s) {
    if (!s) return NULL;
    
    size_t len = strlen(s);
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(s[i]);
    }
    result[len] = '\0';
    return result;
}

int str_word_count(const char *s) {
    if (!s) return 0;
    
    int count = 0;
    int in_word = 0;
    
    while (*s) {
        if (isspace(*s)) {
            in_word = 0;
        } else {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        }
        s++;
    }
    return count;
}

char *str_trim(const char *s) {
    if (!s) return NULL;
    
    const char *start = s;
    while (*start && isspace(*start)) {
        start++;
    }
    
    if (*start == '\0') {
        return allocate_string_copy("", 0);
    }
    
    const char *end = s + strlen(s) - 1;
    while (end > start && isspace(*end)) {
        end--;
    }
    
    size_t len = end - start + 1;
    return allocate_string_copy(start, len);
}

Тестовая программа main.c

#include <stdio.h>
#include <stdlib.h>
#include "strutils.h"

int main(void) {
    const char *test = "  Hello   World  ";
    
    printf("=== ТЕСТИРОВАНИЕ БИБЛИОТЕКИ STRUTILS ===\n\n");
    printf("Original: \"%s\"\n", test);
    
    char *rev = str_reverse(test);
    char *upper = str_to_upper(test);
    char *lower = str_to_lower(test);
    char *trimmed = str_trim(test);
    int words = str_word_count(test);
    
    printf("Reversed: \"%s\"\n", rev);
    printf("Upper:    \"%s\"\n", upper);
    printf("Lower:    \"%s\"\n", lower);
    printf("Trimmed:  \"%s\"\n", trimmed);
    printf("Words:    %d\n", words);
    
    free(rev);
    free(upper);
    free(lower);
    free(trimmed);
    
    return 0;
}

Результаты работы
screenshots/task-10-2.png

Задание 10.3: Makefile для кроссплатформенной сборки
Постановка задачи
Написать универсальный Makefile, который автоматически определяет ОС и собирает статическую и динамическую версии библиотек для Linux и Windows (MSYS2).

Универсальный Makefile
makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC
AR = ar
ARFLAGS = rcs

# Определение операционной системы
ifeq ($(OS),Windows_NT)
    # Windows
    STATIC_LIB_MATH = libmathutils.a
    STATIC_LIB_STR = libstrutils.a
    SHARED_LIB_MATH = mathutils.dll
    SHARED_LIB_STR = strutils.dll
    IMPORT_LIB_MATH = libmathutils.dll.a
    IMPORT_LIB_STR = libstrutils.dll.a
    EXE = program.exe
    EXE_STATIC = program_static.exe
    EXE_SHARED = program_shared.exe
    SHARED_FLAGS = -shared -Wl,--out-implib,
    EXPORT_DEF_MATH = -DMATHUTILS_EXPORTS
    EXPORT_DEF_STR = -DSTRUTILS_EXPORTS
    RM = del /Q 2>NUL || true
    MKDIR = mkdir 2>NUL || true
    SEP = \\
    EXE_EXT = .exe
else
    # Linux
    STATIC_LIB_MATH = libmathutils.a
    STATIC_LIB_STR = libstrutils.a
    SHARED_LIB_MATH = libmathutils.so
    SHARED_LIB_STR = libstrutils.so
    IMPORT_LIB_MATH = 
    IMPORT_LIB_STR = 
    EXE = program
    EXE_STATIC = program_static
    EXE_SHARED = program_shared
    SHARED_FLAGS = -shared
    EXPORT_DEF_MATH = 
    EXPORT_DEF_STR = 
    RM = rm -f
    MKDIR = mkdir -p
    SEP = /
    EXE_EXT = 
endif

.PHONY: all static shared clean info test

all: static shared

# Статические библиотеки
static: $(STATIC_LIB_MATH) $(STATIC_LIB_STR) $(EXE_STATIC)

mathutils_static.o: mathutils.c mathutils.h
	$(CC) $(CFLAGS) -c $< -o $@

strutils_static.o: strutils.c strutils.h
	$(CC) $(CFLAGS) -c $< -o $@

$(STATIC_LIB_MATH): mathutils_static.o
	$(AR) $(ARFLAGS) $@ $^

$(STATIC_LIB_STR): strutils_static.o
	$(AR) $(ARFLAGS) $@ $^

$(EXE_STATIC): main.c $(STATIC_LIB_MATH) $(STATIC_LIB_STR)
	$(CC) $(CFLAGS) main.c -L. -lmathutils -lstrutils -o $@

# Динамические библиотеки
shared: $(SHARED_LIB_MATH) $(SHARED_LIB_STR) $(EXE_SHARED)

mathutils_shared.o: mathutils.c mathutils.h
	$(CC) $(CFLAGS) $(EXPORT_DEF_MATH) -c $< -o $@

strutils_shared.o: strutils.c strutils.h
	$(CC) $(CFLAGS) $(EXPORT_DEF_STR) -c $< -o $@

$(SHARED_LIB_MATH): mathutils_shared.o
	$(CC) $(SHARED_FLAGS) $^ -o $@ $(if $(IMPORT_LIB_MATH),-Wl,--out-implib,$(IMPORT_LIB_MATH),)

$(SHARED_LIB_STR): strutils_shared.o
	$(CC) $(SHARED_FLAGS) $^ -o $@ $(if $(IMPORT_LIB_STR),-Wl,--out-implib,$(IMPORT_LIB_STR),)

$(EXE_SHARED): main.c $(SHARED_LIB_MATH) $(SHARED_LIB_STR)
ifeq ($(OS),Windows_NT)
	$(CC) $(CFLAGS) main.c -L. -lmathutils.dll -lstrutils.dll -o $@
else
	$(CC) $(CFLAGS) main.c -L. -lmathutils -lstrutils -Wl,-rpath,. -o $@
endif

# Информация о собранных файлах
info: $(STATIC_LIB_MATH) $(SHARED_LIB_MATH) $(EXE_SHARED)
	@echo ""
	@echo "=== Информация о статической библиотеке ==="
	@ar -t $(STATIC_LIB_MATH) 2>/dev/null || echo "Библиотека не найдена"
	@echo ""
	@echo "Символы в статической библиотеке:"
	@nm $(STATIC_LIB_MATH) 2>/dev/null | grep -E ' factorial|fibonacci|gcd|lcm|is_prime' || echo "nm не доступен"
	@echo ""
	@echo "=== Зависимости динамической версии ==="
	@ldd $(EXE_SHARED) 2>/dev/null || echo "ldd не доступен"

# Очистка
clean:
	$(RM) *.o $(STATIC_LIB_MATH) $(STATIC_LIB_STR) $(SHARED_LIB_MATH) $(SHARED_LIB_STR)
	$(RM) $(EXE_STATIC) $(EXE_SHARED) $(EXE) 2>/dev/null || true

Результаты работы
screenshots/task-10-3.png

Задание 10.4: Версионирование библиотеки (Linux)
Постановка задачи
Создать версионированную shared library с использованием soname:

Создать библиотеку libcalc.so.1.0.0

Создать символические ссылки: libcalc.so.1 → libcalc.so.1.0.0, libcalc.so → libcalc.so.1

Использовать soname при компиляции

Заголовочный файл calc.h

#ifndef CALC_H
#define CALC_H

int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);
int power(int base, int exp);

#endif // CALC_H

Реализация calc.c

#include "calc.h"
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b == 0) {
        fprintf(stderr, "Ошибка: деление на ноль\n");
        return 0;
    }
    return a / b;
}

int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

Тестовая программа main.c

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
    
    return 0;
}
Makefile для версионирования
makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC
AR = ar
ARFLAGS = rcs

# Версии библиотеки
LIB_MAJOR = 1
LIB_MINOR = 0
LIB_PATCH = 0
LIB_VERSION = $(LIB_MAJOR).$(LIB_MINOR).$(LIB_PATCH)
LIB_SONAME = libcalc.so.$(LIB_MAJOR)

# Имена файлов
STATIC_LIB = libcalc.a
SHARED_LIB_REAL = libcalc.so.$(LIB_VERSION)
SHARED_LIB_SONAME = $(LIB_SONAME)
SHARED_LIB_LINK = libcalc.so

EXE_STATIC = program_static
EXE_SHARED = program_shared

# Флаги для создания версионированной библиотеки
SHARED_FLAGS = -shared -Wl,-soname,$(SHARED_LIB_SONAME)

.PHONY: all static shared symlinks clean info check-version

all: static shared symlinks

static: $(STATIC_LIB) $(EXE_STATIC)

$(STATIC_LIB): calc.o
	$(AR) $(ARFLAGS) $@ $^

calc.o: calc.c calc.h
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE_STATIC): main.c $(STATIC_LIB)
	$(CC) $(CFLAGS) main.c -L. -lcalc -o $@

shared: $(SHARED_LIB_REAL) symlinks $(EXE_SHARED)

$(SHARED_LIB_REAL): calc.o
	$(CC) $(SHARED_FLAGS) $^ -o $@

symlinks: $(SHARED_LIB_REAL)
	@ln -sf $(SHARED_LIB_REAL) $(SHARED_LIB_SONAME)
	@ln -sf $(SHARED_LIB_SONAME) $(SHARED_LIB_LINK)

$(EXE_SHARED): main.c $(SHARED_LIB_REAL)
	$(CC) $(CFLAGS) main.c -L. -lcalc -Wl,-rpath,. -o $@

check-version:
	@echo "=== ПРОВЕРКА ВЕРСИОНИРОВАНИЯ ==="
	@ls -la $(SHARED_LIB_LINK)*
	@echo ""
	@readelf -d $(SHARED_LIB_REAL) | grep SONAME
	@echo ""
	@ldd $(EXE_SHARED) | grep libcalc

clean:
	rm -f *.o $(STATIC_LIB) $(SHARED_LIB_REAL) $(SHARED_LIB_SONAME) $(SHARED_LIB_LINK)
	rm -f $(EXE_STATIC) $(EXE_SHARED)
Процесс создания версионированной библиотеки

# Компиляция объектного файла с -fPIC
gcc -c -fPIC calc.c -o calc.o

# Создание библиотеки с указанием soname
gcc -shared -Wl,-soname,libcalc.so.1 calc.o -o libcalc.so.1.0.0

# Создание символических ссылок
ln -s libcalc.so.1.0.0 libcalc.so.1
ln -s libcalc.so.1 libcalc.so

# Компиляция программы
gcc main.c -L. -lcalc -Wl,-rpath,. -o program_shared
Проверка версионирования
bash
$ make check-version

=== ПРОВЕРКА ВЕРСИОНИРОВАНИЯ ===
lrwxrwxrwx 1 user user   21 фев 26 10:00 libcalc.so -> libcalc.so.1
lrwxrwxrwx 1 user user   21 фев 26 10:00 libcalc.so.1 -> libcalc.so.1.0.0
-rwxrwxr-x 1 user user 7896 фев 26 10:00 libcalc.so.1.0.0

readelf -d libcalc.so.1.0.0 | grep SONAME
 0x000000000000000e (SONAME) Library soname: [libcalc.so.1]

ldd program_shared | grep libcalc
        libcalc.so.1 => ./libcalc.so.1.0.0 (0x00007f8a9a5b4000)
Структура директории после сборки
text
project/
├── calc.h
├── calc.c
├── main.c
├── Makefile
├── libcalc.so.1.0.0      # Реальный файл
├── libcalc.so.1 -> libcalc.so.1.0.0
├── libcalc.so -> libcalc.so.1
├── program_static         # Статическая версия
└── program_shared         # Динамическая версия

Результаты работы
screenshots/task-10-4.png