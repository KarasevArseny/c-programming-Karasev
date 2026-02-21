#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "weather.h"
#include "http.h"
#include "db.h"

void print_usage(const char *prog_name) {
    printf("🌤️  Прогноз погоды\n");
    printf("==================\n\n");
    printf("Использование:\n");
    printf("  %s fetch <город>        - получить погоду для города\n", prog_name);
    printf("  %s history              - показать историю запросов\n", prog_name);
    printf("  %s last                 - показать последний запрос\n", prog_name);
    printf("  %s cleanup <дни>        - очистить записи старше N дней\n", prog_name);
    printf("\nПримеры:\n");
    printf("  %s fetch Moscow\n", prog_name);
    printf("  %s fetch \"New York\"\n", prog_name);
    printf("  %s history\n", prog_name);
    printf("  %s last\n", prog_name);
    printf("  %s cleanup 7\n", prog_name);
}

int parse_city_name(int argc, char *argv[], char *city, size_t city_size) {
    if (argc < 3) {
        return 0;
    }
    
    city[0] = '\0';
    for (int i = 2; i < argc; i++) {
        if (i > 2) strncat(city, " ", city_size - strlen(city) - 1);
        strncat(city, argv[i], city_size - strlen(city) - 1);
    }
    
    return 1;
}

// Парсинг JSON-ответа от wttr.in
int parse_weather_json(const char *json_str, WeatherData *weather) {
    // Упрощённый парсинг для демонстрации
    // В реальном проекте здесь должен быть полноценный парсинг cJSON
    
    // Ищем температуру
    const char *temp_marker = "\"temp_C\":\"";
    const char *temp_pos = strstr(json_str, temp_marker);
    if (temp_pos) {
        temp_pos += strlen(temp_marker);
        weather->temperature = atof(temp_pos);
    }
    
    // Ищем влажность
    const char *humidity_marker = "\"humidity\":\"";
    const char *humidity_pos = strstr(json_str, humidity_marker);
    if (humidity_pos) {
        humidity_pos += strlen(humidity_marker);
        weather->humidity = atoi(humidity_pos);
    }
    
    // Ищем скорость ветра
    const char *wind_marker = "\"windspeedKmph\":\"";
    const char *wind_pos = strstr(json_str, wind_marker);
    if (wind_pos) {
        wind_pos += strlen(wind_marker);
        weather->wind_speed = atof(wind_pos) / 3.6; // Преобразуем км/ч в м/с
    }
    
    // Ищем описание
    const char *desc_marker = "\"weatherDesc\":[{\"value\":\"";
    const char *desc_pos = strstr(json_str, desc_marker);
    if (desc_pos) {
        desc_pos += strlen(desc_marker);
        const char *desc_end = strchr(desc_pos, '"');
        if (desc_end) {
            int len = desc_end - desc_pos;
            if (len > 0 && len < 200) {
                strncpy(weather->description, desc_pos, len);
                weather->description[len] = '\0';
            }
        }
    }
    
    weather->timestamp = time(NULL);
    
    return 1;
}

void print_weather(const WeatherData *w) {
    char time_str[64];
    struct tm *tm_info = localtime(&w->timestamp);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    
    printf("\n🌍 Город: %s, %s\n", w->city, w->country);
    printf("📅 Время: %s\n", time_str);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("🌡️  Температура:     %.1f°C\n", w->temperature);
    printf("🤔 Ощущается как:   %.1f°C\n", w->feels_like);
    printf("💧 Влажность:       %d%%\n", w->humidity);
    printf("💨 Ветер:           %.1f м/с\n", w->wind_speed);
    printf("☁️  Состояние:       %s\n", w->condition);
    printf("📝 Описание:        %s\n", w->description);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

int cmd_fetch(sqlite3 *db, const char *city) {
    printf("🔍 Запрос погоды для города: %s\n", city);
    
    // Формируем URL
    char url[256];
    http_build_weather_url(city, url, sizeof(url));
    printf("🌐 URL: %s\n", url);
    
    // Выполняем HTTP-запрос
    HttpResponse resp;
    if (!http_get(url, &resp)) {
        fprintf(stderr, "❌ Ошибка получения данных\n");
        return 0;
    }
    
    // Парсим JSON
    WeatherData weather;
    memset(&weather, 0, sizeof(WeatherData));
    strncpy(weather.city, city, sizeof(weather.city) - 1);
    strcpy(weather.country, "RU"); // Для демонстрации
    
    if (!parse_weather_json(resp.data, &weather)) {
        fprintf(stderr, "❌ Ошибка парсинга данных\n");
        http_response_free(&resp);
        return 0;
    }
    
    http_response_free(&resp);
    
    // Выводим результат
    print_weather(&weather);
    
    // Сохраняем в базу данных
    if (db_save_weather(db, &weather)) {
        printf("✅ Данные сохранены в базу\n");
    } else {
        fprintf(stderr, "❌ Ошибка сохранения в базу\n");
    }
    
    return 1;
}

int cmd_history(sqlite3 *db) {
    WeatherData history[100];
    int count = 0;
    
    if (!db_get_history(db, history, 100, &count)) {
        fprintf(stderr, "❌ Ошибка получения истории\n");
        return 0;
    }
    
    if (count == 0) {
        printf("📭 История запросов пуста\n");
        return 1;
    }
    
    printf("\n📋 ИСТОРИЯ ЗАПРОСОВ\n");
    printf("═══════════════════════════════════════════════════════════════════\n");
    printf("%-4s %-20s %-10s %-10s %-8s %-30s\n", 
           "ID", "Город", "Температура", "Влажность", "Ветер", "Время");
    printf("──── ──────────────────── ────────── ────────── ──────── ──────────────────────────────\n");
    
    for (int i = 0; i < count; i++) {
        char time_str[20];
        struct tm *tm_info = localtime(&history[i].timestamp);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", tm_info);
        
        printf("%-4d %-20s %-10.1f %-10d %-8.1f %s\n",
               history[i].id,
               history[i].city,
               history[i].temperature,
               history[i].humidity,
               history[i].wind_speed,
               time_str);
    }
    printf("\n📊 Всего записей: %d\n", count);
    
    return 1;
}

int cmd_last(sqlite3 *db) {
    WeatherData last;
    
    if (!db_get_last(db, &last)) {
        fprintf(stderr, "❌ Нет записей в базе данных\n");
        return 0;
    }
    
    printf("\n🕒 ПОСЛЕДНИЙ ЗАПРОС\n");
    print_weather(&last);
    
    return 1;
}

int cmd_cleanup(sqlite3 *db, int days) {
    if (days <= 0) {
        fprintf(stderr, "❌ Укажите положительное количество дней\n");
        return 0;
    }
    
    if (db_cleanup_old(db, days)) {
        printf("✅ Очищены записи старше %d дней\n", days);
    } else {
        fprintf(stderr, "❌ Ошибка при очистке\n");
        return 0;
    }
    
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Инициализация HTTP
    http_global_init();
    
    // Открываем базу данных
    sqlite3 *db;
    if (!db_init(&db)) {
        http_global_cleanup();
        return 1;
    }
    
    int result = 1;
    
    if (strcmp(argv[1], "fetch") == 0) {
        char city[256] = "";
        if (!parse_city_name(argc, argv, city, sizeof(city))) {
            fprintf(stderr, "❌ Укажите название города\n");
            print_usage(argv[0]);
            result = 0;
        } else {
            result = cmd_fetch(db, city);
        }
    }
    else if (strcmp(argv[1], "history") == 0) {
        result = cmd_history(db);
    }
    else if (strcmp(argv[1], "last") == 0) {
        result = cmd_last(db);
    }
    else if (strcmp(argv[1], "cleanup") == 0) {
        if (argc < 3) {
            fprintf(stderr, "❌ Укажите количество дней\n");
            print_usage(argv[0]);
            result = 0;
        } else {
            int days = atoi(argv[2]);
            result = cmd_cleanup(db, days);
        }
    }
    else {
        fprintf(stderr, "❌ Неизвестная команда: %s\n", argv[1]);
        print_usage(argv[0]);
        result = 0;
    }
    
    // Закрываем базу данных
    sqlite3_close(db);
    
    // Очистка HTTP
    http_global_cleanup();
    
    return result ? 0 : 1;
}