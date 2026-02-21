#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cjson/cJSON.h>

// Чтение файла в строку
char *read_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        return NULL;
    }
    
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *data = (char*)malloc(len + 1);
    if (data) {
        size_t read_len = fread(data, 1, len, f);
        if (read_len != (size_t)len) {
            free(data);
            fclose(f);
            return NULL;
        }
        data[len] = '\0';
    }
    
    fclose(f);
    return data;
}

// Запись строки в файл
int write_file(const char *filename, const char *data) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    
    size_t len = strlen(data);
    size_t written = fwrite(data, 1, len, f);
    
    fclose(f);
    return written == len;
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
        if (error_ptr != NULL) {
            fprintf(stderr, "Ошибка парсинга JSON: %s\n", error_ptr);
        } else {
            fprintf(stderr, "Ошибка парсинга JSON\n");
        }
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

// Вспомогательная функция для получения значения булева типа
bool get_bool_value(cJSON *item) {
    if (!item) return false;
    if (cJSON_IsBool(item)) {
        return cJSON_IsTrue(item);
    }
    return false;
}

// Вывод конфигурации
void print_config(cJSON *config) {
    if (!config) return;
    
    // Получаем основные поля
    cJSON *app_name = cJSON_GetObjectItem(config, "app_name");
    cJSON *version = cJSON_GetObjectItem(config, "version");
    cJSON *settings = cJSON_GetObjectItem(config, "settings");
    cJSON *allowed_hosts = cJSON_GetObjectItem(config, "allowed_hosts");
    
    printf("Конфигурация приложения:\n");
    printf("==========================\n\n");
    
    // Вывод основных настроек
    if (cJSON_IsString(app_name)) {
        printf("Название: %s\n", app_name->valuestring);
    }
    
    if (cJSON_IsString(version)) {
        printf("Версия: %s\n", version->valuestring);
    }
    
    // Вывод настроек из секции settings
    if (cJSON_IsObject(settings)) {
        printf("\nНастройки:\n");
        printf("------------\n");
        
        cJSON *debug = cJSON_GetObjectItem(settings, "debug");
        cJSON *max_connections = cJSON_GetObjectItem(settings, "max_connections");
        cJSON *timeout_seconds = cJSON_GetObjectItem(settings, "timeout_seconds");
        cJSON *log_file = cJSON_GetObjectItem(settings, "log_file");
        
        if (cJSON_IsBool(debug)) {
            printf("debug: %s\n", cJSON_IsTrue(debug) ? "true" : "false");
        }
        
        if (cJSON_IsNumber(max_connections)) {
            printf("max_connections: %d\n", max_connections->valueint);
        }
        
        if (cJSON_IsNumber(timeout_seconds)) {
            printf("timeout_seconds: %d\n", timeout_seconds->valueint);
        }
        
        if (cJSON_IsString(log_file)) {
            printf("log_file: %s\n", log_file->valuestring);
        }
    }
    
    // Вывод списка разрешённых хостов
    if (cJSON_IsArray(allowed_hosts)) {
        printf("\nРазрешённые хосты:\n");
        printf("--------------------\n");
        
        int size = cJSON_GetArraySize(allowed_hosts);
        for (int i = 0; i < size; i++) {
            cJSON *host = cJSON_GetArrayItem(allowed_hosts, i);
            if (cJSON_IsString(host)) {
                printf("- %s\n", host->valuestring);
            }
        }
    }
}

// Поиск настройки по пути
cJSON *find_setting(cJSON *config, const char *key) {
    if (!config || !key) return NULL;
    
    // Ищем в корневом объекте
    cJSON *item = cJSON_GetObjectItem(config, key);
    if (item) return item;
    
    // Ищем в секции settings
    cJSON *settings = cJSON_GetObjectItem(config, "settings");
    if (settings) {
        item = cJSON_GetObjectItem(settings, key);
        if (item) return item;
    }
    
    return NULL;
}

// Определение типа значения по строке
int detect_value_type(const char *value) {
    if (!value) return -1;
    
    // Проверка на булево значение
    if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
        return 1; // boolean
    }
    
    // Проверка на число
    char *endptr;
    strtol(value, &endptr, 10);
    if (*endptr == '\0') {
        return 2; // integer
    }
    
    // Проверка на число с плавающей точкой
    strtod(value, &endptr);
    if (*endptr == '\0') {
        return 3; // double
    }
    
    // По умолчанию - строка
    return 4; // string
}

// Изменение настройки
int set_config_value(cJSON *config, const char *key, const char *value) {
    if (!config || !key || !value) return 0;
    
    // Находим настройку
    cJSON *item = find_setting(config, key);
    if (!item) {
        fprintf(stderr, "Настройка '%s' не найдена\n", key);
        return 0;
    }
    
    int value_type = detect_value_type(value);
    
    // Изменяем значение в зависимости от типа
    switch (value_type) {
        case 1: { // boolean
            int bool_val = (strcmp(value, "true") == 0);
            if (cJSON_IsBool(item)) {
                item->valueint = bool_val ? 1 : 0;
                item->type = bool_val ? cJSON_True : cJSON_False;
            } else {
                // Заменяем элемент
                cJSON *new_item = cJSON_CreateBool(bool_val);
                if (new_item) {
                    cJSON_ReplaceItemInObject(
                        cJSON_GetObjectItem(config, "settings") ? 
                        cJSON_GetObjectItem(config, "settings") : config,
                        key, new_item);
                }
            }
            break;
        }
        case 2: { // integer
            int int_val = atoi(value);
            if (cJSON_IsNumber(item)) {
                item->valueint = int_val;
                item->valuedouble = int_val;
            } else {
                cJSON *new_item = cJSON_CreateNumber(int_val);
                if (new_item) {
                    cJSON_ReplaceItemInObject(
                        cJSON_GetObjectItem(config, "settings") ? 
                        cJSON_GetObjectItem(config, "settings") : config,
                        key, new_item);
                }
            }
            break;
        }
        case 3: { // double
            double double_val = atof(value);
            if (cJSON_IsNumber(item)) {
                item->valuedouble = double_val;
                item->valueint = (int)double_val;
            } else {
                cJSON *new_item = cJSON_CreateNumber(double_val);
                if (new_item) {
                    cJSON_ReplaceItemInObject(
                        cJSON_GetObjectItem(config, "settings") ? 
                        cJSON_GetObjectItem(config, "settings") : config,
                        key, new_item);
                }
            }
            break;
        }
        case 4: { // string
            if (cJSON_IsString(item)) {
                // Освобождаем старую строку и создаём новую
                if (item->valuestring) free(item->valuestring);
                item->valuestring = strdup(value);
            } else {
                cJSON *new_item = cJSON_CreateString(value);
                if (new_item) {
                    cJSON_ReplaceItemInObject(
                        cJSON_GetObjectItem(config, "settings") ? 
                        cJSON_GetObjectItem(config, "settings") : config,
                        key, new_item);
                }
            }
            break;
        }
        default:
            fprintf(stderr, "Неизвестный тип значения\n");
            return 0;
    }
    
    return 1;
}

void print_usage(const char *prog_name) {
    printf("Использование:\n");
    printf("  %s                    - показать конфигурацию\n", prog_name);
    printf("  %s set <key> <value>  - изменить настройку\n", prog_name);
    printf("\nПримеры:\n");
    printf("  %s set debug true\n", prog_name);
    printf("  %s set max_connections 200\n", prog_name);
    printf("  %s set log_file /tmp/app.log\n", prog_name);
    printf("  %s set app_name \"New App Name\"\n", prog_name);
}

int main(int argc, char *argv[]) {
    const char *config_file = "config.json";
    
    // Загружаем конфигурацию
    cJSON *config = load_config(config_file);
    if (!config) {
        fprintf(stderr, "Не удалось загрузить конфигурацию. Создайте файл config.json\n");
        return 1;
    }
    
    if (argc == 1) {
        // Показать конфигурацию
        print_config(config);
    } else if (argc == 4 && strcmp(argv[1], "set") == 0) {
        // Изменить настройку
        if (set_config_value(config, argv[2], argv[3])) {
            if (save_config(config_file, config)) {
                printf("✅ Настройка '%s' изменена на '%s'\n", argv[2], argv[3]);
            } else {
                fprintf(stderr, "❌ Не удалось сохранить конфигурацию\n");
            }
        } else {
            fprintf(stderr, "❌ Не удалось изменить настройку '%s'\n", argv[2]);
        }
    } else {
        print_usage(argv[0]);
    }
    
    // Очистка памяти
    cJSON_Delete(config);
    
    return 0;
}