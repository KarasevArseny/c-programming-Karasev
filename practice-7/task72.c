#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============ КОНСТАНТЫ ============

#define MAX_KEY_LEN 64
#define MAX_VALUE_LEN 256
#define MAX_SECTION_LEN 64
#define MAX_ENTRIES 100
#define MAX_SECTIONS 20

// ============ СТРУКТУРЫ ДАННЫХ ============

typedef struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
} ConfigEntry;

typedef struct {
    char name[MAX_SECTION_LEN];
    ConfigEntry entries[MAX_ENTRIES];
    int entry_count;
} ConfigSection;

typedef struct {
    ConfigSection sections[MAX_SECTIONS];
    int section_count;
} Config;

// ============ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ============

// Удаление пробелов в начале и конце строки
char* trim(char *str) {
    if (!str) return NULL;
    
    // Удаляем пробелы в начале
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == 0) return str;
    
    // Удаляем пробелы в конце
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    end[1] = '\0';
    return str;
}

// Проверка, является ли строка комментарием или пустой
int is_comment_or_empty(const char *line) {
    while (isspace((unsigned char)*line)) line++;
    return *line == '\0' || *line == ';' || *line == '#';
}

// ============ ПОИСК СЕКЦИИ ============

ConfigSection* find_section(Config *cfg, const char *name) {
    for (int i = 0; i < cfg->section_count; i++) {
        if (strcmp(cfg->sections[i].name, name) == 0) {
            return &cfg->sections[i];
        }
    }
    return NULL;
}

ConfigEntry* find_entry(ConfigSection *section, const char *key) {
    if (!section) return NULL;
    
    for (int i = 0; i < section->entry_count; i++) {
        if (strcmp(section->entries[i].key, key) == 0) {
            return &section->entries[i];
        }
    }
    return NULL;
}

// ============ ЗАГРУЗКА КОНФИГУРАЦИИ ============

int config_load(const char *filename, Config *cfg) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return -1;
    }
    
    // Инициализация конфигурации
    cfg->section_count = 0;
    
    char line[512];
    int line_num = 0;
    ConfigSection *current_section = NULL;
    
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        
        // Удаляем символ новой строки
        char *p = strchr(line, '\n');
        if (p) *p = '\0';
        p = strchr(line, '\r');
        if (p) *p = '\0';
        
        // Пропускаем комментарии и пустые строки
        if (is_comment_or_empty(line)) {
            continue;
        }
        
        // Проверяем, является ли строка секцией [section]
        if (line[0] == '[') {
            char *end = strchr(line, ']');
            if (!end) {
                printf("Ошибка в строке %d: неверный формат секции\n", line_num);
                fclose(file);
                return -1;
            }
            
            *end = '\0';
            char *section_name = trim(line + 1);
            
            if (cfg->section_count >= MAX_SECTIONS) {
                printf("Ошибка: превышено максимальное количество секций (%d)\n", MAX_SECTIONS);
                fclose(file);
                return -1;
            }
            
            // Добавляем новую секцию
            current_section = &cfg->sections[cfg->section_count];
            strncpy(current_section->name, section_name, MAX_SECTION_LEN - 1);
            current_section->name[MAX_SECTION_LEN - 1] = '\0';
            current_section->entry_count = 0;
            cfg->section_count++;
            
            continue;
        }
        
        // Если это не секция, то это должна быть запись ключ=значение
        char *eq = strchr(line, '=');
        if (!eq) {
            printf("Предупреждение в строке %d: пропущена строка без '='\n", line_num);
            continue;
        }
        
        *eq = '\0';
        char *key = trim(line);
        char *value = trim(eq + 1);
        
        if (strlen(key) == 0) {
            printf("Предупреждение в строке %d: пустой ключ\n", line_num);
            continue;
        }
        
        if (!current_section) {
            // Если нет текущей секции, создаём секцию по умолчанию
            if (cfg->section_count >= MAX_SECTIONS) {
                printf("Ошибка: превышено максимальное количество секций\n");
                fclose(file);
                return -1;
            }
            
            current_section = &cfg->sections[cfg->section_count];
            strcpy(current_section->name, "default");
            current_section->entry_count = 0;
            cfg->section_count++;
        }
        
        if (current_section->entry_count >= MAX_ENTRIES) {
            printf("Ошибка: превышено максимальное количество записей в секции %s\n", 
                   current_section->name);
            fclose(file);
            return -1;
        }
        
        ConfigEntry *entry = &current_section->entries[current_section->entry_count];
        strncpy(entry->key, key, MAX_KEY_LEN - 1);
        entry->key[MAX_KEY_LEN - 1] = '\0';
        strncpy(entry->value, value, MAX_VALUE_LEN - 1);
        entry->value[MAX_VALUE_LEN - 1] = '\0';
        
        current_section->entry_count++;
    }
    
    fclose(file);
    return 0;
}

// ============ СОХРАНЕНИЕ КОНФИГУРАЦИИ ============

int config_save(const char *filename, const Config *cfg) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка: не удалось создать файл %s\n", filename);
        return -1;
    }
    
    fprintf(file, "; Конфигурационный файл\n");
    fprintf(file, "; Сгенерировано автоматически\n\n");
    
    for (int i = 0; i < cfg->section_count; i++) {
        const ConfigSection *section = &cfg->sections[i];
        
        fprintf(file, "[%s]\n", section->name);
        
        for (int j = 0; j < section->entry_count; j++) {
            const ConfigEntry *entry = &section->entries[j];
            fprintf(file, "%s = %s\n", entry->key, entry->value);
        }
        
        fprintf(file, "\n");
    }
    
    fclose(file);
    return 0;
}

// ============ ПОЛУЧЕНИЕ ЗНАЧЕНИЯ ============

const char* config_get(const Config *cfg, const char *section, const char *key) {
    for (int i = 0; i < cfg->section_count; i++) {
        if (strcmp(cfg->sections[i].name, section) == 0) {
            const ConfigSection *sec = &cfg->sections[i];
            
            for (int j = 0; j < sec->entry_count; j++) {
                if (strcmp(sec->entries[j].key, key) == 0) {
                    return sec->entries[j].value;
                }
            }
            
            break;
        }
    }
    
    return NULL;
}

// ============ УСТАНОВКА ЗНАЧЕНИЯ ============

int config_set(Config *cfg, const char *section, const char *key, const char *value) {
    // Ищем существующую секцию
    ConfigSection *sec = NULL;
    
    for (int i = 0; i < cfg->section_count; i++) {
        if (strcmp(cfg->sections[i].name, section) == 0) {
            sec = &cfg->sections[i];
            break;
        }
    }
    
    // Если секция не найдена, создаём новую
    if (!sec) {
        if (cfg->section_count >= MAX_SECTIONS) {
            printf("Ошибка: превышено максимальное количество секций\n");
            return -1;
        }
        
        sec = &cfg->sections[cfg->section_count];
        strncpy(sec->name, section, MAX_SECTION_LEN - 1);
        sec->name[MAX_SECTION_LEN - 1] = '\0';
        sec->entry_count = 0;
        cfg->section_count++;
    }
    
    // Ищем существующий ключ в секции
    for (int i = 0; i < sec->entry_count; i++) {
        if (strcmp(sec->entries[i].key, key) == 0) {
            // Обновляем существующее значение
            strncpy(sec->entries[i].value, value, MAX_VALUE_LEN - 1);
            sec->entries[i].value[MAX_VALUE_LEN - 1] = '\0';
            return 0;
        }
    }
    
    // Если ключ не найден, добавляем новый
    if (sec->entry_count >= MAX_ENTRIES) {
        printf("Ошибка: превышено максимальное количество записей в секции\n");
        return -1;
    }
    
    ConfigEntry *entry = &sec->entries[sec->entry_count];
    strncpy(entry->key, key, MAX_KEY_LEN - 1);
    entry->key[MAX_KEY_LEN - 1] = '\0';
    strncpy(entry->value, value, MAX_VALUE_LEN - 1);
    entry->value[MAX_VALUE_LEN - 1] = '\0';
    
    sec->entry_count++;
    return 0;
}

// ============ УДАЛЕНИЕ КЛЮЧА ============

int config_remove(Config *cfg, const char *section, const char *key) {
    for (int i = 0; i < cfg->section_count; i++) {
        if (strcmp(cfg->sections[i].name, section) == 0) {
            ConfigSection *sec = &cfg->sections[i];
            
            for (int j = 0; j < sec->entry_count; j++) {
                if (strcmp(sec->entries[j].key, key) == 0) {
                    // Сдвигаем оставшиеся записи
                    for (int k = j; k < sec->entry_count - 1; k++) {
                        sec->entries[k] = sec->entries[k + 1];
                    }
                    sec->entry_count--;
                    return 0;
                }
            }
            
            break;
        }
    }
    
    return -1; // Ключ не найден
}

// ============ ПРОВЕРКА СУЩЕСТВОВАНИЯ ============

int config_has_section(const Config *cfg, const char *section) {
    for (int i = 0; i < cfg->section_count; i++) {
        if (strcmp(cfg->sections[i].name, section) == 0) {
            return 1;
        }
    }
    return 0;
}

int config_has_key(const Config *cfg, const char *section, const char *key) {
    for (int i = 0; i < cfg->section_count; i++) {
        if (strcmp(cfg->sections[i].name, section) == 0) {
            const ConfigSection *sec = &cfg->sections[i];
            
            for (int j = 0; j < sec->entry_count; j++) {
                if (strcmp(sec->entries[j].key, key) == 0) {
                    return 1;
                }
            }
            
            break;
        }
    }
    return 0;
}

// ============ ВЫВОД КОНФИГУРАЦИИ ============

void config_print(const Config *cfg) {
    printf("=== Конфигурация ===\n");
    
    for (int i = 0; i < cfg->section_count; i++) {
        const ConfigSection *sec = &cfg->sections[i];
        
        printf("\n[%s]\n", sec->name);
        
        for (int j = 0; j < sec->entry_count; j++) {
            const ConfigEntry *entry = &sec->entries[j];
            printf("  %s = %s\n", entry->key, entry->value);
        }
    }
    
    printf("\nВсего секций: %d\n", cfg->section_count);
}

// ============ СОЗДАНИЕ ТЕСТОВОГО INI-ФАЙЛА ============

void create_test_ini(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "; Тестовый конфигурационный файл\n");
    fprintf(file, "; для демонстрации работы парсера\n\n");
    
    fprintf(file, "[database]\n");
    fprintf(file, "host = localhost\n");
    fprintf(file, "port = 5432\n");
    fprintf(file, "name = myapp_db\n");
    fprintf(file, "user = admin\n");
    fprintf(file, "password = secret123\n\n");
    
    fprintf(file, "[server]\n");
    fprintf(file, "address = 127.0.0.1\n");
    fprintf(file, "port = 8080\n");
    fprintf(file, "timeout = 30\n\n");
    
    fprintf(file, "[logging]\n");
    fprintf(file, "level = info\n");
    fprintf(file, "file = app.log\n");
    fprintf(file, "max_size = 10485760\n\n");
    
    fprintf(file, "; Пустая секция для теста\n");
    fprintf(file, "[empty]\n\n");
    
    fprintf(file, "[app]\n");
    fprintf(file, "name = My Application\n");
    fprintf(file, "version = 1.0.0\n");
    fprintf(file, "debug = true\n");
    
    fclose(file);
    printf("Создан тестовый файл: %s\n", filename);
}

// ============ ДЕМОНСТРАЦИЯ ============

int main() {
    printf("========================================\n");
    printf("   ПАРСЕР И ГЕНЕРАТОР INI-ФАЙЛОВ\n");
    printf("========================================\n\n");
    
    const char *filename = "config.ini";
    
    // Создаём тестовый INI-файл
    create_test_ini(filename);
    printf("\n");
    
    // Загружаем конфигурацию
    Config cfg;
    if (config_load(filename, &cfg) == 0) {
        printf("Файл успешно загружен!\n\n");
        
        // Выводим загруженную конфигурацию
        config_print(&cfg);
        
        // Демонстрация получения значений
        printf("\n=== ПОЛУЧЕНИЕ ЗНАЧЕНИЙ ===\n");
        const char *host = config_get(&cfg, "database", "host");
        const char *port = config_get(&cfg, "database", "port");
        const char *level = config_get(&cfg, "logging", "level");
        
        printf("database.host = %s\n", host ? host : "не найден");
        printf("database.port = %s\n", port ? port : "не найден");
        printf("logging.level = %s\n", level ? level : "не найден");
        
        // Демонстрация проверки существования
        printf("\n=== ПРОВЕРКА СУЩЕСТВОВАНИЯ ===\n");
        printf("Секция 'database' существует? %s\n", 
               config_has_section(&cfg, "database") ? "да" : "нет");
        printf("Секция 'nonexistent' существует? %s\n", 
               config_has_section(&cfg, "nonexistent") ? "да" : "нет");
        printf("Ключ 'host' в секции 'database' существует? %s\n", 
               config_has_key(&cfg, "database", "host") ? "да" : "нет");
        printf("Ключ 'nonexistent' в секции 'database' существует? %s\n", 
               config_has_key(&cfg, "database", "nonexistent") ? "да" : "нет");
        
        // Демонстрация добавления/изменения значений
        printf("\n=== ИЗМЕНЕНИЕ ЗНАЧЕНИЙ ===\n");
        
        printf("Добавляем новый ключ: database.timeout = 60\n");
        config_set(&cfg, "database", "timeout", "60");
        
        printf("Изменяем существующий: server.port = 9090\n");
        config_set(&cfg, "server", "port", "9090");
        
        printf("Удаляем ключ: logging.max_size\n");
        config_remove(&cfg, "logging", "max_size");
        
        // Выводим изменённую конфигурацию
        printf("\n=== КОНФИГУРАЦИЯ ПОСЛЕ ИЗМЕНЕНИЙ ===\n");
        config_print(&cfg);
        
        // Сохраняем в новый файл
        const char *new_filename = "config_new.ini";
        if (config_save(new_filename, &cfg) == 0) {
            printf("\nКонфигурация сохранена в файл: %s\n", new_filename);
        }
        
        // Демонстрация работы с секцией по умолчанию
        printf("\n=== ТЕСТ СЕКЦИИ ПО УМОЛЧАНИЮ ===\n");
        Config cfg2;
        cfg2.section_count = 0;
        
        // Добавляем ключ без указания секции (будет создана секция "default")
        config_set(&cfg2, "default", "test_key", "test_value");
        config_set(&cfg2, "default", "another_key", "another_value");
        
        printf("Конфигурация с секцией по умолчанию:\n");
        config_print(&cfg2);
    } else {
        printf("Ошибка при загрузке файла\n");
    }
    
    return 0;
}