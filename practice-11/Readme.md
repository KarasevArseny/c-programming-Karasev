Практическая работа 11: Работа со сторонними библиотеками
Студент: Карасев Арсений Алексеевич
Группа: 1зб_ИВТ-25
Дата: 22 февраля 2026 г.

Задание 11.1: Работа с JSON (cJSON)
Постановка задачи
Создайте программу config_manager, которая:

Читает конфигурацию из файла config.json

Выводит текущие настройки

Позволяет изменить настройку через командную строку

Сохраняет изменённую конфигурацию

Установка библиотеки

Windows (MSYS2):

bash
pacman -S mingw-w64-ucrt-x86_64-cjson mingw-w64-ucrt-x86_64-pkg-config
Компиляция с pkg-config
bash
gcc config_manager.c $(pkg-config --cflags --libs libcjson) -o config_manager
Вывод pkg-config:

bash
$ pkg-config --cflags --libs libcjson
-I/usr/include/cjson -lcjson

Код программы (config_manager.c)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cjson/cJSON.h>

// Чтение файла в строку
char *read_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *data = (char*)malloc(len + 1);
    if (data) {
        fread(data, 1, len, f);
        data[len] = '\0';
    }
    fclose(f);
    return data;
}

// Запись строки в файл
int write_file(const char *filename, const char *data) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    fputs(data, f);
    fclose(f);
    return 1;
}

// Загрузка конфигурации
cJSON *load_config(const char *filename) {
    char *json_str = read_file(filename);
    if (!json_str) {
        fprintf(stderr, "Ошибка чтения файла: %s\n", filename);
        return NULL;
    }
    
    cJSON *config = cJSON_Parse(json_str);
    free(json_str);
    
    if (!config) {
        const char *error_ptr = cJSON_GetErrorPtr();
        fprintf(stderr, "Ошибка парсинга JSON: %s\n", error_ptr);
    }
    return config;
}

// Сохранение конфигурации
int save_config(const char *filename, cJSON *config) {
    char *json_str = cJSON_Print(config);
    if (!json_str) return 0;
    
    int result = write_file(filename, json_str);
    free(json_str);
    return result;
}

// Вывод конфигурации
void print_config(cJSON *config) {
    cJSON *app_name = cJSON_GetObjectItem(config, "app_name");
    cJSON *version = cJSON_GetObjectItem(config, "version");
    cJSON *settings = cJSON_GetObjectItem(config, "settings");
    cJSON *allowed_hosts = cJSON_GetObjectItem(config, "allowed_hosts");
    
    printf("Конфигурация приложения:\n");
    printf("==========================\n\n");
    
    if (cJSON_IsString(app_name))
        printf("Название: %s\n", app_name->valuestring);
    if (cJSON_IsString(version))
        printf("Версия: %s\n", version->valuestring);
    
    if (cJSON_IsObject(settings)) {
        printf("\nНастройки:\n");
        printf("------------\n");
        
        cJSON *debug = cJSON_GetObjectItem(settings, "debug");
        cJSON *max_connections = cJSON_GetObjectItem(settings, "max_connections");
        cJSON *timeout = cJSON_GetObjectItem(settings, "timeout_seconds");
        cJSON *log_file = cJSON_GetObjectItem(settings, "log_file");
        
        if (cJSON_IsBool(debug))
            printf("debug: %s\n", cJSON_IsTrue(debug) ? "true" : "false");
        if (cJSON_IsNumber(max_connections))
            printf("max_connections: %d\n", max_connections->valueint);
        if (cJSON_IsNumber(timeout))
            printf("timeout_seconds: %d\n", timeout->valueint);
        if (cJSON_IsString(log_file))
            printf("log_file: %s\n", log_file->valuestring);
    }
    
    if (cJSON_IsArray(allowed_hosts)) {
        printf("\nРазрешённые хосты:\n");
        printf("--------------------\n");
        int size = cJSON_GetArraySize(allowed_hosts);
        for (int i = 0; i < size; i++) {
            cJSON *host = cJSON_GetArrayItem(allowed_hosts, i);
            if (cJSON_IsString(host))
                printf("- %s\n", host->valuestring);
        }
    }
}

// Изменение настройки
int set_config_value(cJSON *config, const char *key, const char *value) {
    // Поиск настройки в корне или в секции settings
    cJSON *item = cJSON_GetObjectItem(config, key);
    if (!item) {
        cJSON *settings = cJSON_GetObjectItem(config, "settings");
        if (settings)
            item = cJSON_GetObjectItem(settings, key);
    }
    
    if (!item) {
        fprintf(stderr, "Настройка '%s' не найдена\n", key);
        return 0;
    }
    
    // Определение типа и установка значения
    if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
        int bool_val = (strcmp(value, "true") == 0);
        if (cJSON_IsBool(item)) {
            item->type = bool_val ? cJSON_True : cJSON_False;
        } else {
            cJSON *new_item = cJSON_CreateBool(bool_val);
            cJSON_ReplaceItemInObject(
                cJSON_GetObjectItem(config, "settings") ?: config,
                key, new_item);
        }
    }
    else if (strspn(value, "0123456789-") == strlen(value)) {
        int int_val = atoi(value);
        if (cJSON_IsNumber(item)) {
            item->valueint = int_val;
        } else {
            cJSON *new_item = cJSON_CreateNumber(int_val);
            cJSON_ReplaceItemInObject(
                cJSON_GetObjectItem(config, "settings") ?: config,
                key, new_item);
        }
    }
    else {
        if (cJSON_IsString(item)) {
            free(item->valuestring);
            item->valuestring = strdup(value);
        } else {
            cJSON *new_item = cJSON_CreateString(value);
            cJSON_ReplaceItemInObject(
                cJSON_GetObjectItem(config, "settings") ?: config,
                key, new_item);
        }
    }
    
    return 1;
}
Файл конфигурации (config.json)
json
{
    "app_name": "MyApp",
    "version": "1.0",
    "settings": {
        "debug": false,
        "max_connections": 100,
        "timeout_seconds": 30,
        "log_file": "/var/log/myapp.log"
    },
    "allowed_hosts": ["localhost", "127.0.0.1"]
}

Результаты работы
screenshots/task-11-1.png

Задание 11.2: HTTP-клиент (libcurl)
Постановка задачи
Создайте программу http_client, которая:

Выполняет GET и POST запросы

Поддерживает заголовки

Сохраняет результат в файл (опционально)

Установка библиотеки

Windows (MSYS2):

pacman -S mingw-w64-ucrt-x86_64-curl mingw-w64-ucrt-x86_64-pkg-config
Компиляция с pkg-config
bash
gcc http_client.c $(pkg-config --cflags --libs libcurl) -o http_client
Вывод pkg-config:

bash
$ pkg-config --cflags --libs libcurl
-I/usr/include/x86_64-linux-gnu -lcurl

Код программы (http_client.c)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

typedef struct {
    char *data;
    size_t size;
} Response;

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total = size * nmemb;
    Response *resp = (Response*)userdata;
    
    char *new_data = realloc(resp->data, resp->size + total + 1);
    if (!new_data) return 0;
    
    resp->data = new_data;
    memcpy(resp->data + resp->size, ptr, total);
    resp->size += total;
    resp->data[resp->size] = '\0';
    
    return total;
}

int http_get(const char *url, Response *resp, struct curl_slist *headers) {
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    resp->data = malloc(1);
    resp->data[0] = '\0';
    resp->size = 0;
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    
    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    
    CURLcode res = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "Ошибка: %s\n", curl_easy_strerror(res));
        free(resp->data);
        return 0;
    }
    
    printf("HTTP %ld\n", http_code);
    return 1;
}

int http_post(const char *url, const char *data, Response *resp, 
              struct curl_slist *headers) {
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    resp->data = malloc(1);
    resp->data[0] = '\0';
    resp->size = 0;
    
    struct curl_slist *default_headers = NULL;
    default_headers = curl_slist_append(default_headers, 
                                         "Content-Type: application/json");
    
    if (headers) {
        struct curl_slist *temp = headers;
        while (temp) {
            default_headers = curl_slist_append(default_headers, temp->data);
            temp = temp->next;
        }
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, default_headers);
    
    CURLcode res = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    curl_slist_free_all(default_headers);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "Ошибка: %s\n", curl_easy_strerror(res));
        free(resp->data);
        return 0;
    }
    
    printf("HTTP %ld\n", http_code);
    return 1;
}

int save_to_file(const char *filename, const char *data, size_t size) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    
    return written == size;
}

Результаты работы
screenshots/task-11-2.png

Задание 11.3: База данных (SQLite)
Постановка задачи
Создайте программу todo для управления списком задач с хранением в SQLite.

Установка библиотеки

Windows (MSYS2):

bash
pacman -S mingw-w64-ucrt-x86_64-sqlite3 mingw-w64-ucrt-x86_64-pkg-config
Компиляция с pkg-config
bash
gcc todo.c $(pkg-config --cflags --libs sqlite3) -o todo
Вывод pkg-config:

bash
$ pkg-config --cflags --libs sqlite3
-I/usr/include -lsqlite3

Код программы (todo.c)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define DB_FILE "todo.db"

int init_db(sqlite3 *db) {
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS tasks ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT, "
        " title TEXT NOT NULL, "
        " done INTEGER DEFAULT 0, "
        " created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    char *err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    return 1;
}

int add_task(sqlite3 *db, const char *title) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO tasks (title) VALUES (?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Ошибка: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Ошибка добавления: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    printf("✅ Задача добавлена (ID: %lld)\n", sqlite3_last_insert_rowid(db));
    return 1;
}

int list_tasks(sqlite3 *db, int show_all) {
    sqlite3_stmt *stmt;
    const char *sql = show_all 
        ? "SELECT id, title, done, created_at FROM tasks ORDER BY done, id;"
        : "SELECT id, title, done, created_at FROM tasks WHERE done=0 ORDER BY id;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Ошибка: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    printf("\n%-4s %-3s %-30s %s\n", "ID", "✓", "Задача", "Создано");
    printf("---- --- ------------------------------ -------------------------\n");
    
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *title = sqlite3_column_text(stmt, 1);
        int done = sqlite3_column_int(stmt, 2);
        const unsigned char *created = sqlite3_column_text(stmt, 3);
        
        printf("%-4d [%c] %-30s %s\n", 
               id, done ? 'x' : ' ', title, created);
        count++;
    }
    
    sqlite3_finalize(stmt);
    printf("\n📊 Всего задач: %d\n", count);
    return 1;
}

int complete_task(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE tasks SET done = 1 WHERE id = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE && sqlite3_changes(db) > 0) {
        printf("✅ Задача %d отмечена выполненной\n", id);
        return 1;
    }
    
    fprintf(stderr, "❌ Задача %d не найдена\n", id);
    return 0;
}

Результаты работы
screenshots/task-11-3.png

Задание 11.4: Комбинированный проект (Weather App)
Постановка задачи
Создайте программу weather, которая объединяет все три библиотеки:

Делает HTTP-запрос к API погоды (https://wttr.in/Moscow?format=j1)

Парсит JSON-ответ

Сохраняет данные в SQLite

Выводит историю запросов

Структура проекта

weather/
├── include/
│   ├── weather.h
│   ├── http.h
│   └── db.h
├── src/
│   ├── weather.c
│   ├── http.c
│   └── db.c
├── Makefile
└── README.md
Заголовочные файлы

weather.h:

#ifndef WEATHER_H
#define WEATHER_H

#include <time.h>

typedef struct {
    int id;
    char city[100];
    char country[10];
    double temperature;
    double feels_like;
    int humidity;
    double wind_speed;
    char condition[50];
    char description[200];
    time_t timestamp;
} WeatherData;

void print_usage(const char *prog_name);
int parse_city_name(int argc, char *argv[], char *city, size_t city_size);

#endif // WEATHER_H

http.h:

#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

typedef struct {
    char *data;
    size_t size;
} HttpResponse;

void http_global_init(void);
void http_global_cleanup(void);
int http_get(const char *url, HttpResponse *resp);
void http_response_free(HttpResponse *resp);
void http_build_weather_url(const char *city, char *url, size_t url_size);

#endif // HTTP_H

db.h:

#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include "weather.h"

#define DB_FILE "weather.db"

int db_init(sqlite3 **db);
int db_save_weather(sqlite3 *db, const WeatherData *weather);
int db_get_history(sqlite3 *db, WeatherData *results, int max_results, int *count);
int db_get_last(sqlite3 *db, WeatherData *result);
int db_cleanup_old(sqlite3 *db, int days);

#endif // DB_H

Компиляция с pkg-config
bash
gcc -Iinclude weather.c http.c db.c $(pkg-config --cflags --libs libcurl libcjson sqlite3) -o weather
Вывод pkg-config:

bash
$ pkg-config --cflags --libs libcurl libcjson sqlite3
-I/usr/include/x86_64-linux-gnu -I/usr/include/cjson -I/usr/include -lcurl -lcjson -lsqlite3

Результаты работы
screenshots/task-11-4.png