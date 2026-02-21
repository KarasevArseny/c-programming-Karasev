# config_manager - Менеджер конфигурации JSON

Программа для работы с конфигурационным файлом в формате JSON с использованием библиотеки cJSON.

## Возможности

- Чтение конфигурации из файла `config.json`
- Отображение текущих настроек
- Изменение настроек через командную строку
- Автоматическое сохранение изменений
- Поддержка различных типов данных (boolean, integer, string)

## Требования

- Компилятор C (gcc/clang)
- Библиотека cJSON
- pkg-config (опционально, для автоматического определения флагов)

## Установка зависимостей

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install libcjson-dev pkg-config

### Windows (MSYS2)

pacman -S mingw-w64-ucrt-x86_64-cjson mingw-w64-ucrt-x86_64-pkg-config

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

config_manager/
├── config_manager.c   # Исходный код
├── config.json        # Файл конфигурации
├── Makefile           # Makefile для сборки
└── README.md          # Документация