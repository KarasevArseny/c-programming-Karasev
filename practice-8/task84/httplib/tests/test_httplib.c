#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "http.h"

void test_method_conversion() {
    printf("Тестирование преобразования методов...\n");
    
    assert(strcmp(http_method_to_string(HTTP_GET), "GET") == 0);
    assert(strcmp(http_method_to_string(HTTP_POST), "POST") == 0);
    assert(strcmp(http_method_to_string(HTTP_PUT), "PUT") == 0);
    assert(strcmp(http_method_to_string(HTTP_DELETE), "DELETE") == 0);
    assert(strcmp(http_method_to_string(HTTP_HEAD), "HEAD") == 0);
    assert(strcmp(http_method_to_string(HTTP_OPTIONS), "OPTIONS") == 0);
    
    assert(http_method_from_string("GET") == HTTP_GET);
    assert(http_method_from_string("POST") == HTTP_POST);
    assert(http_method_from_string("PUT") == HTTP_PUT);
    assert(http_method_from_string("DELETE") == HTTP_DELETE);
    assert(http_method_from_string("HEAD") == HTTP_HEAD);
    assert(http_method_from_string("OPTIONS") == HTTP_OPTIONS);
    
    printf("✓ Методы работают корректно\n");
}

void test_request_parsing() {
    printf("Тестирование парсинга запросов...\n");
    
    const char *raw = 
        "GET /index.html HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: test-agent\r\n"
        "\r\n";
    
    HttpRequest *req = http_request_parse(raw);
    assert(req != NULL);
    assert(req->method == HTTP_GET);
    assert(strcmp(req->path, "/index.html") == 0);
    assert(strcmp(req->version, "HTTP/1.1") == 0);
    assert(req->header_count == 2);
    assert(strcmp(req->headers[0].name, "Host") == 0);
    assert(strcmp(req->headers[0].value, "example.com") == 0);
    assert(req->body == NULL);
    
    http_request_free(req);
    
    // Тест с телом
    const char *raw_post = 
        "POST /api HTTP/1.1\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 20\r\n"
        "\r\n"
        "{\"test\":\"value\"}";
    
    req = http_request_parse(raw_post);
    assert(req != NULL);
    assert(req->method == HTTP_POST);
    assert(req->body != NULL);
    assert(strstr(req->body, "test") != NULL);
    
    http_request_free(req);
    
    printf("✓ Парсинг запросов работает\n");
}

void test_response_parsing() {
    printf("Тестирование парсинга ответов...\n");
    
    const char *raw = 
        "HTTP/1.1 200 OK\r\n"
        "Server: test-server\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "\r\n"
        "Hello World!";
    
    HttpResponse *resp = http_response_parse(raw);
    assert(resp != NULL);
    assert(resp->status_code == 200);
    assert(strcmp(resp->status_text, "OK") == 0);
    assert(resp->header_count == 3);
    assert(strcmp(resp->headers[0].name, "Server") == 0);
    assert(strcmp(resp->headers[0].value, "test-server") == 0);
    assert(resp->body != NULL);
    assert(strcmp(resp->body, "Hello World!") == 0);
    
    http_response_free(resp);
    
    printf("✓ Парсинг ответов работает\n");
}

void test_request_creation() {
    printf("Тестирование создания запросов...\n");
    
    HttpRequest *req = http_request_create(HTTP_GET, "/test");
    http_request_add_header(req, "X-Custom", "test-value");
    http_request_set_body(req, "body content");
    
    char *str = http_request_to_string(req);
    assert(str != NULL);
    assert(strstr(str, "GET /test HTTP/1.1") != NULL);
    assert(strstr(str, "X-Custom: test-value") != NULL);
    assert(strstr(str, "body content") != NULL);
    
    free(str);
    http_request_free(req);
    
    printf("✓ Создание запросов работает\n");
}

void test_response_creation() {
    printf("Тестирование создания ответов...\n");
    
    HttpResponse *resp = http_response_create(404, NULL);
    http_response_add_header(resp, "X-Test", "value");
    http_response_set_body(resp, "Not Found");
    
    char *str = http_response_to_string(resp);
    assert(str != NULL);
    assert(strstr(str, "HTTP/1.1 404 Not Found") != NULL);
    assert(strstr(str, "X-Test: value") != NULL);
    assert(strstr(str, "Not Found") != NULL);
    
    free(str);
    http_response_free(resp);
    
    printf("✓ Создание ответов работает\n");
}

void test_header_access() {
    printf("Тестирование доступа к заголовкам...\n");
    
    HttpRequest *req = http_request_create(HTTP_GET, "/");
    http_request_add_header(req, "Content-Type", "application/json");
    http_request_add_header(req, "Accept", "text/html");
    
    const char *ct = http_request_get_header(req, "Content-Type");
    assert(ct != NULL && strcmp(ct, "application/json") == 0);
    
    const char *accept = http_request_get_header(req, "ACCEPT"); // case-insensitive
    assert(accept != NULL && strcmp(accept, "text/html") == 0);
    
    const char *none = http_request_get_header(req, "Nonexistent");
    assert(none == NULL);
    
    http_request_free(req);
    
    printf("✓ Доступ к заголовкам работает\n");
}

int main() {
    printf("========================================\n");
    printf("   ТЕСТИРОВАНИЕ HTTP БИБЛИОТЕКИ\n");
    printf("========================================\n\n");
    
    test_method_conversion();
    test_request_parsing();
    test_response_parsing();
    test_request_creation();
    test_response_creation();
    test_header_access();
    
    printf("\n✅ Все тесты пройдены успешно!\n");
    
    return 0;
}