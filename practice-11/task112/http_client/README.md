# HTTP-клиент на C с использованием libcurl

Консольная утилита для выполнения HTTP-запросов с поддержкой GET, POST, заголовков и сохранения ответов в файл.

## Возможности

- GET и POST запросы
- Поддержка пользовательских заголовков (-H)
- Сохранение ответа в файл (-o)
- Автоматическое следование редиректам
- Таймаут 30 секунд
- Вывод HTTP кода ответа

## Требования

- Компилятор C (gcc/clang)
- Библиотека libcurl
- pkg-config (опционально)

## Установка зависимостей

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install libcurl4-openssl-dev pkg-config

Windows (MSYS2)
pacman -S mingw-w64-ucrt-x86_64-curl mingw-w64-ucrt-x86_64-pkg-config

Сборка

# Сборка программы
make

# Сборка с информацией о pkg-config
make check-pkg
make info

# Запуск программы
make run

# Очистка
make clean

Структура проекта

http_client/
├── http_client.c    # Исходный код
├── Makefile         # Makefile для сборки
└── README.md        # Документация