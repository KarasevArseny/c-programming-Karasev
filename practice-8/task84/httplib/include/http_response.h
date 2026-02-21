#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <stddef.h>

typedef struct {
    char version[16];
    int status_code;
    char status_text[64];
    HttpHeader headers[50];
    int header_count;
    char *body;
    size_t body_length;
} HttpResponse;

// Парсинг
HttpResponse* http_response_parse(const char *raw);
void http_response_free(HttpResponse *resp);

// Создание
HttpResponse* http_response_create(int status_code, const char *status_text);
void http_response_add_header(HttpResponse *resp, const char *name, const char *value);
void http_response_set_body(HttpResponse *resp, const char *body);

// Сериализация
char* http_response_to_string(const HttpResponse *resp);

// Утилиты
const char* http_response_get_header(const HttpResponse *resp, const char *name);
const char* http_status_text(int status_code);

#endif // HTTP_RESPONSE_H