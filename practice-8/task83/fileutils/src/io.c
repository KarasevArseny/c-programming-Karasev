#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define F_OK 0
#else
#include <unistd.h>
#endif

char* file_read_all(const char *filename, size_t *size) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        if (size) *size = 0;
        return NULL;
    }
    
    // Определяем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size < 0) {
        fclose(file);
        if (size) *size = 0;
        return NULL;
    }
    
    // Выделяем память
    char *buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        if (size) *size = 0;
        return NULL;
    }
    
    // Читаем файл
    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[bytes_read] = '\0';
    
    fclose(file);
    
    if (size) *size = bytes_read;
    return buffer;
}

int file_write_all(const char *filename, const char *content) {
    FILE *file = fopen(filename, "w");
    if (!file) return -1;
    
    size_t len = content ? strlen(content) : 0;
    size_t written = fwrite(content, 1, len, file);
    
    fclose(file);
    
    return (written == len) ? 0 : -1;
}

int file_append(const char *filename, const char *content) {
    FILE *file = fopen(filename, "a");
    if (!file) return -1;
    
    size_t len = content ? strlen(content) : 0;
    size_t written = fwrite(content, 1, len, file);
    
    fclose(file);
    
    return (written == len) ? 0 : -1;
}

char** file_read_lines(const char *filename, size_t *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        if (count) *count = 0;
        return NULL;
    }
    
    // Первый проход - подсчёт строк
    char buffer[4096];
    size_t line_count = 0;
    
    while (fgets(buffer, sizeof(buffer), file)) {
        line_count++;
    }
    
    // Возвращаемся в начало
    rewind(file);
    
    // Выделяем память под массив строк
    char **lines = (char**)malloc(line_count * sizeof(char*));
    if (!lines) {
        fclose(file);
        if (count) *count = 0;
        return NULL;
    }
    
    // Второй проход - чтение строк
    size_t index = 0;
    while (fgets(buffer, sizeof(buffer), file) && index < line_count) {
        // Удаляем символ новой строки
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len-1] == '\n' || buffer[len-1] == '\r')) {
            buffer[len-1] = '\0';
        }
        if (len > 1 && (buffer[len-2] == '\r')) {
            buffer[len-2] = '\0';
        }
        
        lines[index] = (char*)malloc(strlen(buffer) + 1);
        if (lines[index]) {
            strcpy(lines[index], buffer);
            index++;
        }
    }
    
    fclose(file);
    
    if (count) *count = index;
    return lines;
}

void file_free_lines(char **lines, size_t count) {
    if (!lines) return;
    
    for (size_t i = 0; i < count; i++) {
        free(lines[i]);
    }
    
    free(lines);
}

int file_copy(const char *src, const char *dst) {
    FILE *src_file = fopen(src, "rb");
    if (!src_file) return -1;
    
    FILE *dst_file = fopen(dst, "wb");
    if (!dst_file) {
        fclose(src_file);
        return -1;
    }
    
    char buffer[8192];
    size_t bytes_read;
    size_t bytes_written;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        bytes_written = fwrite(buffer, 1, bytes_read, dst_file);
        if (bytes_written != bytes_read) {
            fclose(src_file);
            fclose(dst_file);
            return -1;
        }
    }
    
    fclose(src_file);
    fclose(dst_file);
    
    return 0;
}

int file_move(const char *src, const char *dst) {
    // Пробуем rename (работает если на одном томе)
    if (rename(src, dst) == 0) {
        return 0;
    }
    
    // Если rename не сработал, копируем и удаляем
    if (file_copy(src, dst) == 0) {
        remove(src);
        return 0;
    }
    
    return -1;
}

int file_delete(const char *path) {
    return remove(path) == 0 ? 0 : -1;
}

long file_size(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    
    return size;
}

bool file_exists(const char *filename) {
    return access(filename, F_OK) == 0;
}