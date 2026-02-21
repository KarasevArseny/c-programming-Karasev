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

.PHONY: all static shared clean info install uninstall check

all: static shared symlinks

# Статическая библиотека
static: $(STATIC_LIB) $(EXE_STATIC)

calc_static.o: calc.c calc.h
	$(CC) $(CFLAGS) -c $< -o $@

$(STATIC_LIB): calc_static.o
	$(AR) $(ARFLAGS) $@ $^

$(EXE_STATIC): main.c $(STATIC_LIB)
	$(CC) $(CFLAGS) main.c -L. -lcalc -o $@

# Динамическая библиотека с версионированием
shared: $(SHARED_LIB_REAL) symlinks $(EXE_SHARED)

calc_shared.o: calc.c calc.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SHARED_LIB_REAL): calc_shared.o
	$(CC) $(SHARED_FLAGS) $^ -o $@

# Создание символических ссылок для версионирования
symlinks: $(SHARED_LIB_REAL)
	@echo "Создание символических ссылок..."
	@ln -sf $(SHARED_LIB_REAL) $(SHARED_LIB_SONAME)
	@ln -sf $(SHARED_LIB_SONAME) $(SHARED_LIB_LINK)
	@ls -la $(SHARED_LIB_LINK)*

$(EXE_SHARED): main.c $(SHARED_LIB_REAL)
	$(CC) $(CFLAGS) main.c -L. -lcalc -Wl,-rpath,. -o $@

# Проверка версионирования
check-version: $(SHARED_LIB_REAL)
	@echo "=== ПРОВЕРКА ВЕРСИОНИРОВАНИЯ ==="
	@echo ""
	@echo "Содержимое директории:"
	@ls -la $(SHARED_LIB_LINK)*
	@echo ""
	@echo "Информация о SONAME в библиотеке:"
	@readelf -d $(SHARED_LIB_REAL) | grep SONAME || echo "SONAME не найден"
	@echo ""
	@echo "Зависимости программы:"
	@ldd $(EXE_SHARED) | grep libcalc || echo "Библиотека не найдена в зависимостях"
	@echo ""
	@echo "Проверка ссылок:"
	@readlink -f $(SHARED_LIB_LINK)
	@readlink -f $(SHARED_LIB_SONAME)

# Информация о собранных файлах
info: $(STATIC_LIB) $(SHARED_LIB_REAL) $(EXE_STATIC) $(EXE_SHARED)
	@echo "============================================="
	@echo "   ИНФОРМАЦИЯ О СОБРАННЫХ ФАЙЛАХ"
	@echo "============================================="
	@echo ""
	@echo "--- Статическая библиотека ---"
	@ar -t $(STATIC_LIB) 2>/dev/null || echo "Библиотека не найдена"
	@echo ""
	@echo "Символы в статической библиотеке:"
	@nm $(STATIC_LIB) 2>/dev/null | grep -E ' add|subtract|multiply|divide|power' || echo "nm не доступен"
	@echo ""
	@echo "--- Версионированная динамическая библиотека ---"
	@ls -la $(SHARED_LIB_REAL) $(SHARED_LIB_SONAME) $(SHARED_LIB_LINK) 2>/dev/null || echo "Библиотеки не найдены"
	@echo ""
	@echo "--- Исполняемые файлы ---"
	@ls -la $(EXE_STATIC) $(EXE_SHARED) 2>/dev/null || echo "Файлы не найдены"
	@echo ""
	@echo "--- Размеры файлов ---"
	@if [ -f $(EXE_STATIC) ] && [ -f $(EXE_SHARED) ]; then \
		STATIC_SIZE=$$(stat -c%s $(EXE_STATIC) 2>/dev/null || stat -f%z $(EXE_STATIC) 2>/dev/null); \
		SHARED_SIZE=$$(stat -c%s $(EXE_SHARED) 2>/dev/null || stat -f%z $(EXE_SHARED) 2>/dev/null); \
		echo "Статическая версия: $$STATIC_SIZE байт"; \
		echo "Динамическая версия: $$SHARED_SIZE байт"; \
		echo "Разница: $$((STATIC_SIZE - SHARED_SIZE)) байт"; \
	fi

# Тестирование
test: $(EXE_STATIC)
	@echo "============================================="
	@echo "   ЗАПУСК ТЕСТОВОЙ ПРОГРАММЫ"
	@echo "============================================="
	@echo ""
	@./$(EXE_STATIC)

test-shared: $(EXE_SHARED)
	@echo "============================================="
	@echo "   ЗАПУСК ДИНАМИЧЕСКОЙ ВЕРСИИ"
	@echo "============================================="
	@echo ""
	@LD_LIBRARY_PATH=. ./$(EXE_SHARED)

# Имитация установки в системную директорию
install: $(SHARED_LIB_REAL) $(STATIC_LIB)
	@echo "Имитация установки в /usr/local/lib..."
	@sudo mkdir -p /usr/local/lib
	@sudo cp $(SHARED_LIB_REAL) /usr/local/lib/
	@sudo ln -sf /usr/local/lib/$(SHARED_LIB_REAL) /usr/local/lib/$(SHARED_LIB_SONAME)
	@sudo ln -sf /usr/local/lib/$(SHARED_LIB_SONAME) /usr/local/lib/$(SHARED_LIB_LINK)
	@sudo ldconfig
	@echo "Библиотека установлена. Проверка:"
	@ls -la /usr/local/lib/$(SHARED_LIB_LINK)*

uninstall:
	@echo "Удаление из /usr/local/lib..."
	@sudo rm -f /usr/local/lib/$(SHARED_LIB_REAL)
	@sudo rm -f /usr/local/lib/$(SHARED_LIB_SONAME)
	@sudo rm -f /usr/local/lib/$(SHARED_LIB_LINK)
	@sudo ldconfig
	@echo "Библиотека удалена"

# Очистка
clean:
	rm -f *.o $(STATIC_LIB) $(SHARED_LIB_REAL) $(SHARED_LIB_SONAME) $(SHARED_LIB_LINK)
	rm -f $(EXE_STATIC) $(EXE_SHARED)

# Полная проверка
check: all check-version info test test-shared

# Справка
help:
	@echo "Доступные цели:"
	@echo "  all           - сборка всего (по умолчанию)"
	@echo "  static        - сборка статической библиотеки"
	@echo "  shared        - сборка динамической библиотеки с версионированием"
	@echo "  symlinks      - создание символических ссылок"
	@echo "  check-version - проверка версионирования"
	@echo "  info          - информация о собранных файлах"
	@echo "  test          - запуск статической версии"
	@echo "  test-shared   - запуск динамической версии"
	@echo "  install       - имитация установки в систему"
	@echo "  uninstall     - удаление из системы"
	@echo "  check         - полная проверка"
	@echo "  clean         - очистка"
	@echo "  help          - эта справка"