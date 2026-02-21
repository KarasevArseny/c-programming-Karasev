#include <stdio.h>
#include <pthread.h>

// Структура для передачи данных в поток
typedef struct {
    int a;
    int b;
    int result;  // Сюда поток запишет результат
} ThreadArgs;

// Функция, выполняемая потоком
void *thread_function(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    
    // Вычисляем сумму
    args->result = args->a + args->b;
    
    return NULL;
}

int main(void) {
    pthread_t threads[4];
    ThreadArgs args[4] = {
        {10, 20, 0},
        {5, 15, 0},
        {100, 200, 0},
        {7, 3, 0}
    };
    
    // Создаём 4 потока
    for (int i = 0; i < 4; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            fprintf(stderr, "Ошибка при создании потока %d\n", i + 1);
            return 1;
        }
    }
    
    // Дожидаемся завершения всех потоков
    for (int i = 0; i < 4; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Ошибка при ожидании потока %d\n", i + 1);
            return 1;
        }
    }
    
    // Выводим результаты
    for (int i = 0; i < 4; i++) {
        printf("Поток %d: %d + %d = %d\n", 
               i + 1, 
               args[i].a, 
               args[i].b, 
               args[i].result);
    }
    
    return 0;
}