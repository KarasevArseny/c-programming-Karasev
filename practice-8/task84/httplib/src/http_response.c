#include "http_response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Внутренняя функция для обрезки пробелов
static char* trim_whitespace(char *str) {
    if (!str) return NULL;
    
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == 0) return str;
    
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    end[1] = '\0';
    
    return str;
}

const char* http_status_text(int status_code) {
    switch (status_code) {
        case 100: return "Continue";
        case 101: return "Switching Protocols";
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 204: return "No Content";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 304: return "Not Modified";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 408: return "Request Timeout";
        case 418: return "I'm a teapot";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Timeout";
        default:  return "Unknown Status";
    }
}

HttpResponse* http_response_create(int status_code, const char *status_text) {
    HttpResponse *resp = (HttpResponse*)calloc(1, sizeof(HttpResponse));
    if (!resp) return NULL;
    
    strcpy(resp->version, "HTTP/1.1");
    resp->status_code = status_code;
    
    if (status_text) {
        strncpy(resp->status_text, status_text, sizeof(resp->status_text) - 1);
    } else {
        const char *text = http_status_text(status_code);
        strncpy(resp->status_text, text, sizeof(resp->status_text) - 1);
    }
    
    resp->header_count = 0;
    resp->body = NULL;
    resp->body_length = 0;
    
    return resp;
}

void http_response_add_header(HttpResponse *resp, const char *name, const char *value) {
    if (!resp || resp->header_count >= 50) return;
    
    HttpHeader *header = &resp->headers[resp->header_count];
    strncpy(header->name, name, sizeof(header->name) - 1);
    strncpy(header->value, value, sizeof(header->value) - 1);
    resp->header_count++;
}

void http_response_set_body(HttpResponse *resp, const char *body) {
    if (!resp) return;
    
    if (resp->body) {
        free(resp->body);
    }
    
    if (body) {
        resp->body_length = strlen(body);
        resp->body = (char*)malloc(resp->body_length + 1);
        if (resp->body) {
            strcpy(resp->body, body);
        }
        
        // Добавляем заголовок Content-Length, если его нет
        if (!http_response_get_header(resp, "Content-Length")) {
            char len_str[32];
            snprintf(len_str, sizeof(len_str), "%zu", resp->body_length);
            http_response_add_header(resp, "Content-Length", len_str);
        }
    } else {
        resp->body = NULL;
        resp->body_length = 0;
    }
}

const char* http_response_get_header(const HttpResponse *resp, const char *name) {
    if (!resp || !name) return NULL;
    
    for (int i = 0; i < resp->header_count; i++) {
        if (strcasecmp(resp->headers[i].name, name) == 0) {
            return resp->headers[i].value;
        }
    }
    
    return NULL;
}

HttpResponse* http_response_parse(const char *raw) {
    if (!raw) return NULL;
    
    HttpResponse *resp = (HttpResponse*)calloc(1, sizeof(HttpResponse));
    if (!resp) return NULL;
    
    char *raw_copy = strdup(raw);
    if (!raw_copy) {
        free(resp);
        return NULL;
    }
    
    char *line = strtok(raw_copy, "\r\n");
    if (!line) {
        free(raw_copy);
        free(resp);
        return NULL;
    }
    
    // Парсинг стартовой строки: HTTP/1.1 200 OK
    char version[16] = {0};
    int status_code = 0;
    char status_text[64] = {0};
    
    int parsed = sscanf(line, "%15s %d %63[^\r\n]", version, &status_code, status_text);
    if (parsed >= 2) {
        strncpy(resp->version, version, sizeof(resp->version) - 1);
        resp->status_code = status_code;
        
        if (parsed == 3) {
            strncpy(resp->status_text, status_text, sizeof(resp->status_text) - 1);
        } else {
            const char *text = http_status_text(status_code);
            strncpy(resp->status_text, text, sizeof(resp->status_text) - 1);
        }
    } else {
        free(raw_copy);
        free(resp);
        return NULL;
    }
    
    // Парсинг заголовков
    resp->header_count = 0;
    while ((line = strtok(NULL, "\r\n")) != NULL) {
        if (strlen(line) == 0) {
            break;
        }
        
        char *colon = strchr(line, ':');
        if (colon && resp->header_count < 50) {
            *colon = '\0';
            char *name = line;
            char *value = colon + 1;
            
            name = trim_whitespace(name);
            value = trim_whitespace(value);
            
            HttpHeader *header = &resp->headers[resp->header_count];
            strncpy(header->name, name, sizeof(header->name) - 1);
            strncpy(header->value, value, sizeof(header->value) - 1);
            resp->header_count++;
        }
    }
    
    // Тело ответа
    char *body_start = strtok(NULL, "\r\n");
    if (body_start) {
        const char *body_pos = raw;
        const char *double_crlf = strstr(raw, "\r\n\r\n");
        if (double_crlf) {
            body_pos = double_crlf + 4;
            resp->body_length = strlen(body_pos);
            resp->body = (char*)malloc(resp->body_length + 1);
            if (resp->body) {
                strcpy(resp->body, body_pos);
            }
        }
    }
    
    free(raw_copy);
    return resp;
}

char* http_response_to_string(const HttpResponse *resp) {
    if (!resp) return NULL;
    
    size_t buffer_size = 1024;
    char *buffer = (char*)malloc(buffer_size);
    if (!buffer) return NULL;
    
    int pos = 0;
    
    // Стартовая строка
    pos += snprintf(buffer + pos, buffer_size - pos, "%s %d %s\r\n",
                    resp->version,
                    resp->status_code,
                    resp->status_text);
    
    // Заголовки
    for (int i = 0; i < resp->header_count; i++) {
        pos += snprintf(buffer + pos, buffer_size - pos, "%s: %s\r\n",
                        resp->headers[i].name,
                        resp->headers[i].value);
    }
    
    // Пустая строка
    pos += snprintf(buffer + pos, buffer_size - pos, "\r\n");
    
    // Тело
    if (resp->body && resp->body_length > 0) {
        if (pos + resp->body_length + 1 > buffer_size) {
            buffer_size = pos + resp->body_length + 1024;
            buffer = (char*)realloc(buffer, buffer_size);
            if (!buffer) return NULL;
        }
        memcpy(buffer + pos, resp->body, resp->body_length);
        pos += resp->body_length;
    }
    
    return buffer;
}

void http_response_free(HttpResponse *resp) {
    if (resp) {
        if (resp->body) {
            free(resp->body);
        }
        free(resp);
    }
}