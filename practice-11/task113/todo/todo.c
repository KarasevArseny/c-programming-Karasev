#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define DB_FILE "todo.db"

// Инициализация базы данных
int init_db(sqlite3 *db) {
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS tasks ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT, "
        " title TEXT NOT NULL, "
        " done INTEGER DEFAULT 0, "
        " created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";
    
    char *err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    
    return 1;
}

// Добавить задачу
int add_task(sqlite3 *db, const char *title) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO tasks (title) VALUES (?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Ошибка добавления задачи: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    printf("✅ Задача добавлена (ID: %lld)\n", sqlite3_last_insert_rowid(db));
    return 1;
}

// Показать все задачи
int list_tasks(sqlite3 *db, int show_all) {
    sqlite3_stmt *stmt;
    const char *sql = show_all 
        ? "SELECT id, title, done, created_at FROM tasks ORDER BY done, id;"
        : "SELECT id, title, done, created_at FROM tasks WHERE done=0 ORDER BY id;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    printf("\n%-4s %-3s %-30s %s\n", "ID", "✓", "Задача", "Создано");
    printf("---- --- ------------------------------ -------------------------\n");
    
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *title = sqlite3_column_text(stmt, 1);
        int done = sqlite3_column_int(stmt, 2);
        const unsigned char *created = sqlite3_column_text(stmt, 3);
        
        printf("%-4d [%c] %-30s %s\n", 
               id, 
               done ? 'x' : ' ', 
               title ? (const char*)title : "", 
               created ? (const char*)created : "");
        count++;
    }
    
    sqlite3_finalize(stmt);
    printf("\n📊 Всего задач: %d\n", count);
    
    return 1;
}

// Отметить задачу выполненной
int complete_task(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE tasks SET done = 1 WHERE id = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE && sqlite3_changes(db) > 0) {
        printf("✅ Задача %d отмечена выполненной\n", id);
        return 1;
    }
    
    fprintf(stderr, "❌ Задача %d не найдена\n", id);
    return 0;
}

// Удалить задачу
int delete_task(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM tasks WHERE id = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc == SQLITE_DONE && sqlite3_changes(db) > 0) {
        printf("✅ Задача %d удалена\n", id);
        return 1;
    }
    
    fprintf(stderr, "❌ Задача %d не найдена\n", id);
    return 0;
}

// Очистить выполненные задачи
int clear_completed(sqlite3 *db) {
    const char *sql = "DELETE FROM tasks WHERE done = 1;";
    char *err_msg = NULL;
    
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка удаления выполненных задач: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    
    int changes = sqlite3_changes(db);
    if (changes > 0) {
        printf("✅ Удалено выполненных задач: %d\n", changes);
    } else {
        printf("ℹ️ Нет выполненных задач для удаления\n");
    }
    
    return 1;
}

// Показать статистику
int show_stats(sqlite3 *db) {
    sqlite3_stmt *stmt;
    
    // Общее количество задач
    const char *sql_total = "SELECT COUNT(*) FROM tasks;";
    // Количество выполненных задач
    const char *sql_done = "SELECT COUNT(*) FROM tasks WHERE done = 1;";
    
    int total = 0, done = 0;
    
    // Получаем общее количество
    if (sqlite3_prepare_v2(db, sql_total, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            total = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    
    // Получаем количество выполненных
    if (sqlite3_prepare_v2(db, sql_done, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            done = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    
    printf("\n📈 СТАТИСТИКА\n");
    printf("=============\n");
    printf("Всего задач: %d\n", total);
    printf("Выполнено:   %d (%.0f%%)\n", done, total ? (done * 100.0 / total) : 0);
    printf("Осталось:    %d\n", total - done);
    
    return 1;
}

void print_usage(const char *prog_name) {
    printf("📝 Менеджер задач (TODO) с SQLite\n");
    printf("==================================\n\n");
    printf("Использование:\n");
    printf("  %s list                 - показать невыполненные задачи\n", prog_name);
    printf("  %s list all             - показать все задачи\n", prog_name);
    printf("  %s add <название>        - добавить задачу\n", prog_name);
    printf("  %s done <id>             - отметить выполненной\n", prog_name);
    printf("  %s delete <id>           - удалить задачу\n", prog_name);
    printf("  %s clear                 - удалить все выполненные задачи\n", prog_name);
    printf("  %s stats                 - показать статистику\n", prog_name);
    printf("\nПримеры:\n");
    printf("  %s add \"Изучить cJSON\"\n", prog_name);
    printf("  %s add \"Написать HTTP-клиент\"\n", prog_name);
    printf("  %s list\n", prog_name);
    printf("  %s done 1\n", prog_name);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Открываем базу данных
    sqlite3 *db;
    if (sqlite3_open(DB_FILE, &db) != SQLITE_OK) {
        fprintf(stderr, "❌ Ошибка открытия БД: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    
    // Инициализируем базу данных (создаём таблицу, если не существует)
    if (!init_db(db)) {
        sqlite3_close(db);
        return 1;
    }
    
    // Обработка команд
    int result = 0;
    
    if (strcmp(argv[1], "list") == 0) {
        int show_all = (argc > 2 && strcmp(argv[2], "all") == 0);
        result = list_tasks(db, show_all);
    }
    else if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {
            fprintf(stderr, "❌ Укажите название задачи\n");
            print_usage(argv[0]);
            result = 0;
        } else {
            // Объединяем все аргументы после add в одну строку
            char title[512] = "";
            for (int i = 2; i < argc; i++) {
                if (i > 2) strcat(title, " ");
                strcat(title, argv[i]);
            }
            result = add_task(db, title);
        }
    }
    else if (strcmp(argv[1], "done") == 0) {
        if (argc < 3) {
            fprintf(stderr, "❌ Укажите ID задачи\n");
            print_usage(argv[0]);
            result = 0;
        } else {
            int id = atoi(argv[2]);
            if (id <= 0) {
                fprintf(stderr, "❌ Некорректный ID\n");
                result = 0;
            } else {
                result = complete_task(db, id);
            }
        }
    }
    else if (strcmp(argv[1], "delete") == 0) {
        if (argc < 3) {
            fprintf(stderr, "❌ Укажите ID задачи\n");
            print_usage(argv[0]);
            result = 0;
        } else {
            int id = atoi(argv[2]);
            if (id <= 0) {
                fprintf(stderr, "❌ Некорректный ID\n");
                result = 0;
            } else {
                result = delete_task(db, id);
            }
        }
    }
    else if (strcmp(argv[1], "clear") == 0) {
        result = clear_completed(db);
    }
    else if (strcmp(argv[1], "stats") == 0) {
        result = show_stats(db);
    }
    else {
        fprintf(stderr, "❌ Неизвестная команда: %s\n", argv[1]);
        print_usage(argv[0]);
        result = 0;
    }
    
    // Закрываем базу данных
    sqlite3_close(db);
    
    return result ? 0 : 1;
}