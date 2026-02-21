#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int db_init(sqlite3 **db) {
    if (sqlite3_open(DB_FILE, db) != SQLITE_OK) {
        fprintf(stderr, "❌ Ошибка открытия БД: %s\n", sqlite3_errmsg(*db));
        return 0;
    }
    
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS weather ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " city TEXT NOT NULL,"
        " country TEXT,"
        " temperature REAL,"
        " feels_like REAL,"
        " humidity INTEGER,"
        " wind_speed REAL,"
        " condition TEXT,"
        " description TEXT,"
        " timestamp INTEGER"
        ");";
    
    char *err_msg = NULL;
    int rc = sqlite3_exec(*db, sql, NULL, NULL, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "❌ SQL ошибка: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        return 0;
    }
    
    return 1;
}

int db_save_weather(sqlite3 *db, const WeatherData *weather) {
    sqlite3_stmt *stmt;
    const char *sql = 
        "INSERT INTO weather (city, country, temperature, feels_like, "
        "humidity, wind_speed, condition, description, timestamp) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "❌ Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, weather->city, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, weather->country, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, weather->temperature);
    sqlite3_bind_double(stmt, 4, weather->feels_like);
    sqlite3_bind_int(stmt, 5, weather->humidity);
    sqlite3_bind_double(stmt, 6, weather->wind_speed);
    sqlite3_bind_text(stmt, 7, weather->condition, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, weather->description, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 9, weather->timestamp);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "❌ Ошибка сохранения: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    return 1;
}

int db_get_history(sqlite3 *db, WeatherData *results, int max_results, int *count) {
    sqlite3_stmt *stmt;
    const char *sql = 
        "SELECT id, city, country, temperature, feels_like, "
        "humidity, wind_speed, condition, description, timestamp "
        "FROM weather ORDER BY timestamp DESC LIMIT ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "❌ Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, max_results);
    
    *count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && *count < max_results) {
        WeatherData *w = &results[*count];
        
        w->id = sqlite3_column_int(stmt, 0);
        strncpy(w->city, (const char*)sqlite3_column_text(stmt, 1), sizeof(w->city) - 1);
        strncpy(w->country, (const char*)sqlite3_column_text(stmt, 2), sizeof(w->country) - 1);
        w->temperature = sqlite3_column_double(stmt, 3);
        w->feels_like = sqlite3_column_double(stmt, 4);
        w->humidity = sqlite3_column_int(stmt, 5);
        w->wind_speed = sqlite3_column_double(stmt, 6);
        strncpy(w->condition, (const char*)sqlite3_column_text(stmt, 7), sizeof(w->condition) - 1);
        strncpy(w->description, (const char*)sqlite3_column_text(stmt, 8), sizeof(w->description) - 1);
        w->timestamp = sqlite3_column_int(stmt, 9);
        
        (*count)++;
    }
    
    sqlite3_finalize(stmt);
    return 1;
}

int db_get_last(sqlite3 *db, WeatherData *result) {
    sqlite3_stmt *stmt;
    const char *sql = 
        "SELECT id, city, country, temperature, feels_like, "
        "humidity, wind_speed, condition, description, timestamp "
        "FROM weather ORDER BY timestamp DESC LIMIT 1;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "❌ Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    int rc = sqlite3_step(stmt);
    
    if (rc == SQLITE_ROW) {
        result->id = sqlite3_column_int(stmt, 0);
        strncpy(result->city, (const char*)sqlite3_column_text(stmt, 1), sizeof(result->city) - 1);
        strncpy(result->country, (const char*)sqlite3_column_text(stmt, 2), sizeof(result->country) - 1);
        result->temperature = sqlite3_column_double(stmt, 3);
        result->feels_like = sqlite3_column_double(stmt, 4);
        result->humidity = sqlite3_column_int(stmt, 5);
        result->wind_speed = sqlite3_column_double(stmt, 6);
        strncpy(result->condition, (const char*)sqlite3_column_text(stmt, 7), sizeof(result->condition) - 1);
        strncpy(result->description, (const char*)sqlite3_column_text(stmt, 8), sizeof(result->description) - 1);
        result->timestamp = sqlite3_column_int(stmt, 9);
        
        sqlite3_finalize(stmt);
        return 1;
    }
    
    sqlite3_finalize(stmt);
    return 0;
}

int db_cleanup_old(sqlite3 *db, int days) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM weather WHERE timestamp < ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "❌ Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    time_t cutoff = time(NULL) - days * 24 * 60 * 60;
    sqlite3_bind_int(stmt, 1, cutoff);
    
    int rc = sqlite3_step(stmt);
    int changes = sqlite3_changes(db);
    
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "❌ Ошибка очистки: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    printf("✅ Удалено записей: %d\n", changes);
    return 1;
}