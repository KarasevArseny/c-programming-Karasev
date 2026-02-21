#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total = size * nmemb;
    HttpResponse *resp = (HttpResponse*)userdata;
    
    char *new_data = realloc(resp->data, resp->size + total + 1);
    if (!new_data) {
        fprintf(stderr, "❌ Ошибка выделения памяти\n");
        return 0;
    }
    
    resp->data = new_data;
    memcpy(resp->data + resp->size, ptr, total);
    resp->size += total;
    resp->data[resp->size] = '\0';
    
    return total;
}

void http_global_init(void) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void http_global_cleanup(void) {
    curl_global_cleanup();
}

int http_get(const char *url, HttpResponse *resp) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "❌ Ошибка инициализации CURL\n");
        return 0;
    }
    
    resp->data = malloc(1);
    if (!resp->data) {
        fprintf(stderr, "❌ Ошибка выделения памяти\n");
        curl_easy_cleanup(curl);
        return 0;
    }
    resp->data[0] = '\0';
    resp->size = 0;
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "WeatherApp/1.0");
    
    CURLcode res = curl_easy_perform(curl);
    
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "❌ Ошибка HTTP запроса: %s\n", curl_easy_strerror(res));
        free(resp->data);
        resp->data = NULL;
        resp->size = 0;
        return 0;
    }
    
    if (http_code != 200) {
        fprintf(stderr, "❌ HTTP ошибка: %ld\n", http_code);
        free(resp->data);
        resp->data = NULL;
        resp->size = 0;
        return 0;
    }
    
    return 1;
}

void http_response_free(HttpResponse *resp) {
    if (resp && resp->data) {
        free(resp->data);
        resp->data = NULL;
        resp->size = 0;
    }
}

void http_build_weather_url(const char *city, char *url, size_t url_size) {
    // Кодируем пробелы в URL
    char encoded_city[256] = "";
    const char *src = city;
    char *dst = encoded_city;
    
    while (*src && dst - encoded_city < 255) {
        if (*src == ' ') {
            *dst++ = '%';
            *dst++ = '2';
            *dst++ = '0';
        } else {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
    
    snprintf(url, url_size, "https://wttr.in/%s?format=j1", encoded_city);
}