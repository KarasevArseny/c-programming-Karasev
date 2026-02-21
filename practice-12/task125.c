#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 4

// Структура для передачи данных в поток
typedef struct {
    int *array;        // Указатель на массив
    int start;         // Начальный индекс (включительно)
    int end;           // Конечный индекс (не включительно)
    long long partial_sum;  // Частичная сумма (заполняется потоком)
} ThreadData;

// Функция, выполняемая потоком
void *sum_partial(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->partial_sum = 0;
    
    for (int i = data->start; i < data->end; i++) {
        data->partial_sum += data->array[i];
    }
    
    printf("Поток %d: сумма [%d, %d) = %lld\n", 
           (int)(data->start / (ARRAY_SIZE / NUM_THREADS)), 
           data->start, data->end, data->partial_sum);
    
    return NULL;
}

int main(void) {
    // Выделяем память под массив
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (!array) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 1;
    }
    
    // Заполняем массив числами от 1 до ARRAY_SIZE
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }
    
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    
    // Рассчитываем размер части для каждого потока
    int chunk_size = ARRAY_SIZE / NUM_THREADS;
    
    // Создаём потоки
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].array = array;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * chunk_size;
        thread_data[i].partial_sum = 0;
        
        if (pthread_create(&threads[i], NULL, sum_partial, &thread_data[i]) != 0) {
            fprintf(stderr, "Ошибка при создании потока %d\n", i);
            free(array);
            return 1;
        }
    }
    
    // Ожидаем завершения всех потоков
    long long total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].partial_sum;
    }
    
    // Вычисляем ожидаемую сумму по формуле арифметической прогрессии
    long long expected_sum = (long long)ARRAY_SIZE * (ARRAY_SIZE + 1) / 2;
    
    printf("\nОбщая сумма: %lld\n", total_sum);
    printf("Проверка: %lld\n", expected_sum);
    printf("Результат: %s\n", (total_sum == expected_sum) ? "ВЕРНО" : "ОШИБКА");
    
    free(array);
    return 0;
}