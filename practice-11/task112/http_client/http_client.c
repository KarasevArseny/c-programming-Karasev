#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Структура для хранения ответа
typedef struct {
    char *data;
    size_t size;
} Response;

// Callback для записи данных
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total = size * nmemb;
    Response *resp = (Response*)userdata;
    
    char *new_data = realloc(resp->data, resp->size + total + 1);
    if (!new_data) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 0;
    }
    
    resp->data = new_data;
    memcpy(resp->data + resp->size, ptr, total);
    resp->size += total;
    resp->data[resp->size] = '\0';
    
    return total;
}

// GET запрос
int http_get(const char *url, Response *resp, struct curl_slist *headers) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Ошибка инициализации CURL\n");
        return 0;
    }
    
    // Инициализация структуры ответа
    resp->data = malloc(1);
    if (!resp->data) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        curl_easy_cleanup(curl);
        return 0;
    }
    resp->data[0] = '\0';
    resp->size = 0;
    
    // Настройка CURL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    
    // Добавление заголовков, если есть
    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    
    // Выполнение запроса
    CURLcode res = curl_easy_perform(curl);
    
    // Получение HTTP кода ответа
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "Ошибка HTTP запроса: %s\n", curl_easy_strerror(res));
        free(resp->data);
        resp->data = NULL;
        resp->size = 0;
        return 0;
    }
    
    printf("HTTP %ld\n", http_code);
    return 1;
}

// POST запрос
int http_post(const char *url, const char *data, Response *resp, struct curl_slist *headers) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Ошибка инициализации CURL\n");
        return 0;
    }
    
    // Инициализация структуры ответа
    resp->data = malloc(1);
    if (!resp->data) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        curl_easy_cleanup(curl);
        return 0;
    }
    resp->data[0] = '\0';
    resp->size = 0;
    
    // Базовые заголовки
    struct curl_slist *default_headers = NULL;
    default_headers = curl_slist_append(default_headers, "Content-Type: application/json");
    default_headers = curl_slist_append(default_headers, "Accept: application/json");
    
    // Добавляем пользовательские заголовки
    if (headers) {
        struct curl_slist *temp = headers;
        while (temp) {
            default_headers = curl_slist_append(default_headers, temp->data);
            temp = temp->next;
        }
    }
    
    // Настройка CURL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, default_headers);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    
    // Выполнение запроса
    CURLcode res = curl_easy_perform(curl);
    
    // Получение HTTP кода ответа
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    curl_slist_free_all(default_headers);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "Ошибка HTTP запроса: %s\n", curl_easy_strerror(res));
        free(resp->data);
        resp->data = NULL;
        resp->size = 0;
        return 0;
    }
    
    printf("HTTP %ld\n", http_code);
    return 1;
}

// Функция для добавления заголовка
struct curl_slist* add_header(struct curl_slist *headers, const char *header) {
    return curl_slist_append(headers, header);
}

// Сохранение ответа в файл
int save_to_file(const char *filename, const char *data, size_t size) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Ошибка создания файла: %s\n", filename);
        return 0;
    }
    
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    
    if (written != size) {
        fprintf(stderr, "Ошибка записи в файл\n");
        return 0;
    }
    
    return 1;
}

void print_usage(const char *prog_name) {
    printf("HTTP Клиент v1.0\n");
    printf("=================\n\n");
    printf("Использование:\n");
    printf("  %s get <url>                    - GET запрос\n", prog_name);
    printf("  %s get <url> -H 'Header: Value' - GET запрос с заголовком\n", prog_name);
    printf("  %s get <url> -o <file>          - GET запрос с сохранением в файл\n", prog_name);
    printf("  %s post <url> <json>            - POST запрос с JSON\n", prog_name);
    printf("  %s post <url> <json> -H 'Header: Value' - POST с заголовком\n", prog_name);
    printf("  %s post <url> <json> -o <file>  - POST с сохранением в файл\n", prog_name);
    printf("\nПримеры:\n");
    printf("  %s get https://httpbin.org/get\n", prog_name);
    printf("  %s get https://api.github.com -H 'User-Agent: MyClient/1.0'\n", prog_name);
    printf("  %s get https://httpbin.org/json -o response.json\n", prog_name);
    printf("  %s post https://httpbin.org/post '{\"name\":\"Alice\",\"age\":25}'\n", prog_name);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Инициализация libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    Response resp = {NULL, 0};
    int success = 0;
    const char *output_file = NULL;
    struct curl_slist *headers = NULL;
    
    // Парсинг аргументов для GET запроса
    if (strcmp(argv[1], "get") == 0) {
        const char *url = argv[2];
        
        // Поиск опций (-H для заголовков, -o для выходного файла)
        for (int i = 3; i < argc; i++) {
            if (strcmp(argv[i], "-H") == 0 && i + 1 < argc) {
                headers = add_header(headers, argv[i + 1]);
                i++; // Пропускаем значение заголовка
            } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                output_file = argv[i + 1];
                i++; // Пропускаем имя файла
            }
        }
        
        success = http_get(url, &resp, headers);
        
        // Освобождаем заголовки, если были
        if (headers) {
            curl_slist_free_all(headers);
        }
    }
    // Парсинг аргументов для POST запроса
    else if (strcmp(argv[1], "post") == 0 && argc >= 4) {
        const char *url = argv[2];
        const char *post_data = argv[3];
        
        // Поиск опций
        for (int i = 4; i < argc; i++) {
            if (strcmp(argv[i], "-H") == 0 && i + 1 < argc) {
                headers = add_header(headers, argv[i + 1]);
                i++; // Пропускаем значение заголовка
            } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                output_file = argv[i + 1];
                i++; // Пропускаем имя файла
            }
        }
        
        success = http_post(url, post_data, &resp, headers);
        
        if (headers) {
            curl_slist_free_all(headers);
        }
    } else {
        print_usage(argv[0]);
        curl_global_cleanup();
        return 1;
    }
    
    // Обработка результата
    if (success && resp.data) {
        if (output_file) {
            // Сохраняем в файл
            if (save_to_file(output_file, resp.data, resp.size)) {
                printf("✅ Ответ сохранён в %s (%zu байт)\n", output_file, resp.size);
            } else {
                fprintf(stderr, "❌ Ошибка сохранения в файл\n");
            }
        } else {
            // Выводим в консоль
            printf("\n%s\n", resp.data);
        }
    }
    
    // Очистка
    if (resp.data) {
        free(resp.data);
    }
    
    curl_global_cleanup();
    return success ? 0 : 1;
}