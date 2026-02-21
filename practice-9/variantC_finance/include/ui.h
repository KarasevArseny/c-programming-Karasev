#ifndef UI_H
#define UI_H

#include "data.h"

// Инициализация интерфейса
void ui_init(void);
void ui_cleanup(void);

// Основной цикл
void ui_main_loop(FinanceData *data);

// Отображение меню
void ui_show_main_menu(void);
void ui_show_help(void);

// Ввод данных
int ui_read_int(const char *prompt, int min, int max);
double ui_read_double(const char *prompt, double min, double max);
void ui_read_string(const char *prompt, char *buffer, size_t size);
time_t ui_read_date(const char *prompt);

// Отображение данных
void ui_show_transaction(const Transaction *t, int show_id);
void ui_show_transactions(const Transaction *transactions, int count, 
                          const char *title);
void ui_show_accounts(const FinanceData *data);
void ui_show_categories(const FinanceData *data);
void ui_show_stats(const FinanceData *data);

// Подтверждение действий
int ui_confirm(const char *message);

// Ожидание нажатия клавиши
void ui_pause(void);

// Очистка экрана
void ui_clear_screen(void);

#endif // UI_H