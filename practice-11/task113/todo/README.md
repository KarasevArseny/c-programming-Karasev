# TODO менеджер на C с SQLite

Консольное приложение для управления списком задач с хранением в SQLite базе данных.

## Возможности

- Добавление задач
- Просмотр всех/невыполненных задач
- Отметка задач как выполненных
- Удаление задач
- Очистка выполненных задач
- Статистика по задачам
- Автоматическое создание базы данных

## Требования

- Компилятор C (gcc/clang)
- Библиотека SQLite3
- pkg-config (опционально)

## Установка зависимостей

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install libsqlite3-dev pkg-config

Windows (MSYS2)
pacman -S mingw-w64-ucrt-x86_64-sqlite3 mingw-w64-ucrt-x86_64-pkg-config

Сборка
# Сборка программы
make

# Сборка с информацией о pkg-config
make check-pkg
make info

# Запуск программы
make run

# Тестирование
make test

# Примеры использования
make examples

# Очистка
make clean

# Сброс базы данных
make reset-db

Структура проекта

todo/
├── todo.c          # Исходный код
├── Makefile        # Makefile для сборки
├── README.md       # Документация
└── todo.db         # База данных (создаётся автоматически)