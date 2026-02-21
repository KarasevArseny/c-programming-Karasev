CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC
AR = ar
ARFLAGS = rcs

# Определение операционной системы
ifeq ($(OS),Windows_NT)
    # Windows
    STATIC_LIB = libstrutils.a
    SHARED_LIB = strutils.dll
    IMPORT_LIB = libstrutils.dll.a
    EXE = program.exe
    EXE_STATIC = program_static.exe
    EXE_SHARED = program_shared.exe
    SHARED_FLAGS = -shared -Wl,--out-implib,$(IMPORT_LIB)
    EXPORT_DEF = -DSTRUTILS_EXPORTS
    RM = del /Q 2>NUL || true
    MKDIR = mkdir 2>NUL || true
    SEP = \\
else
    # Linux/Unix
    STATIC_LIB = libstrutils.a
    SHARED_LIB = libstrutils.so
    IMPORT_LIB =
    EXE = program
    EXE_STATIC = program_static
    EXE_SHARED = program_shared
    SHARED_FLAGS = -shared
    EXPORT_DEF =
    RM = rm -f
    MKDIR = mkdir -p
    SEP = /
endif

# Цели по умолчанию
.PHONY: all static shared clean info test

all: static shared

# Статическая библиотека
static: $(STATIC_LIB) $(EXE_STATIC)

$(STATIC_LIB): strutils_static.o
	$(AR) $(ARFLAGS) $@ $^

strutils_static.o: strutils.c strutils.h
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE_STATIC): main.c $(STATIC_LIB)
	$(CC) $(CFLAGS) main.c -L. -lstrutils -o $@

# Динамическая библиотека
shared: $(SHARED_LIB) $(EXE_SHARED)

strutils_shared.o: strutils.c strutils.h
	$(CC) $(CFLAGS) $(EXPORT_DEF) -c $< -o $@

$(SHARED_LIB): strutils_shared.o
	$(CC) $(SHARED_FLAGS) $^ -o $@

$(EXE_SHARED): main.c $(SHARED_LIB)
ifeq ($(OS),Windows_NT)
	$(CC) $(CFLAGS) main.c -L. -lstrutils.dll -o $@
else
	$(CC) $(CFLAGS) main.c -L. -lstrutils -Wl,-rpath,. -o $@
endif

# Очистка
clean:
	$(RM) *.o $(STATIC_LIB) $(SHARED_LIB) $(IMPORT_LIB) $(EXE_STATIC) $(EXE_SHARED) 2>/dev/null || true
	$(RM) $(EXE) 2>/dev/null || true

# Информация о собранных библиотеках
info: $(STATIC_LIB) $(SHARED_LIB)
	@echo "=== Информация о статической библиотеке ==="
	@ar -t $(STATIC_LIB) 2>/dev/null || echo "Статическая библиотека не найдена"
	@echo ""
	@echo "Символы в статической библиотеке:"
	@nm $(STATIC_LIB) 2>/dev/null | grep -E ' str_reverse|str_to_upper|str_to_lower|str_trim|str_word_count' || echo "nm не доступен или библиотека не найдена"
	@echo ""
	@echo "=== Информация о динамической библиотеке ==="
	@ls -la $(SHARED_LIB) 2>/dev/null || echo "Динамическая библиотека не найдена"
	@echo ""
	@echo "=== Информация о исполняемых файлах ==="
	@ls -la $(EXE_STATIC) $(EXE_SHARED) 2>/dev/null || echo "Исполняемые файлы не найдены"
	@echo ""
	@echo "=== Зависимости динамической версии ==="
	@ldd $(EXE_SHARED) 2>/dev/null || echo "ldd не доступен или файл не найден"

# Тестирование утечек памяти (только для Linux)
test: $(EXE_STATIC)
	@echo "=== Проверка утечек памяти ==="
	@if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full ./$(EXE_STATIC); \
	else \
		echo "valgrind не установлен, пропускаем проверку"; \
		./$(EXE_STATIC); \
	fi