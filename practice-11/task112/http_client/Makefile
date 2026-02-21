CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = http_client
SRC = http_client.c

# Получаем флаги для libcurl через pkg-config
CURL_CFLAGS = $(shell pkg-config --cflags libcurl 2>/dev/null)
CURL_LIBS = $(shell pkg-config --libs libcurl 2>/dev/null)

# Fallback, если pkg-config не настроен
ifeq ($(CURL_LIBS),)
    CURL_LIBS = -lcurl
endif

.PHONY: all clean run test examples info install-deps-linux install-deps-windows

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(CURL_CFLAGS) $< $(CURL_LIBS) -o $@
	@echo "✅ Сборка завершена: $(TARGET)"

# Проверка pkg-config и libcurl
check-pkg:
	@echo "=== Проверка pkg-config ==="
	@if command -v pkg-config >/dev/null 2>&1; then \
		echo "✅ pkg-config установлен"; \
		echo "Флаги libcurl:"; \
		pkg-config --cflags --libs libcurl || echo "❌ libcurl не найден"; \
	else \
		echo "❌ pkg-config не установлен"; \
	fi

# Информация о проекте
info: check-pkg
	@echo ""
	@echo "=== Информация о проекте ==="
	@echo "Компилятор: $(CC)"
	@echo "Флаги компиляции: $(CFLAGS) $(CURL_CFLAGS)"
	@echo "Библиотеки: $(CURL_LIBS)"
	@echo ""
	@echo "Доступные цели:"
	@echo "  all          - сборка программы"
	@echo "  run          - запуск программы"
	@echo "  test         - тестирование"
	@echo "  examples     - запуск примеров"
	@echo "  clean        - очистка"
	@echo "  install-deps-linux - установка зависимостей для Linux"
	@echo "  install-deps-windows - установка зависимостей для Windows"

# Установка зависимостей для Linux
install-deps-linux:
	@echo "Установка зависимостей для Linux..."
	@sudo apt update
	@sudo apt install -y libcurl4-openssl-dev pkg-config

# Установка зависимостей для Windows (MSYS2)
install-deps-windows:
	@echo "Установка зависимостей для Windows (MSYS2)..."
	@pacman -S --noconfirm mingw-w64-ucrt-x86_64-curl mingw-w64-ucrt-x86_64-pkg-config

# Запуск программы
run: $(TARGET)
	@echo "=== Запуск программы ==="
	@./$(TARGET)

# Тестирование
test: $(TARGET)
	@echo "=== Тестирование HTTP-клиента ==="
	@echo ""
	@echo "1. GET запрос к httpbin.org"
	@./$(TARGET) get https://httpbin.org/get > /tmp/test_output.txt
	@if grep -q "HTTP 200" /tmp/test_output.txt; then \
		echo "✅ GET запрос выполнен успешно"; \
	else \
		echo "❌ Ошибка GET запроса"; \
	fi
	@echo ""
	@echo "2. GET запрос с заголовком"
	@./$(TARGET) get https://httpbin.org/headers -H "User-Agent: TestClient/1.0" > /tmp/test_headers.txt
	@if grep -q "HTTP 200" /tmp/test_headers.txt; then \
		echo "✅ GET с заголовком выполнен"; \
	else \
		echo "❌ Ошибка GET с заголовком"; \
	fi
	@echo ""
	@echo "3. GET запрос с сохранением в файл"
	@./$(TARGET) get https://httpbin.org/json -o /tmp/response.json
	@if [ -f /tmp/response.json ]; then \
		echo "✅ Файл сохранен: /tmp/response.json"; \
	else \
		echo "❌ Ошибка сохранения файла"; \
	fi
	@echo ""
	@echo "4. POST запрос с JSON"
	@./$(TARGET) post https://httpbin.org/post '{"test":"data"}' > /tmp/post_output.txt
	@if grep -q "HTTP 200" /tmp/post_output.txt; then \
		echo "✅ POST запрос выполнен успешно"; \
	else \
		echo "❌ Ошибка POST запроса"; \
	fi
	@rm -f /tmp/test_output.txt /tmp/test_headers.txt /tmp/response.json /tmp/post_output.txt

# Примеры использования
examples: $(TARGET)
	@echo "=== Примеры использования ==="
	@echo ""
	@echo "1. Простой GET запрос:"
	@./$(TARGET) get https://httpbin.org/get
	@echo ""
	@echo "2. GET запрос с заголовком:"
	@./$(TARGET) get https://httpbin.org/headers -H "User-Agent: MyClient/1.0"
	@echo ""
	@echo "3. GET запрос с сохранением в файл:"
	@./$(TARGET) get https://httpbin.org/json -o response.json
	@ls -la response.json
	@echo ""
	@echo "4. POST запрос с JSON:"
	@./$(TARGET) post https://httpbin.org/post '{"name":"Alice","age":25}'
	@rm -f response.json

# Очистка
clean:
	rm -f $(TARGET) *.o response.json
	@echo "🧹 Очистка завершена"

# Справка
help: info