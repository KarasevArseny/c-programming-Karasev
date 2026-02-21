#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include "weather.h"

#define DB_FILE "weather.db"

// Инициализация базы данных
int db_init(sqlite3 **db);

// Сохранение данных о погоде
int db_save_weather(sqlite3 *db, const WeatherData *weather);

// Получение истории запросов
int db_get_history(sqlite3 *db, WeatherData *results, int max_results, int *count);

// Получение последней записи
int db_get_last(sqlite3 *db, WeatherData *result);

// Очистка старых записей (старше N дней)
int db_cleanup_old(sqlite3 *db, int days);

#endif // DB_H