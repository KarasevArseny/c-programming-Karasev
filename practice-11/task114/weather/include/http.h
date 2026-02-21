#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

// Структура для хранения HTTP-ответа
typedef struct {
    char *data;
    size_t size;
} HttpResponse;

// Инициализация и очистка HTTP
void http_global_init(void);
void http_global_cleanup(void);

// Выполнение HTTP-запроса
int http_get(const char *url, HttpResponse *resp);

// Освобождение памяти ответа
void http_response_free(HttpResponse *resp);

// Формирование URL для API погоды
void http_build_weather_url(const char *city, char *url, size_t url_size);

#endif // HTTP_H