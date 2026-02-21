#include <stdio.h>
#include <string.h>
#include "http.h"

void print_separator() {
    printf("--------------------------------------------------\n");
}

void demo_request_parsing() {
    printf("\n=== ПАРСИНГ HTTP ЗАПРОСА ===\n");
    print_separator();
    
    const char *raw_request = 
        "POST /api/users HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 27\r\n"
        "\r\n"
        "{\"name\":\"John\",\"age\":30}";
    
    printf("Исходный запрос:\n%s\n\n", raw_request);
    
    HttpRequest *req = http_request_parse(raw_request);
    if (req) {
        printf("Результат парсинга:\n");
        printf("  Метод: %s\n", http_method_to_string(req->method));
        printf("  Путь: %s\n", req->path);
        printf("  Версия: %s\n", req->version);
        printf("  Заголовков: %d\n", req->header_count);
        
        for (int i = 0; i < req->header_count; i++) {
            printf("    %s: %s\n", req->headers[i].name, req->headers[i].value);
        }
        
        printf("  Тело (%zu байт): %s\n", req->body_length, req->body ? req->body : "(пусто)");
        
        // Получение конкретного заголовка
        const char *content_type = http_request_get_header(req, "Content-Type");
        printf("\n  Content-Type: %s\n", content_type ? content_type : "не найден");
        
        http_request_free(req);
    }
}

void demo_response_parsing() {
    printf("\n=== ПАРСИНГ HTTP ОТВЕТА ===\n");
    print_separator();
    
    const char *raw_response = 
        "HTTP/1.1 200 OK\r\n"
        "Server: nginx/1.18.0\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 42\r\n"
        "\r\n"
        "{\"id\":123,\"name\":\"John\",\"status\":\"active\"}";
    
    printf("Исходный ответ:\n%s\n\n", raw_response);
    
    HttpResponse *resp = http_response_parse(raw_response);
    if (resp) {
        printf("Результат парсинга:\n");
        printf("  Версия: %s\n", resp->version);
        printf("  Статус: %d %s\n", resp->status_code, resp->status_text);
        printf("  Заголовков: %d\n", resp->header_count);
        
        for (int i = 0; i < resp->header_count; i++) {
            printf("    %s: %s\n", resp->headers[i].name, resp->headers[i].value);
        }
        
        printf("  Тело (%zu байт): %s\n", resp->body_length, resp->body ? resp->body : "(пусто)");
        
        // Получение конкретного заголовка
        const char *server = http_response_get_header(resp, "Server");
        printf("\n  Server: %s\n", server ? server : "не найден");
        
        http_response_free(resp);
    }
}

void demo_request_creation() {
    printf("\n=== СОЗДАНИЕ HTTP ЗАПРОСА ===\n");
    print_separator();
    
    HttpRequest *req = http_request_create(HTTP_POST, "/api/users");
    
    http_request_add_header(req, "Host", "example.com");
    http_request_add_header(req, "User-Agent", "HTTPLib/1.0");
    http_request_add_header(req, "Content-Type", "application/json");
    http_request_set_body(req, "{\"name\":\"Alice\",\"age\":25}");
    
    printf("Созданный запрос:\n");
    char *req_str = http_request_to_string(req);
    if (req_str) {
        printf("%s", req_str);
        free(req_str);
    }
    
    http_request_free(req);
}

void demo_response_creation() {
    printf("\n=== СОЗДАНИЕ HTTP ОТВЕТА ===\n");
    print_separator();
    
    HttpResponse *resp = http_response_create(200, "OK");
    
    http_response_add_header(resp, "Server", "HTTPLib/1.0");
    http_response_add_header(resp, "Content-Type", "text/html");
    http_response_set_body(resp, "<html><body><h1>Hello World!</h1></body></html>");
    
    printf("Созданный ответ:\n");
    char *resp_str = http_response_to_string(resp);
    if (resp_str) {
        printf("%s", resp_str);
        free(resp_str);
    }
    
    http_response_free(resp);
}

void demo_error_responses() {
    printf("\n=== РАЗЛИЧНЫЕ КОДЫ ОТВЕТА ===\n");
    print_separator();
    
    int codes[] = {200, 201, 204, 301, 400, 403, 404, 418, 500, 502};
    
    for (size_t i = 0; i < sizeof(codes)/sizeof(codes[0]); i++) {
        int code = codes[i];
        const char *text = http_status_text(code);
        
        HttpResponse *resp = http_response_create(code, NULL);
        http_response_add_header(resp, "X-Test", "true");
        http_response_set_body(resp, "Test body");
        
        char *resp_str = http_response_to_string(resp);
        printf("%d %s\n", code, text);
        printf("  -> %s\n", resp_str);
        
        free(resp_str);
        http_response_free(resp);
    }
}

int main() {
    printf("========================================\n");
    printf("   ПРОСТОЙ HTTP ПАРСЕР\n");
    printf("========================================\n");
    
    demo_request_parsing();
    demo_response_parsing();
    demo_request_creation();
    demo_response_creation();
    demo_error_responses();
    
    printf("\n========================================\n");
    printf("   ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА\n");
    printf("========================================\n");
    
    return 0;
}