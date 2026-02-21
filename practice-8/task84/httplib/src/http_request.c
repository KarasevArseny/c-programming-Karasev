#include "http_request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Внутренняя функция для обрезки пробелов
static char* trim_whitespace(char *str) {
    if (!str) return NULL;
    
    // Обрезаем в начале
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == 0) return str;
    
    // Обрезаем в конце
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    end[1] = '\0';
    
    return str;
}

const char* http_method_to_string(HttpMethod method) {
    switch (method) {
        case HTTP_GET:     return "GET";
        case HTTP_POST:    return "POST";
        case HTTP_PUT:     return "PUT";
        case HTTP_DELETE:  return "DELETE";
        case HTTP_HEAD:    return "HEAD";
        case HTTP_OPTIONS: return "OPTIONS";
        default:           return "UNKNOWN";
    }
}

HttpMethod http_method_from_string(const char *str) {
    if (strcmp(str, "GET") == 0) return HTTP_GET;
    if (strcmp(str, "POST") == 0) return HTTP_POST;
    if (strcmp(str, "PUT") == 0) return HTTP_PUT;
    if (strcmp(str, "DELETE") == 0) return HTTP_DELETE;
    if (strcmp(str, "HEAD") == 0) return HTTP_HEAD;
    if (strcmp(str, "OPTIONS") == 0) return HTTP_OPTIONS;
    return HTTP_UNKNOWN;
}

HttpRequest* http_request_create(HttpMethod method, const char *path) {
    HttpRequest *req = (HttpRequest*)calloc(1, sizeof(HttpRequest));
    if (!req) return NULL;
    
    req->method = method;
    if (path) {
        strncpy(req->path, path, sizeof(req->path) - 1);
    } else {
        strcpy(req->path, "/");
    }
    strcpy(req->version, "HTTP/1.1");
    req->header_count = 0;
    req->body = NULL;
    req->body_length = 0;
    
    return req;
}

void http_request_add_header(HttpRequest *req, const char *name, const char *value) {
    if (!req || req->header_count >= 50) return;
    
    HttpHeader *header = &req->headers[req->header_count];
    strncpy(header->name, name, sizeof(header->name) - 1);
    strncpy(header->value, value, sizeof(header->value) - 1);
    req->header_count++;
}

void http_request_set_body(HttpRequest *req, const char *body) {
    if (!req) return;
    
    // Освобождаем старый body, если есть
    if (req->body) {
        free(req->body);
    }
    
    if (body) {
        req->body_length = strlen(body);
        req->body = (char*)malloc(req->body_length + 1);
        if (req->body) {
            strcpy(req->body, body);
        }
    } else {
        req->body = NULL;
        req->body_length = 0;
    }
}

const char* http_request_get_header(const HttpRequest *req, const char *name) {
    if (!req || !name) return NULL;
    
    for (int i = 0; i < req->header_count; i++) {
        if (strcasecmp(req->headers[i].name, name) == 0) {
            return req->headers[i].value;
        }
    }
    
    return NULL;
}

HttpRequest* http_request_parse(const char *raw) {
    if (!raw) return NULL;
    
    HttpRequest *req = (HttpRequest*)calloc(1, sizeof(HttpRequest));
    if (!req) return NULL;
    
    char *raw_copy = strdup(raw);
    if (!raw_copy) {
        free(req);
        return NULL;
    }
    
    char *line = strtok(raw_copy, "\r\n");
    if (!line) {
        free(raw_copy);
        free(req);
        return NULL;
    }
    
    // Парсинг стартовой строки: METHOD PATH VERSION
    char method_str[16] = {0};
    char path[256] = {0};
    char version[16] = {0};
    
    int parsed = sscanf(line, "%15s %255s %15s", method_str, path, version);
    if (parsed >= 2) {
        req->method = http_method_from_string(method_str);
        strncpy(req->path, path, sizeof(req->path) - 1);
        
        if (parsed == 3) {
            strncpy(req->version, version, sizeof(req->version) - 1);
        } else {
            strcpy(req->version, "HTTP/0.9");
        }
    } else {
        free(raw_copy);
        free(req);
        return NULL;
    }
    
    // Парсинг заголовков
    req->header_count = 0;
    while ((line = strtok(NULL, "\r\n")) != NULL) {
        // Пустая строка означает конец заголовков
        if (strlen(line) == 0) {
            break;
        }
        
        // Парсинг заголовка: Name: Value
        char *colon = strchr(line, ':');
        if (colon && req->header_count < 50) {
            *colon = '\0';
            char *name = line;
            char *value = colon + 1;
            
            // Обрезаем пробелы
            name = trim_whitespace(name);
            value = trim_whitespace(value);
            
            HttpHeader *header = &req->headers[req->header_count];
            strncpy(header->name, name, sizeof(header->name) - 1);
            strncpy(header->value, value, sizeof(header->value) - 1);
            req->header_count++;
        }
    }
    
    // Тело запроса (оставшаяся часть)
    char *body_start = strtok(NULL, "\r\n");
    if (body_start) {
        // Вычисляем размер тела
        const char *body_pos = raw;
        const char *double_crlf = strstr(raw, "\r\n\r\n");
        if (double_crlf) {
            body_pos = double_crlf + 4;
            req->body_length = strlen(body_pos);
            req->body = (char*)malloc(req->body_length + 1);
            if (req->body) {
                strcpy(req->body, body_pos);
            }
        }
    }
    
    free(raw_copy);
    return req;
}

char* http_request_to_string(const HttpRequest *req) {
    if (!req) return NULL;
    
    // Оцениваем размер буфера
    size_t buffer_size = 1024; // Начальный размер
    char *buffer = (char*)malloc(buffer_size);
    if (!buffer) return NULL;
    
    int pos = 0;
    
    // Стартовая строка
    pos += snprintf(buffer + pos, buffer_size - pos, "%s %s %s\r\n",
                    http_method_to_string(req->method),
                    req->path,
                    req->version);
    
    // Заголовки
    for (int i = 0; i < req->header_count; i++) {
        pos += snprintf(buffer + pos, buffer_size - pos, "%s: %s\r\n",
                        req->headers[i].name,
                        req->headers[i].value);
    }
    
    // Пустая строка между заголовками и телом
    pos += snprintf(buffer + pos, buffer_size - pos, "\r\n");
    
    // Тело
    if (req->body && req->body_length > 0) {
        if (pos + req->body_length + 1 > buffer_size) {
            buffer_size = pos + req->body_length + 1024;
            buffer = (char*)realloc(buffer, buffer_size);
            if (!buffer) return NULL;
        }
        memcpy(buffer + pos, req->body, req->body_length);
        pos += req->body_length;
    }
    
    return buffer;
}

void http_request_free(HttpRequest *req) {
    if (req) {
        if (req->body) {
            free(req->body);
        }
        free(req);
    }
}