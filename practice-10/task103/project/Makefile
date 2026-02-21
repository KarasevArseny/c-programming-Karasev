# Кроссплатформенный Makefile для сборки статических и динамических библиотек
# Работает в Linux и Windows (MSYS2, MinGW)

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC
AR = ar
ARFLAGS = rcs

# Определение операционной системы
ifeq ($(OS),Windows_NT)
    # Windows настройки
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
    LIB_PREFIX = 
else
    # Linux настройки
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
    LIB_PREFIX = lib
endif

# Цели по умолчанию
.PHONY: all static shared clean info test

all: static shared

# Статическая библиотека
static: $(STATIC_LIB_MATH) $(STATIC_LIB_STR) $(EXE_STATIC)

# Объектные файлы для статической библиотеки
mathutils_static.o: mathutils.c mathutils.h
	$(CC) $(CFLAGS) -c $< -o $@

strutils_static.o: strutils.c strutils.h
	$(CC) $(CFLAGS) -c $< -o $@

# Создание статических библиотек
$(STATIC_LIB_MATH): mathutils_static.o
	$(AR) $(ARFLAGS) $@ $^

$(STATIC_LIB_STR): strutils_static.o
	$(AR) $(ARFLAGS) $@ $^

# Компоновка со статическими библиотеками
$(EXE_STATIC): main.c $(STATIC_LIB_MATH) $(STATIC_LIB_STR)
	$(CC) $(CFLAGS) main.c -L. -lmathutils -lstrutils -o $@

# Динамическая библиотека
shared: $(SHARED_LIB_MATH) $(SHARED_LIB_STR) $(EXE_SHARED)

# Объектные файлы для динамической библиотеки
mathutils_shared.o: mathutils.c mathutils.h
	$(CC) $(CFLAGS) $(EXPORT_DEF_MATH) -c $< -o $@

strutils_shared.o: strutils.c strutils.h
	$(CC) $(CFLAGS) $(EXPORT_DEF_STR) -c $< -o $@

# Создание динамических библиотек
$(SHARED_LIB_MATH): mathutils_shared.o
	$(CC) $(SHARED_FLAGS) $^ -o $@ $(if $(IMPORT_LIB_MATH),-Wl,--out-implib,$(IMPORT_LIB_MATH),)

$(SHARED_LIB_STR): strutils_shared.o
	$(CC) $(SHARED_FLAGS) $^ -o $@ $(if $(IMPORT_LIB_STR),-Wl,--out-implib,$(IMPORT_LIB_STR),)

# Компоновка с динамическими библиотеками
$(EXE_SHARED): main.c $(SHARED_LIB_MATH) $(SHARED_LIB_STR)
ifeq ($(OS),Windows_NT)
	$(CC) $(CFLAGS) main.c -L. -lmathutils.dll -lstrutils.dll -o $@
else
	$(CC) $(CFLAGS) main.c -L. -lmathutils -lstrutils -Wl,-rpath,. -o $@
endif

# Очистка
clean:
	$(RM) *.o $(STATIC_LIB_MATH) $(STATIC_LIB_STR) $(SHARED_LIB_MATH) $(SHARED_LIB_STR) 2>/dev/null || true
	$(RM) $(IMPORT_LIB_MATH) $(IMPORT_LIB_STR) 2>/dev/null || true
	$(RM) $(EXE_STATIC) $(EXE_SHARED) 2>/dev/null || true
	$(RM) $(EXE) 2>/dev/null || true

# Информация о собранных файлах
info: $(STATIC_LIB_MATH) $(SHARED_LIB_MATH) $(EXE_SHARED)
	@echo ""
	@echo "============================================="
	@echo "   ИНФОРМАЦИЯ О СОБРАННЫХ БИБЛИОТЕКАХ"
	@echo "============================================="
	@echo ""
	@echo "--- Статическая библиотека mathutils ---"
	@ar -t $(STATIC_LIB_MATH) 2>/dev/null || echo "Библиотека не найдена"
	@echo ""
	@echo "Символы в статической библиотеке:"
	@nm $(STATIC_LIB_MATH) 2>/dev/null | grep -E ' factorial|fibonacci|gcd|lcm|is_prime' || echo "nm не доступен"
	@echo ""
	@echo "--- Динамическая библиотека mathutils ---"
	@ls -la $(SHARED_LIB_MATH) 2>/dev/null || echo "Библиотека не найдена"
	@echo ""
	@echo "--- Исполняемые файлы ---"
	@ls -la $(EXE_STATIC) $(EXE_SHARED) 2>/dev/null || echo "Файлы не найдены"
	@echo ""
	@echo "--- Размеры файлов ---"
	@if [ -f $(EXE_STATIC) ] && [ -f $(EXE_SHARED) ]; then \
		echo "Статическая версия: `ls -l $(EXE_STATIC) | awk '{print $$5}'` байт"; \
		echo "Динамическая версия: `ls -l $(EXE_SHARED) | awk '{print $$5}'` байт"; \
		echo "Разница: $$((`ls -l $(EXE_STATIC) | awk '{print $$5}'` - `ls -l $(EXE_SHARED) | awk '{print $$5}'`)) байт"; \
	fi
	@echo ""
	@echo "--- Зависимости динамической версии ---"
	@ldd $(EXE_SHARED) 2>/dev/null || echo "ldd не доступен (возможно Windows)"

# Запуск тестов
test: $(EXE_STATIC)
	@echo "============================================="
	@echo "   ЗАПУСК ТЕСТОВОЙ ПРОГРАММЫ"
	@echo "============================================="
	@echo ""
	@./$(EXE_STATIC)

# Проверка утечек памяти (только Linux с valgrind)
valgrind: $(EXE_STATIC)
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "Проверка утечек памяти..."; \
		valgrind --leak-check=full ./$(EXE_STATIC); \
	else \
		echo "valgrind не установлен, пропускаем"; \
	fi

# Запуск всех проверок
check: all info test

# Справка
help:
	@echo "Доступные цели:"
	@echo "  all        - сборка всего (по умолчанию)"
	@echo "  static     - сборка статических библиотек"
	@echo "  shared     - сборка динамических библиотек"
	@echo "  clean      - очистка"
	@echo "  info       - информация о собранных файлах"
	@echo "  test       - запуск тестовой программы"
	@echo "  valgrind   - проверка утечек памяти (Linux)"
	@echo "  check      - сборка, информация и тесты"
	@echo "  help       - эта справка"