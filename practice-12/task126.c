#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 10
#define INCREMENTS_PER_THREAD 10000

// Глобальный счётчик
int counter = 0;

// Мьютекс для защиты счётчика
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Структура для передачи номера потока
typedef struct {
    int thread_id;
} ThreadData;

// Функция, выполняемая потоками
void *increment_counter(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int id = data->thread_id;
    
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        pthread_mutex_lock(&mutex);
        counter++;  // Критическая секция
        pthread_mutex_unlock(&mutex);
    }
    
    printf("Поток %d завершил работу (увеличил счётчик %d раз)\n", 
           id, INCREMENTS_PER_THREAD);
    
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    struct timespec start, end;
    
    printf("Запуск %d потоков, каждый увеличивает счётчик %d раз\n", 
           NUM_THREADS, INCREMENTS_PER_THREAD);
    printf("Ожидаемое итоговое значение: %d\n", 
           NUM_THREADS * INCREMENTS_PER_THREAD);
    printf("\n");
    
    // Засекаем время начала
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Создаём потоки
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i + 1;
        
        if (pthread_create(&threads[i], NULL, increment_counter, &thread_data[i]) != 0) {
            fprintf(stderr, "Ошибка при создании потока %d\n", i + 1);
            return 1;
        }
    }
    
    // Ожидаем завершения всех потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Засекаем время окончания
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    // Вычисляем время выполнения
    double time_spent = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    
    printf("\n✅ Все потоки завершились.\n");
    printf("Итоговое значение счётчика: %d\n", counter);
    printf("Ожидаемое значение:          %d\n", NUM_THREADS * INCREMENTS_PER_THREAD);
    printf("Результат: %s\n", 
           (counter == NUM_THREADS * INCREMENTS_PER_THREAD) ? "ВЕРНО" : "ОШИБКА");
    printf("Время выполнения: %.3f секунд\n", time_spent);
    
    // Уничтожаем мьютекс
    pthread_mutex_destroy(&mutex);
    
    return 0;
}