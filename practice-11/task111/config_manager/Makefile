CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = config_manager
SRC = config_manager.c

# Получаем флаги для cJSON через pkg-config
PKG_CONFIG = pkg-config
CJSON_CFLAGS = $(shell $(PKG_CONFIG) --cflags libcjson 2>/dev/null)
CJSON_LIBS = $(shell $(PKG_CONFIG) --libs libcjson 2>/dev/null)

# Fallback, если pkg-config не настроен
ifeq ($(CJSON_CFLAGS),)
    CJSON_CFLAGS = -I/usr/include/cjson
    CJSON_LIBS = -lcjson
endif

.PHONY: all clean install uninstall check-pkg help

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(CJSON_CFLAGS) $< $(CJSON_LIBS) -o $@
	@echo "✅ Сборка завершена: $(TARGET)"

# Проверка pkg-config
check-pkg:
	@echo "=== Проверка pkg-config ==="
	@if command -v pkg-config >/dev/null 2>&1; then \
		echo "✅ pkg-config установлен"; \
		echo "Флаги cJSON:"; \
		pkg-config --cflags --libs libcjson || echo "❌ libcjson не найден"; \
	else \
		echo "❌ pkg-config не установлен"; \
	fi

# Запуск программы
run: $(TARGET)
	@echo "=== Текущая конфигурация ==="
	@./$(TARGET)

# Установка зависимостей для Linux
install-deps-linux:
	@echo "Установка зависимостей для Linux..."
	@sudo apt update
	@sudo apt install -y libcjson-dev pkg-config

# Установка зависимостей для Windows (MSYS2)
install-deps-windows:
	@echo "Установка зависимостей для Windows (MSYS2)..."
	@pacman -S --noconfirm mingw-w64-ucrt-x86_64-cjson mingw-w64-ucrt-x86_64-pkg-config

# Примеры использования
examples: $(TARGET)
	@echo "=== Примеры использования ==="
	@echo ""
	@echo "1. Показать конфигурацию:"
	@./$(TARGET)
	@echo ""
	@echo "2. Изменить debug на true:"
	@./$(TARGET) set debug true
	@./$(TARGET)
	@echo ""
	@echo "3. Изменить max_connections на 200:"
	@./$(TARGET) set max_connections 200
	@./$(TARGET)
	@echo ""
	@echo "4. Изменить log_file:"
	@./$(TARGET) set log_file /tmp/app.log
	@./$(TARGET)

# Тестирование
test: $(TARGET)
	@echo "=== Тестирование программы ==="
	@echo ""
	@echo "1. Проверка показа конфигурации"
	@./$(TARGET) > /tmp/test_output.txt
	@if grep -q "debug: false" /tmp/test_output.txt; then \
		echo "✅ Показана конфигурация"; \
	else \
		echo "❌ Ошибка показа конфигурации"; \
	fi
	@echo ""
	@echo "2. Проверка изменения настройки"
	@./$(TARGET) set debug true
	@./$(TARGET) | grep -q "debug: true" && echo "✅ Настройка изменена" || echo "❌ Ошибка изменения"
	@./$(TARGET) set debug false
	@echo ""
	@echo "3. Проверка сохранения в файл"
	@./$(TARGET) set max_connections 150
	@./$(TARGET) | grep -q "max_connections: 150" && echo "✅ Изменение сохранено" || echo "❌ Ошибка сохранения"
	@./$(TARGET) set max_connections 100

# Очистка
clean:
	rm -f $(TARGET) *.o
	@echo "🧹 Очистка завершена"

# Информация
info:
	@echo "=== Информация о проекте ==="
	@echo "Компилятор: $(CC)"
	@echo "Флаги компиляции: $(CFLAGS) $(CJSON_CFLAGS)"
	@echo "Библиотеки: $(CJSON_LIBS)"
	@echo ""
	@echo "Доступные цели:"
	@echo "  all          - сборка программы"
	@echo "  run          - запуск программы"
	@echo "  check-pkg    - проверка pkg-config"
	@echo "  install-deps-linux - установка зависимостей для Linux"
	@echo "  install-deps-windows - установка зависимостей для Windows"
	@echo "  examples     - запуск примеров"
	@echo "  test         - тестирование"
	@echo "  clean        - очистка"
	@echo "  info         - информация о проекте"

help: info

# Восстановление исходного config.json
reset-config:
	@echo '{"app_name":"MyApp","version":"1.0","settings":{"debug":false,"max_connections":100,"timeout_seconds":30,"log_file":"/var/log/myapp.log"},"allowed_hosts":["localhost","127.0.0.1"]}' > config.json
	@echo "✅ Конфигурация сброшена"