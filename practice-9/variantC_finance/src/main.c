#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "data.h"
#include "storage.h"
#include "ui.h"
#include "utils.h"

int main(void) {
    // Устанавливаем локаль для корректного отображения русских символов
    setlocale(LC_ALL, "");
    
    FinanceData data;
    finance_init(&data);
    
    // Пытаемся загрузить сохранённые данные
    int load_result = storage_load(&data, "finance.dat");
    
    if (load_result == 0) {
        printf("✅ Данные загружены из finance.dat\n");
    } else if (load_result == -2) {
        printf("ℹ️ Файл данных не найден. Будет создана новая база.\n");
        
        // Добавляем начальные данные для демонстрации
        finance_add_account(&data, "Наличные", 0);
        finance_add_account(&data, "Карта", 0);
        
        finance_add_category(&data, "Продукты");
        finance_add_category(&data, "Транспорт");
        finance_add_category(&data, "Развлечения");
        finance_add_category(&data, "Коммунальные");
        finance_add_category(&data, "Зарплата");
    } else {
        printf("❌ Ошибка при загрузке данных (код %d)\n", load_result);
    }
    
    ui_init();
    ui_main_loop(&data);
    ui_cleanup();
    
    // Финальное сохранение (на всякий случай)
    storage_save(&data, "finance.dat");
    finance_cleanup(&data);
    
    printf("\n👋 До свидания!\n");
    return 0;
}