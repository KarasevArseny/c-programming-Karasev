#include "path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define stat _stat
#else
#include <unistd.h>
#endif

// Внутренняя функция для копирования строки
static char* strdup_safe(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char *copy = (char*)malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}

// Внутренняя функция для проверки существования
static int file_exists_internal(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

char* path_join(const char *base, const char *name) {
    if (!base && !name) return NULL;
    if (!base) return strdup_safe(name);
    if (!name) return strdup_safe(base);
    
    size_t base_len = strlen(base);
    size_t name_len = strlen(name);
    
    // Убираем завершающий разделитель в base
    while (base_len > 0 && base[base_len - 1] == PATH_SEP) {
        base_len--;
    }
    
    // Убираем начальный разделитель в name
    size_t name_start = 0;
    while (name_start < name_len && name[name_start] == PATH_SEP) {
        name_start++;
    }
    
    size_t result_len = base_len + 1 + (name_len - name_start) + 1;
    char *result = (char*)malloc(result_len);
    
    if (result) {
        if (base_len > 0) {
            memcpy(result, base, base_len);
        }
        result[base_len] = PATH_SEP;
        
        if (name_len - name_start > 0) {
            memcpy(result + base_len + 1, name + name_start, name_len - name_start);
        }
        
        result[base_len + 1 + (name_len - name_start)] = '\0';
    }
    
    return result;
}

char* path_dirname(const char *path) {
    if (!path || *path == '\0') return strdup_safe(".");
    
    // Находим последний разделитель
    const char *last_sep = NULL;
    for (const char *p = path; *p; p++) {
        if (*p == '/' || *p == '\\') {
            last_sep = p;
        }
    }
    
    if (!last_sep) {
        // Нет разделителя - возвращаем "."
        return strdup_safe(".");
    }
    
    // Если последний разделитель - первый символ
    if (last_sep == path) {
        return strdup_safe(PATH_SEP_STR);
    }
    
    size_t len = last_sep - path;
    char *result = (char*)malloc(len + 1);
    if (result) {
        memcpy(result, path, len);
        result[len] = '\0';
    }
    
    return result;
}

char* path_basename(const char *path) {
    if (!path || *path == '\0') return strdup_safe("");
    
    // Находим последний разделитель
    const char *last_sep = NULL;
    for (const char *p = path; *p; p++) {
        if (*p == '/' || *p == '\\') {
            last_sep = p;
        }
    }
    
    const char *start = last_sep ? last_sep + 1 : path;
    return strdup_safe(start);
}

char* path_extension(const char *path) {
    if (!path) return NULL;
    
    const char *basename = path_basename(path);
    if (!basename) return NULL;
    
    const char *dot = strrchr(basename, '.');
    char *result = dot ? strdup_safe(dot) : strdup_safe("");
    
    free((void*)basename);
    return result;
}

char* path_without_extension(const char *path) {
    if (!path) return NULL;
    
    const char *basename = path_basename(path);
    if (!basename) return NULL;
    
    const char *dot = strrchr(basename, '.');
    char *result;
    
    if (dot) {
        size_t len = dot - basename;
        result = (char*)malloc(len + 1);
        if (result) {
            memcpy(result, basename, len);
            result[len] = '\0';
        }
    } else {
        result = strdup_safe(basename);
    }
    
    free((void*)basename);
    return result;
}

char* path_normalize(const char *path) {
    if (!path) return NULL;
    
    // Простая нормализация: убираем лишние разделители
    size_t len = strlen(path);
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    int j = 0;
    int prev_sep = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (path[i] == '/' || path[i] == '\\') {
            if (!prev_sep) {
                result[j++] = PATH_SEP;
                prev_sep = 1;
            }
        } else {
            result[j++] = path[i];
            prev_sep = 0;
        }
    }
    
    result[j] = '\0';
    
    // Убираем завершающий разделитель, если путь не корневой
    if (j > 1 && result[j-1] == PATH_SEP) {
        result[j-1] = '\0';
    }
    
    return result;
}

bool path_is_absolute(const char *path) {
    if (!path) return false;
    
#ifdef _WIN32
    // Windows: C:\ или \\server\share
    if (strlen(path) >= 2 && path[1] == ':') return true;
    if (strlen(path) >= 2 && path[0] == '\\' && path[1] == '\\') return true;
#else
    // Unix: начинается с /
    if (path[0] == '/') return true;
#endif
    
    return false;
}

bool path_exists(const char *path) {
    return file_exists_internal(path);
}

bool path_is_file(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISREG(st.st_mode);
}

bool path_is_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return false;
    return S_ISDIR(st.st_mode);
}