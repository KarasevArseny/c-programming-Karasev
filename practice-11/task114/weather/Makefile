CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = 

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/weather

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Получаем флаги для библиотек через pkg-config
CJSON_CFLAGS = $(shell pkg-config --cflags libcjson 2>/dev/null)
CJSON_LIBS = $(shell pkg-config --libs libcjson 2>/dev/null)
CURL_CFLAGS = $(shell pkg-config --cflags libcurl 2>/dev/null)
CURL_LIBS = $(shell pkg-config --libs libcurl 2>/dev/null)
SQLITE_CFLAGS = $(shell pkg-config --cflags sqlite3 2>/dev/null)
SQLITE_LIBS = $(shell pkg-config --libs sqlite3 2>/dev/null)

# Fallback, если pkg-config не настроен
ifeq ($(CJSON_LIBS),)
    CJSON_LIBS = -lcjson
endif
ifeq ($(CURL_LIBS),)
    CURL_LIBS = -lcurl
endif
ifeq ($(SQLITE_LIBS),)
    SQLITE_LIBS = -lsqlite3
endif

# Объединяем все флаги
ALL_CFLAGS = $(CFLAGS) $(CJSON_CFLAGS) $(CURL_CFLAGS) $(SQLITE_CFLAGS)
ALL_LIBS = $(CJSON_LIBS) $(CURL_LIBS) $(SQLITE_LIBS) $(LDFLAGS)

.PHONY: all clean run test examples info install-deps-linux install-deps-windows reset-db dirs

all: dirs $(TARGET)

dirs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(ALL_LIBS) -o $@
	@echo "✅ Сборка завершена: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(ALL_CFLAGS) -c $< -o $@

# Проверка pkg-config и библиотек
check-pkg:
	@echo "=== Проверка pkg-config ==="
	@if command -v pkg-config >/dev/null 2>&1; then \
		echo "✅ pkg-config установлен"; \
		echo ""; \
		echo "cJSON:"; \
		pkg-config --cflags --libs libcjson || echo "❌ cJSON не найден"; \
		echo ""; \
		echo "libcurl:"; \
		pkg-config --cflags --libs libcurl || echo "❌ libcurl не найден"; \
		echo ""; \
		echo "SQLite:"; \
		pkg-config --cflags --libs sqlite3 || echo "❌ SQLite не найден"; \
	else \
		echo "❌ pkg-config не установлен"; \
	fi

# Информация о проекте
info: check-pkg
	@echo ""
	@echo "=== Информация о проекте ==="
	@echo "Компилятор: $(CC)"
	@echo "Флаги компиляции: $(ALL_CFLAGS)"
	@echo "Библиотеки: $(ALL_LIBS)"
	@echo ""
	@echo "Доступные цели:"
	@echo "  all          - сборка программы"
	@echo "  run          - запуск программы"
	@echo "  test         - тестирование"
	@echo "  examples     - запуск примеров"
	@echo "  clean        - очистка"
	@echo "  reset-db     - удалить базу данных"
	@echo "  install-deps-linux - установка зависимостей для Linux"
	@echo "  install-deps-windows - установка зависимостей для Windows"

# Установка зависимостей для Linux
install-deps-linux:
	@echo "Установка зависимостей для Linux..."
	@sudo apt update
	@sudo apt install -y libcjson-dev libcurl4-openssl-dev libsqlite3-dev pkg-config

# Установка зависимостей для Windows (MSYS2)
install-deps-windows:
	@echo "Установка зависимостей для Windows (MSYS2)..."
	@pacman -S --noconfirm mingw-w64-ucrt-x86_64-cjson mingw-w64-ucrt-x86_64-curl mingw-w64-ucrt-x86_64-sqlite3 mingw-w64-ucrt-x86_64-pkg-config

# Запуск программы
run: $(TARGET)
	@echo "=== Запуск программы ==="
	@./$(TARGET)

# Тестирование
test: $(TARGET)
	@echo "=== Тестирование программы ==="
	@echo ""
	@echo "1. Получение погоды для Москвы"
	@./$(TARGET) fetch Moscow
	@echo ""
	@echo "2. Показать историю"
	@./$(TARGET) history
	@echo ""
	@echo "3. Показать последний запрос"
	@./$(TARGET) last

# Примеры использования
examples: $(TARGET)
	@echo "=== Примеры использования ==="
	@echo ""
	@echo "1. Получение погоды для разных городов:"
	@./$(TARGET) fetch Moscow
	@./$(TARGET) fetch "Saint Petersburg"
	@./$(TARGET) fetch London
	@echo ""
	@echo "2. История запросов:"
	@./$(TARGET) history
	@echo ""
	@echo "3. Последний запрос:"
	@./$(TARGET) last
	@echo ""
	@echo "4. Очистка старых записей:"
	@./$(TARGET) cleanup 7

# Очистка файлов сборки
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "🧹 Очистка файлов сборки завершена"

# Сброс базы данных
reset-db:
	rm -f $(DB_FILE) weather.db
	@echo "🗑️ База данных удалена"

# Полная очистка
distclean: clean reset-db

# Справка
help: info