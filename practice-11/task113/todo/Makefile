CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = todo
SRC = todo.c

# Получаем флаги для SQLite через pkg-config
SQLITE_CFLAGS = $(shell pkg-config --cflags sqlite3 2>/dev/null)
SQLITE_LIBS = $(shell pkg-config --libs sqlite3 2>/dev/null)

# Fallback, если pkg-config не настроен
ifeq ($(SQLITE_LIBS),)
    SQLITE_LIBS = -lsqlite3
endif

.PHONY: all clean run test examples info install-deps-linux install-deps-windows reset-db

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SQLITE_CFLAGS) $< $(SQLITE_LIBS) -o $@
	@echo "✅ Сборка завершена: $(TARGET)"

# Проверка pkg-config и SQLite
check-pkg:
	@echo "=== Проверка pkg-config ==="
	@if command -v pkg-config >/dev/null 2>&1; then \
		echo "✅ pkg-config установлен"; \
		echo "Флаги SQLite:"; \
		pkg-config --cflags --libs sqlite3 || echo "❌ SQLite не найден"; \
	else \
		echo "❌ pkg-config не установлен"; \
	fi

# Информация о проекте
info: check-pkg
	@echo ""
	@echo "=== Информация о проекте ==="
	@echo "Компилятор: $(CC)"
	@echo "Флаги компиляции: $(CFLAGS) $(SQLITE_CFLAGS)"
	@echo "Библиотеки: $(SQLITE_LIBS)"
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
	@sudo apt install -y libsqlite3-dev pkg-config

# Установка зависимостей для Windows (MSYS2)
install-deps-windows:
	@echo "Установка зависимостей для Windows (MSYS2)..."
	@pacman -S --noconfirm mingw-w64-ucrt-x86_64-sqlite3 mingw-w64-ucrt-x86_64-pkg-config

# Запуск программы
run: $(TARGET)
	@echo "=== Запуск программы ==="
	@./$(TARGET)

# Тестирование
test: $(TARGET)
	@echo "=== Тестирование TODO менеджера ==="
	@echo ""
	@echo "1. Очистка базы данных для теста"
	@rm -f $(DB_FILE)
	@echo ""
	@echo "2. Добавление тестовых задач"
	@./$(TARGET) add "Изучить cJSON"
	@./$(TARGET) add "Написать HTTP-клиент"
	@./$(TARGET) add "Сдать практическую работу"
	@echo ""
	@echo "3. Показать все задачи"
	@./$(TARGET) list all
	@echo ""
	@echo "4. Отметить выполненной первую задачу"
	@./$(TARGET) done 1
	@echo ""
	@echo "5. Показать невыполненные задачи"
	@./$(TARGET) list
	@echo ""
	@echo "6. Показать статистику"
	@./$(TARGET) stats
	@echo ""
	@echo "7. Удалить задачу 2"
	@./$(TARGET) delete 2
	@./$(TARGET) list all

# Примеры использования
examples: $(TARGET)
	@echo "=== Примеры использования ==="
	@echo ""
	@echo "1. Добавление задач:"
	@./$(TARGET) add "Написать отчёт"
	@./$(TARGET) add "Подготовить презентацию"
	@./$(TARGET) add "Провести встречу"
	@echo ""
	@echo "2. Просмотр всех задач:"
	@./$(TARGET) list all
	@echo ""
	@echo "3. Отметка выполненной задачи:"
	@./$(TARGET) done 1
	@./$(TARGET) list
	@echo ""
	@echo "4. Просмотр статистики:"
	@./$(TARGET) stats
	@echo ""
	@echo "5. Удаление задачи:"
	@./$(TARGET) delete 2
	@./$(TARGET) list all
	@echo ""
	@echo "6. Очистка выполненных задач:"
	@./$(TARGET) clear
	@./$(TARGET) list all

# Очистка файлов сборки
clean:
	rm -f $(TARGET) *.o
	@echo "🧹 Очистка файлов сборки завершена"

# Сброс базы данных (удаление todo.db)
reset-db:
	rm -f $(DB_FILE)
	@echo "🗑️ База данных удалена"

# Полная очистка
distclean: clean reset-db

# Справка
help: info