# Weather App - Прогноз погоды

Комбинированный проект, объединяющий три библиотеки:
- **libcurl** - для HTTP-запросов к API погоды
- **cJSON** - для парсинга JSON-ответов
- **SQLite** - для хранения истории запросов

## Возможности

- Получение погоды для любого города мира
- Автоматическое сохранение в базу данных SQLite
- Просмотр истории запросов
- Просмотр последнего запроса
- Очистка старых записей
- Детальная информация: температура, влажность, ветер, описание

## Требования

- Компилятор C (gcc/clang)
- Библиотеки: libcurl, cJSON, sqlite3
- pkg-config (опционально)

## Установка зависимостей

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install libcjson-dev libcurl4-openssl-dev libsqlite3-dev pkg-config

Windows (MSYS2)

pacman -S mingw-w64-ucrt-x86_64-cjson mingw-w64-ucrt-x86_64-curl mingw-w64-ucrt-x86_64-sqlite3 mingw-w64-ucrt-x86_64-pkg-config

Сборка

# Сборка программы
make

# Проверка pkg-config и библиотек
make check-pkg

# Информация о проекте
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