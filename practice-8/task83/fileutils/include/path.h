#ifndef PATH_H
#define PATH_H

#include <stdbool.h>
#include <stddef.h>

// Разделитель пути в зависимости от платформы
#ifdef _WIN32
#define PATH_SEP '\\'
#define PATH_SEP_STR "\\"
#else
#define PATH_SEP '/'
#define PATH_SEP_STR "/"
#endif

// Функции для работы с путями
char* path_join(const char *base, const char *name);
char* path_dirname(const char *path);
char* path_basename(const char *path);
char* path_extension(const char *path);
char* path_without_extension(const char *path);
char* path_normalize(const char *path);
bool path_is_absolute(const char *path);
bool path_exists(const char *path);
bool path_is_file(const char *path);
bool path_is_directory(const char *path);

#endif // PATH_H