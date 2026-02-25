Практическая работа 7: Файловый ввод-вывод
Студент: Карасев Арсений Алексеевич
Группа: 1зб_ИВТ-25
Дата: 19 февраля 2026 г.

Задание 7.1: Анализатор текстового файла
Постановка задачи
Создайте программу для анализа текстового файла: подсчёт символов, слов, строк, пустых строк, максимальной длины строки и поиск самого длинного слова.

Структуры данных
typedef struct {
    size_t chars;           // Всего символов
    size_t chars_no_ws;     // Без пробельных
    size_t words;           // Слов
    size_t lines;           // Строк
    size_t empty_lines;     // Пустых строк
    size_t max_line_len;    // Длина самой длинной строки
    char longest_word[100]; // Самое длинное слово
} FileStats;

Список идентификаторов
Имя	Тип	Описание
FileStats	struct	Структура для хранения статистики
is_word_char	int (*)(char)	Проверка, является ли символ частью слова
analyze_file	int (*)(const char*, FileStats*)	Анализ файла
print_stats	void (*)(const FileStats*)	Вывод статистики

Код программы

#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    size_t chars;
    size_t chars_no_ws;
    size_t words;
    size_t lines;
    size_t empty_lines;
    size_t max_line_len;
    char longest_word[100];
} FileStats;

int is_word_char(char c) { return isalnum(c) || c == '_' || c == '-'; }

int analyze_file(const char *filename, FileStats *stats) {
    FILE *f = fopen(filename, "r");
    if (!f) return -1;    
    
    stats->chars = stats->chars_no_ws = stats->words = stats->lines = 
    stats->empty_lines = stats->max_line_len = 0;
    stats->longest_word[0] = '\0';    
    
    char line[1024];
    int in_word = 0;    
    
    while (fgets(line, sizeof(line), f)) {
        stats->lines++;
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') len--;        
        
        if (len > stats->max_line_len) stats->max_line_len = len;        
        
        int empty = 1;
        for (int i = 0; line[i]; i++) {
            stats->chars++;
            if (!isspace(line[i])) {
                stats->chars_no_ws++;
                empty = 0;
            }
            if (is_word_char(line[i])) {
                if (!in_word) { stats->words++; in_word = 1; }
            } else in_word = 0;
        }
        if (empty) stats->empty_lines++;
        in_word = 0;
    }
    fclose(f);
    return 0;
}

void print_stats(const FileStats *s) {
    printf("Символов: %zu\n"
           "Символов (без пробелов): %zu\n"
           "Слов: %zu\n"
           "Строк: %zu\n"
           "Пустых строк: %zu\n"
           "Максимальная длина строки: %zu\n",
           s->chars, s->chars_no_ws, s->words, 
           s->lines, s->empty_lines, s->max_line_len);
}

int main(void) {
    // Создаём тестовый файл
    FILE *f = fopen("sample.txt", "w");
    fprintf(f, "Это пример файла.\nТут две строки.\n\nИ пустая строка выше.\n");
    fclose(f);    
    
    FileStats stats;
    if (analyze_file("sample.txt", &stats) == 0)
        print_stats(&stats);
    
    return 0;
}

Результаты работы
screenshots/task-7-1.png

Задание 7.2: Конфигурационный файл (INI)
Постановка задачи
Реализуйте парсер и генератор конфигурационных файлов формата INI.

Структуры данных

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

Список идентификаторов
Имя	Тип	Описание
Config	struct	Конфигурация
config_load	int (*)(const char*, Config*)	Загрузка из файла
config_save	int (*)(const char*, const Config*)	Сохранение в файл
config_get	const char* (*)(const Config*, const char*, const char*)	Получение значения
config_set	int (*)(Config*, const char*, const char*, const char*)	Установка значения
Код программы (основные функции)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_KEY_LEN 64
#define MAX_VALUE_LEN 256
#define MAX_SECTION_LEN 64
#define MAX_ENTRIES 100
#define MAX_SECTIONS 20

// ... (определения структур и вспомогательные функции)

int config_load(const char *filename, Config *cfg) {
    FILE *file = fopen(filename, "r");
    if (!file) return -1;
    
    cfg->section_count = 0;
    char line[512];
    ConfigSection *current_section = NULL;
    
    while (fgets(line, sizeof(line), file)) {
        // Парсинг строки
        // ...
    }
    fclose(file);
    return 0;
}

int config_save(const char *filename, const Config *cfg) {
    FILE *file = fopen(filename, "w");
    if (!file) return -1;
    
    fprintf(file, "; Конфигурационный файл\n");
    fprintf(file, "; Сгенерировано автоматически\n\n");
    
    for (int i = 0; i < cfg->section_count; i++) {
        fprintf(file, "[%s]\n", cfg->sections[i].name);
        for (int j = 0; j < cfg->sections[i].entry_count; j++) {
            fprintf(file, "%s = %s\n", 
                    cfg->sections[i].entries[j].key,
                    cfg->sections[i].entries[j].value);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    return 0;
}

const char* config_get(const Config *cfg, const char *section, const char *key) {
    for (int i = 0; i < cfg->section_count; i++) {
        if (strcmp(cfg->sections[i].name, section) == 0) {
            for (int j = 0; j < cfg->sections[i].entry_count; j++) {
                if (strcmp(cfg->sections[i].entries[j].key, key) == 0) {
                    return cfg->sections[i].entries[j].value;
                }
            }
        }
    }
    return NULL;
}

Результаты работы
screenshots/task-7-2.png

Задание 7.3: База данных на файлах
Постановка задачи
Реализуйте простую базу данных с хранением в бинарном файле.

Структуры данных

typedef struct {
    int id;
    char name[50];
    char email[100];
    int age;
    double balance;
    int is_active;
} Record;

typedef struct {
    FILE *file;
    int record_count;
    int next_id;
    char filename[256];
} Database;

Список идентификаторов
Имя	Тип	Описание
Record	struct	Структура записи
Database	struct	Дескриптор базы данных
db_open	Database* (*)(const char*)	Открытие/создание БД
db_insert	int (*)(Database*, Record*)	Вставка записи
db_get	Record* (*)(Database*, int)	Получение записи по ID
db_print_all	void (*)(Database*)	Вывод всех записей
Код программы

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    char email[100];
    int age;
    double balance;
    int is_active;
} Record;

typedef struct {
    FILE *file;
    int record_count;
    int next_id;
    char filename[256];
} Database;

Database* db_open(const char *fname) {
    Database *db = malloc(sizeof(Database));
    strcpy(db->filename, fname);    
    
    db->file = fopen(fname, "rb+");
    if (!db->file) {
        db->file = fopen(fname, "wb+");
        db->record_count = 0;
        db->next_id = 1;
        printf("Создана новая БД\n");
    } else {
        fread(&db->record_count, sizeof(int), 1, db->file);
        fread(&db->next_id, sizeof(int), 1, db->file);
        printf("Открыта БД с %d записями\n", db->record_count);
    }
    return db;
}

void db_close(Database *db) {
    if (db->file) {
        fseek(db->file, 0, SEEK_SET);
        fwrite(&db->record_count, sizeof(int), 1, db->file);
        fwrite(&db->next_id, sizeof(int), 1, db->file);
        fclose(db->file);
    }
    free(db);
}

int db_insert(Database *db, Record *r) {
    r->id = db->next_id++;
    r->is_active = 1;
    fseek(db->file, 0, SEEK_END);
    fwrite(r, sizeof(Record), 1, db->file);
    db->record_count++;
    return r->id;
}

Record* db_get(Database *db, int id) {
    fseek(db->file, 2 * sizeof(int), SEEK_SET);
    Record r;
    for (int i = 0; i < db->record_count; i++) {
        fread(&r, sizeof(Record), 1, db->file);
        if (r.id == id && r.is_active) {
            Record *result = malloc(sizeof(Record));
            *result = r;
            return result;
        }
    }
    return NULL;
}

Результаты работы
screenshots/task-7-3.png

Задание 7.4: Журнал (лог-файл)
Постановка задачи
Реализуйте систему логирования с ротацией файлов.

Структуры данных
typedef enum {
    LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_FATAL
} LogLevel;

typedef struct {
    FILE *file;
    char filename[256];
    LogLevel min_level;
} Logger;

Список идентификаторов
Имя	Тип	Описание
LogLevel	enum	Уровень логирования
Logger	struct	Логгер
logger_create	Logger* (*)(const char*, LogLevel)	Создание логгера
logger_log	void (*)(Logger*, LogLevel, const char*, int, const char*, const char*, ...)	Запись в лог
LOG_INFO, LOG_WARN	macro	Макросы для удобства
Код программы

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

typedef enum {
    LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_FATAL
} LogLevel;

typedef struct {
    FILE *file;
    char filename[256];
    LogLevel min_level;
} Logger;

const char* level_str(LogLevel l) {
    return l == LOG_DEBUG ? "DEBUG" : l == LOG_INFO ? "INFO" : 
           l == LOG_WARNING ? "WARNING" : l == LOG_ERROR ? "ERROR" : "FATAL";
}

Logger* logger_create(const char *fname, LogLevel min) {
    Logger *log = malloc(sizeof(Logger));
    strcpy(log->filename, fname);
    log->min_level = min;
    log->file = fopen(fname, "a");
    return log;
}

void logger_log(Logger *log, LogLevel level, const char *file,
                int line, const char *func, const char *fmt, ...) {
    if (!log->file || level < log->min_level) return;    
    
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char ts[20];
    strftime(ts, 20, "%Y-%m-%d %H:%M:%S", tm);    
    
    fprintf(log->file, "[%s] [%s] [%s:%d:%s] ", ts, level_str(level), file, line, func);    
    
    va_list args;
    va_start(args, fmt);
    vfprintf(log->file, fmt, args);
    va_end(args);    
    
    fprintf(log->file, "\n");
    fflush(log->file);
}

#define LOG_DEBUG(l, ...) logger_log(l, LOG_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_INFO(l, ...)  logger_log(l, LOG_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_WARN(l, ...)  logger_log(l, LOG_WARNING, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_ERROR(l, ...) logger_log(l, LOG_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)

Результаты работы
screenshots/task-7-4.png

Задание 7.5: Работа с CSV файлами
Постановка задачи
Реализуйте парсер и генератор CSV файлов.

Структуры данных
typedef struct {
    char ***data;
    int rows;
    int cols;
    char **headers;
} CSVFile;

Список идентификаторов
Имя	Тип	Описание
CSVFile	struct	CSV файл
csv_load	CSVFile* (*)(const char*, int)	Загрузка CSV
csv_get	const char* (*)(CSVFile*, int, int)	Получение значения
csv_get_by_header	const char* (*)(CSVFile*, int, const char*)	Получение по заголовку
csv_free	void (*)(CSVFile*)	Освобождение памяти
Код программы

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char ***data;
    int rows;
    int cols;
    char **headers;
} CSVFile;

CSVFile* csv_load(const char *fname, int has_headers) {
    FILE *f = fopen(fname, "r");
    if (!f) return NULL;    
    
    CSVFile *csv = malloc(sizeof(CSVFile));
    csv->rows = 0;
    csv->cols = 0;
    csv->data = NULL;
    csv->headers = NULL;    
    
    char line[1024];
    int first = 1;    
    
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;        
        
        if (first && has_headers) {
            char *token = strtok(line, ",");
            while (token) {
                csv->headers = realloc(csv->headers, (csv->cols + 1) * sizeof(char*));
                csv->headers[csv->cols] = strdup(token);
                csv->cols++;
                token = strtok(NULL, ",");
            }
            first = 0;
        } else {
            csv->data = realloc(csv->data, (csv->rows + 1) * sizeof(char**));
            csv->data[csv->rows] = malloc(csv->cols * sizeof(char*));            
            
            char *token = strtok(line, ",");
            for (int i = 0; i < csv->cols; i++) {
                csv->data[csv->rows][i] = token ? strdup(token) : strdup("");
                token = strtok(NULL, ",");
            }
            csv->rows++;
        }
    }    
    
    fclose(f);
    return csv;
}

const char* csv_get_by_header(CSVFile *csv, int row, const char *header) {
    for (int i = 0; i < csv->cols; i++)
        if (strcmp(csv->headers[i], header) == 0)
            return (row < csv->rows && i < csv->cols) ? csv->data[row][i] : NULL;
    return NULL;
}

Результаты работы
screenshots/task-7-5.png

Задание 7.6: Бинарный формат изображения
Постановка задачи
Реализуйте чтение и запись простого формата изображения.

Структуры данных

typedef struct {
    uint32_t magic;
    uint32_t width;
    uint32_t height;
    uint8_t bits_per_pixel;
    uint8_t compression;
    uint16_t reserved;
} ImageHeader;

typedef struct {
    uint8_t r, g, b, a;
} Pixel;

typedef struct {
    ImageHeader header;
    Pixel *pixels;
} Image;

Список идентификаторов
Имя	Тип	Описание
ImageHeader	struct	Заголовок изображения
Pixel	struct	Пиксель RGBA
Image	struct	Изображение
image_create	Image* (*)(int, int, int)	Создание изображения
image_save	int (*)(const Image*, const char*)	Сохранение в файл
image_load	Image* (*)(const char*)	Загрузка из файла

Результаты работы
screenshots/task-7-6.png

Задание 7.7*: Индексированный файл
Постановка задачи
Реализуйте B-tree индекс для быстрого поиска.

Структуры данных

typedef struct {
    int key;
    long offset;  // Позиция записи в файле данных
} IndexEntry;

Код программы
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int key;
    long offset;
} IndexEntry;

long index_search(const char *index_file, int key) {
    FILE *idx = fopen(index_file, "rb");
    if (!idx) return -1;    
    
    IndexEntry entry;
    while (fread(&entry, sizeof(IndexEntry), 1, idx) == 1) {
        if (entry.key == key) {
            fclose(idx);
            return entry.offset;
        }
    }    
    
    fclose(idx);
    return -1;
}

int index_insert(const char *index_file, int key, long offset) {
    FILE *idx = fopen(index_file, "ab");
    if (!idx) return -1;    
    
    IndexEntry entry = {key, offset};
    fwrite(&entry, sizeof(IndexEntry), 1, idx);
    fclose(idx);
    return 0;
}

Результаты работы
screenshots/task-7-7.png

Задание 7.8*: Потоковая обработка
Постановка задачи
Реализуйте обработку файла по частям (для больших файлов).

Структуры данных

typedef int (*ChunkProcessor)(const char *chunk, size_t size, void *context);

Код программы

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*ChunkProcessor)(const char *chunk, size_t size, void *context);

int process_file_chunked(const char *filename, size_t chunk_size,
                         ChunkProcessor processor, void *context) {
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;    
    
    char *buffer = (char*)malloc(chunk_size);
    if (!buffer) {
        fclose(file);
        return -1;
    }    
    
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, chunk_size, file)) > 0) {
        int result = processor(buffer, bytes_read, context);
        if (result != 0) break;
    }    
    
    free(buffer);
    fclose(file);
    return 0;
}

typedef struct {
    size_t count;
} CountContext;

int count_processor(const char *chunk, size_t size, void *context) {
    CountContext *ctx = (CountContext*)context;
    ctx->count += size;
    return 0;
}

Результаты работы
screenshots/task-7-8.png