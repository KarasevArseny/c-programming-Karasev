#ifndef WEATHER_H
#define WEATHER_H

#include <time.h>

// Структура для хранения данных о погоде
typedef struct {
    int id;
    char city[100];
    char country[10];
    double temperature;
    double feels_like;
    int humidity;
    double wind_speed;
    char condition[50];
    char description[200];
    time_t timestamp;
} WeatherData;

// Основные функции программы
void print_usage(const char *prog_name);
int parse_city_name(int argc, char *argv[], char *city, size_t city_size);

#endif // WEATHER_H