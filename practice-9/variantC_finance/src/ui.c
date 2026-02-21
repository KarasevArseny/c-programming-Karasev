#include "ui.h"
#include "utils.h"
#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

void ui_init(void) {
    // Ничего не делаем
}

void ui_cleanup(void) {
    // Ничего не делаем
}

void ui_clear_screen(void) {
    system(CLEAR_SCREEN);
}

void ui_show_main_menu(void) {
    printf("\n========================================\n");
    printf("   СИСТЕМА УЧЁТА ФИНАНСОВ v1.0\n");
    printf("========================================\n\n");
    
    printf("1. Добавить транзакцию\n");
    printf("2. Список транзакций\n");
    printf("3. Редактировать транзакцию\n");
    printf("4. Удалить транзакцию\n");
    printf("5. Статистика\n");
    printf("6. Отчёт за период\n");
    printf("7. Управление счетами\n");
    printf("8. Управление категориями\n");
    printf("9. Экспорт в CSV\n");
    printf("0. Сохранить и выйти\n");
    printf("\nВыберите действие: ");
}

void ui_show_help(void) {
    ui_clear_screen();
    printf("=== СПРАВКА ===\n\n");
    printf("Команды:\n");
    printf("  add     - добавить транзакцию\n");
    printf("  list    - показать все транзакции\n");
    printf("  stats   - показать статистику\n");
    printf("  report  - отчёт за период\n");
    printf("  edit    - редактировать транзакцию\n");
    printf("  delete  - удалить транзакцию\n");
    printf("  accounts - управление счетами\n");
    printf("  categories - управление категориями\n");
    printf("  export  - экспорт в CSV\n");
    printf("  save    - сохранить данные\n");
    printf("  quit    - выход\n\n");
    ui_pause();
}

int ui_read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[50];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%d", &value) == 1) {
                if (value >= min && value <= max) {
                    return value;
                }
            }
        }
        printf("Ошибка: введите число от %d до %d\n", min, max);
    }
}

double ui_read_double(const char *prompt, double min, double max) {
    double value;
    char buffer[50];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%lf", &value) == 1) {
                if (value >= min && value <= max && utils_is_valid_amount(value)) {
                    return value;
                }
            }
        }
        printf("Ошибка: введите число от %.2f до %.2f\n", min, max);
    }
}

void ui_read_string(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
    }
}

time_t ui_read_date(const char *prompt) {
    char buffer[50];
    printf("%s (ГГГГ-ММ-ДД): ", prompt);
    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;
        return utils_parse_date(buffer);
    }
    return time(NULL);
}

void ui_show_transaction(const Transaction *t, int show_id) {
    char date_str[20];
    utils_format_date(t->date, date_str, sizeof(date_str));
    
    if (show_id) {
        printf("%3d | ", t->id);
    }
    
    printf("%s | %10.2f | %-15s | %-30s | %-10s | %s\n",
           t->type == TRANSACTION_INCOME ? "ДОХОД" : "РАСХОД",
           t->amount,
           t->category,
           t->description,
           t->account,
           date_str);
}

void ui_show_transactions(const Transaction *transactions, int count, 
                          const char *title) {
    if (count == 0) {
        printf("Нет транзакций для отображения.\n");
        return;
    }
    
    printf("\n=== %s ===\n", title);
    printf("ID  | Тип    | Сумма      | Категория       | Описание                      | Счёт       | Дата\n");
    printf("----|--------|------------|-----------------|-------------------------------|------------|----------------\n");
    
    for (int i = 0; i < count; i++) {
        ui_show_transaction(&transactions[i], 1);
    }
    printf("\n");
}

void ui_show_accounts(const FinanceData *data) {
    printf("\n=== СЧЕТА ===\n");
    printf("№   | Название счёта      | Баланс\n");
    printf("----|---------------------|------------\n");
    
    for (int i = 0; i < data->account_count; i++) {
        printf("%3d | %-19s | %10.2f ₽\n", 
               i + 1, data->accounts[i].name, data->accounts[i].balance);
    }
    printf("\n");
}

void ui_show_categories(const FinanceData *data) {
    printf("\n=== КАТЕГОРИИ ===\n");
    for (int i = 0; i < data->category_count; i++) {
        printf("%3d. %s\n", i + 1, data->categories[i]);
    }
    printf("\n");
}

void ui_show_stats(const FinanceData *data) {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    
    // Начало текущего месяца
    struct tm tm_month_start = *tm_now;
    tm_month_start.tm_mday = 1;
    tm_month_start.tm_hour = 0;
    tm_month_start.tm_min = 0;
    tm_month_start.tm_sec = 0;
    time_t month_start = mktime(&tm_month_start);
    
    // Начало текущего года
    struct tm tm_year_start = *tm_now;
    tm_year_start.tm_mon = 0;
    tm_year_start.tm_mday = 1;
    tm_year_start.tm_hour = 0;
    tm_year_start.tm_min = 0;
    tm_year_start.tm_sec = 0;
    time_t year_start = mktime(&tm_year_start);
    
    double total_balance = finance_get_balance(data);
    double month_income = finance_get_income_total(data, month_start, now);
    double month_expense = finance_get_expense_total(data, month_start, now);
    double year_income = finance_get_income_total(data, year_start, now);
    double year_expense = finance_get_expense_total(data, year_start, now);
    
    printf("\n=== СТАТИСТИКА ===\n\n");
    printf("Общий баланс: %.2f ₽\n\n", total_balance);
    
    printf("Текущий месяц:\n");
    printf("  Доходы:  +%.2f ₽\n", month_income);
    printf("  Расходы: -%.2f ₽\n", month_expense);
    printf("  Итого:   %+.2f ₽\n", month_income - month_expense);
    
    printf("\nТекущий год:\n");
    printf("  Доходы:  +%.2f ₽\n", year_income);
    printf("  Расходы: -%.2f ₽\n", year_expense);
    printf("  Итого:   %+.2f ₽\n", year_income - year_expense);
    
    // Статистика по категориям за текущий месяц
    if (month_expense > 0) {
        char cats[MAX_CATEGORIES][MAX_CATEGORY_NAME];
        double amounts[MAX_CATEGORIES];
        int cat_count;
        
        finance_get_category_stats(data, month_start, now, cats, amounts, &cat_count);
        
        if (cat_count > 0) {
            printf("\nРасходы по категориям (текущий месяц):\n");
            for (int i = 0; i < cat_count; i++) {
                double percent = (amounts[i] / month_expense) * 100;
                printf("  %-15s %10.2f ₽ (%5.1f%%)\n", 
                       cats[i], amounts[i], percent);
            }
        }
    }
}

int ui_confirm(const char *message) {
    char response[10];
    printf("%s (y/n): ", message);
    if (fgets(response, sizeof(response), stdin)) {
        return (response[0] == 'y' || response[0] == 'Y');
    }
    return 0;
}

void ui_pause(void) {
    printf("\nНажмите Enter для продолжения...");
    utils_clear_input_buffer();
}

// Внутренние функции для обработки команд
static void cmd_add_transaction(FinanceData *data) {
    ui_clear_screen();
    printf("=== ДОБАВЛЕНИЕ ТРАНЗАКЦИИ ===\n\n");
    
    if (data->account_count == 0) {
        printf("Ошибка: сначала создайте хотя бы один счёт.\n");
        ui_pause();
        return;
    }
    
    if (data->category_count == 0) {
        printf("Ошибка: сначала создайте хотя бы одну категорию.\n");
        ui_pause();
        return;
    }
    
    ui_show_accounts(data);
    int acc_idx = ui_read_int("Выберите счёт (№): ", 1, data->account_count) - 1;
    
    ui_show_categories(data);
    int cat_idx = ui_read_int("Выберите категорию (№): ", 1, data->category_count) - 1;
    
    int type = ui_read_int("Тип (1-доход, 2-расход): ", 1, 2);
    
    double amount = ui_read_double("Сумма: ", 0.01, 1e9);
    
    char description[MAX_DESCRIPTION];
    ui_read_string("Описание: ", description, MAX_DESCRIPTION);
    
    time_t date = ui_read_date("Дата");
    
    int id = finance_add_transaction(data, 
                                     type == 1 ? TRANSACTION_INCOME : TRANSACTION_EXPENSE,
                                     amount,
                                     data->categories[cat_idx],
                                     description,
                                     data->accounts[acc_idx].name,
                                     date);
    
    if (id > 0) {
        printf("✅ Транзакция #%d добавлена\n", id);
    } else {
        printf("❌ Ошибка при добавлении транзакции\n");
    }
    
    ui_pause();
}

static void cmd_list_transactions(FinanceData *data) {
    ui_clear_screen();
    
    if (data->transaction_count == 0) {
        printf("Нет транзакций.\n");
        ui_pause();
        return;
    }
    
    printf("=== СПИСОК ТРАНЗАКЦИЙ ===\n");
    printf("1. Все транзакции\n");
    printf("2. Только доходы\n");
    printf("3. Только расходы\n");
    printf("4. За текущий месяц\n");
    printf("Выберите фильтр: ");
    
    int filter = ui_read_int("", 1, 4);
    
    time_t now = time(NULL);
    time_t month_start = now;
    if (filter == 4) {
        struct tm *tm_now = localtime(&now);
        tm_now->tm_mday = 1;
        tm_now->tm_hour = 0;
        tm_now->tm_min = 0;
        tm_now->tm_sec = 0;
        month_start = mktime(tm_now);
    }
    
    // Собираем транзакции по фильтру
    Transaction *filtered = malloc(data->transaction_count * sizeof(Transaction));
    int count = 0;
    
    for (int i = 0; i < data->transaction_count; i++) {
        Transaction *t = &data->transactions[i];
        int include = 0;
        
        switch (filter) {
            case 1: include = 1; break;
            case 2: include = (t->type == TRANSACTION_INCOME); break;
            case 3: include = (t->type == TRANSACTION_EXPENSE); break;
            case 4: include = (t->date >= month_start && t->date <= now); break;
        }
        
        if (include) {
            filtered[count++] = *t;
        }
    }
    
    if (count > 0) {
        printf("\nСортировать по:\n");
        printf("1. Дате\n");
        printf("2. Сумме\n");
        printf("3. Категории\n");
        int sort = ui_read_int("Выберите: ", 1, 3);
        
        printf("Порядок (1-возрастание, 2-убывание): ");
        int order = ui_read_int("", 1, 2);
        
        switch (sort) {
            case 1: finance_sort_by_date(filtered, count, order == 1); break;
            case 2: finance_sort_by_amount(filtered, count, order == 1); break;
            case 3: finance_sort_by_category(filtered, count, order == 1); break;
        }
        
        ui_show_transactions(filtered, count, "ТРАНЗАКЦИИ");
    } else {
        printf("Нет транзакций по выбранному фильтру.\n");
    }
    
    free(filtered);
    ui_pause();
}

static void cmd_edit_transaction(FinanceData *data) {
    ui_clear_screen();
    printf("=== РЕДАКТИРОВАНИЕ ТРАНЗАКЦИИ ===\n\n");
    
    int id = ui_read_int("Введите ID транзакции: ", 1, data->next_transaction_id);
    
    Transaction *t = finance_get_transaction(data, id);
    if (!t) {
        printf("Транзакция с ID %d не найдена.\n", id);
        ui_pause();
        return;
    }
    
    printf("\nТекущие данные:\n");
    ui_show_transaction(t, 1);
    printf("\n");
    
    if (!ui_confirm("Редактировать эту транзакцию?")) {
        return;
    }
    
    ui_show_accounts(data);
    int acc_idx = ui_read_int("Выберите счёт (№): ", 1, data->account_count) - 1;
    
    ui_show_categories(data);
    int cat_idx = ui_read_int("Выберите категорию (№): ", 1, data->category_count) - 1;
    
    int type = ui_read_int("Тип (1-доход, 2-расход): ", 1, 2);
    
    double amount = ui_read_double("Сумма: ", 0.01, 1e9);
    
    char description[MAX_DESCRIPTION];
    ui_read_string("Описание: ", description, MAX_DESCRIPTION);
    
    time_t date = ui_read_date("Дата");
    
    int result = finance_edit_transaction(data, id,
                                         type == 1 ? TRANSACTION_INCOME : TRANSACTION_EXPENSE,
                                         amount,
                                         data->categories[cat_idx],
                                         description,
                                         data->accounts[acc_idx].name,
                                         date);
    
    if (result == 0) {
        printf("✅ Транзакция #%d обновлена\n", id);
    } else {
        printf("❌ Ошибка при обновлении транзакции\n");
    }
    
    ui_pause();
}

static void cmd_delete_transaction(FinanceData *data) {
    ui_clear_screen();
    printf("=== УДАЛЕНИЕ ТРАНЗАКЦИИ ===\n\n");
    
    int id = ui_read_int("Введите ID транзакции: ", 1, data->next_transaction_id);
    
    Transaction *t = finance_get_transaction(data, id);
    if (!t) {
        printf("Транзакция с ID %d не найдена.\n", id);
        ui_pause();
        return;
    }
    
    printf("\nТранзакция для удаления:\n");
    ui_show_transaction(t, 1);
    printf("\n");
    
    if (ui_confirm("Удалить эту транзакцию?")) {
        if (finance_delete_transaction(data, id) == 0) {
            printf("✅ Транзакция #%d удалена\n", id);
        } else {
            printf("❌ Ошибка при удалении транзакции\n");
        }
    }
    
    ui_pause();
}

static void cmd_statistics(FinanceData *data) {
    ui_clear_screen();
    ui_show_stats(data);
    ui_pause();
}

static void cmd_report(FinanceData *data) {
    ui_clear_screen();
    printf("=== ОТЧЁТ ЗА ПЕРИОД ===\n\n");
    
    time_t start = ui_read_date("Начальная дата");
    time_t end = ui_read_date("Конечная дата");
    
    if (end < start) {
        printf("Ошибка: конечная дата раньше начальной\n");
        ui_pause();
        return;
    }
    
    double income = finance_get_income_total(data, start, end);
    double expense = finance_get_expense_total(data, start, end);
    
    printf("\n=== ОТЧЁТ ===\n");
    printf("Период: ");
    char start_str[20], end_str[20];
    utils_format_date(start, start_str, sizeof(start_str));
    utils_format_date(end, end_str, sizeof(end_str));
    printf("%s - %s\n\n", start_str, end_str);
    
    printf("Доходы:  +%.2f ₽\n", income);
    printf("Расходы: -%.2f ₽\n", expense);
    printf("Баланс:  %+.2f ₽\n\n", income - expense);
    
    if (expense > 0) {
        printf("По категориям расходов:\n");
        char cats[MAX_CATEGORIES][MAX_CATEGORY_NAME];
        double amounts[MAX_CATEGORIES];
        int cat_count;
        
        finance_get_category_stats(data, start, end, cats, amounts, &cat_count);
        
        for (int i = 0; i < cat_count; i++) {
            double percent = (amounts[i] / expense) * 100;
            printf("  %-15s %10.2f ₽ (%5.1f%%)\n", cats[i], amounts[i], percent);
        }
    }
    
    ui_pause();
}

static void cmd_manage_accounts(FinanceData *data) {
    int choice;
    do {
        ui_clear_screen();
        ui_show_accounts(data);
        
        printf("УПРАВЛЕНИЕ СЧЕТАМИ:\n");
        printf("1. Добавить счёт\n");
        printf("2. Удалить счёт\n");
        printf("0. Назад\n");
        choice = ui_read_int("Выберите: ", 0, 2);
        
        switch (choice) {
            case 1: {
                char name[MAX_ACCOUNT_NAME];
                ui_read_string("Название счёта: ", name, MAX_ACCOUNT_NAME);
                
                if (!utils_is_valid_account(name)) {
                    printf("❌ Некорректное название счёта\n");
                    ui_pause();
                    break;
                }
                
                double initial = ui_read_double("Начальный баланс: ", 0, 1e9);
                
                int result = finance_add_account(data, name, initial);
                if (result == 0) {
                    printf("✅ Счёт '%s' добавлен\n", name);
                } else if (result == -2) {
                    printf("❌ Счёт '%s' уже существует\n", name);
                } else {
                    printf("❌ Ошибка при добавлении счёта\n");
                }
                ui_pause();
                break;
            }
            
            case 2: {
                if (data->account_count == 0) {
                    printf("Нет счетов для удаления.\n");
                    ui_pause();
                    break;
                }
                
                ui_show_accounts(data);
                int idx = ui_read_int("Выберите счёт для удаления (№): ", 1, data->account_count) - 1;
                
                if (ui_confirm("Удалить этот счёт?")) {
                    int result = finance_remove_account(data, data->accounts[idx].name);
                    if (result == 0) {
                        printf("✅ Счёт удалён\n");
                    } else if (result == -3) {
                        printf("❌ Нельзя удалить счёт с транзакциями\n");
                    } else {
                        printf("❌ Ошибка при удалении счёта\n");
                    }
                }
                ui_pause();
                break;
            }
        }
    } while (choice != 0);
}

static void cmd_manage_categories(FinanceData *data) {
    int choice;
    do {
        ui_clear_screen();
        ui_show_categories(data);
        
        printf("УПРАВЛЕНИЕ КАТЕГОРИЯМИ:\n");
        printf("1. Добавить категорию\n");
        printf("2. Удалить категорию\n");
        printf("0. Назад\n");
        choice = ui_read_int("Выберите: ", 0, 2);
        
        switch (choice) {
            case 1: {
                char name[MAX_CATEGORY_NAME];
                ui_read_string("Название категории: ", name, MAX_CATEGORY_NAME);
                
                if (!utils_is_valid_category(name)) {
                    printf("❌ Некорректное название категории\n");
                    ui_pause();
                    break;
                }
                
                int result = finance_add_category(data, name);
                if (result == 0) {
                    printf("✅ Категория '%s' добавлена\n", name);
                } else if (result == -2) {
                    printf("❌ Категория '%s' уже существует\n", name);
                } else {
                    printf("❌ Ошибка при добавлении категории\n");
                }
                ui_pause();
                break;
            }
            
            case 2: {
                if (data->category_count == 0) {
                    printf("Нет категорий для удаления.\n");
                    ui_pause();
                    break;
                }
                
                ui_show_categories(data);
                int idx = ui_read_int("Выберите категорию для удаления (№): ", 1, data->category_count) - 1;
                
                if (ui_confirm("Удалить эту категорию?")) {
                    int result = finance_remove_category(data, data->categories[idx]);
                    if (result == 0) {
                        printf("✅ Категория удалена\n");
                    } else if (result == -3) {
                        printf("❌ Нельзя удалить категорию с транзакциями\n");
                    } else {
                        printf("❌ Ошибка при удалении категории\n");
                    }
                }
                ui_pause();
                break;
            }
        }
    } while (choice != 0);
}

static void cmd_export_csv(FinanceData *data) {
    ui_clear_screen();
    printf("=== ЭКСПОРТ В CSV ===\n\n");
    
    time_t start = ui_read_date("Начальная дата (Enter - все время)");
    time_t end = ui_read_date("Конечная дата (Enter - все время)");
    
    if (end < start) {
        printf("Ошибка: конечная дата раньше начальной\n");
        ui_pause();
        return;
    }
    
    char filename[100];
    ui_read_string("Имя файла (без .csv): ", filename, sizeof(filename));
    strcat(filename, ".csv");
    
    int result = storage_export_csv(data, filename, start, end);
    
    if (result == 0) {
        printf("✅ Данные экспортированы в %s\n", filename);
    } else {
        printf("❌ Ошибка при экспорте\n");
    }
    
    ui_pause();
}

void ui_main_loop(FinanceData *data) {
    int choice;
    
    do {
        ui_clear_screen();
        ui_show_main_menu();
        
        choice = ui_read_int("", 0, 9);
        
        switch (choice) {
            case 1: cmd_add_transaction(data); break;
            case 2: cmd_list_transactions(data); break;
            case 3: cmd_edit_transaction(data); break;
            case 4: cmd_delete_transaction(data); break;
            case 5: cmd_statistics(data); break;
            case 6: cmd_report(data); break;
            case 7: cmd_manage_accounts(data); break;
            case 8: cmd_manage_categories(data); break;
            case 9: cmd_export_csv(data); break;
            case 0: 
                if (ui_confirm("Сохранить данные перед выходом?")) {
                    storage_save(data, "finance.dat");
                }
                break;
        }
    } while (choice != 0);
}