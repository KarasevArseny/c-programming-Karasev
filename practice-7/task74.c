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
int main(void) {
Logger *log = logger_create("app.log", LOG_INFO);    
LOG_INFO(log, "Application started");
LOG_DEBUG(log, "Connecting to database...");
LOG_WARN(log, "Slow connection (1.5s)");
LOG_ERROR(log, "Invalid password for user \"admin\"");    
fclose(log->file);
free(log);
return 0;
}