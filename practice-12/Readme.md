Практическая работа 12: Многопоточное программирование
Студент: Карасев Арсений Алексеевич
Группа: 1зб_ИВТ-25
Дата: 23 февраля 2026 г.

Задание 1: Hello World с потоками
Постановка задачи
Напишите программу, которая создаёт три потока. Каждый поток выводит своё приветствие.

Список идентификаторов
Имя	Тип	Описание
threads[3]	pthread_t[]	Массив идентификаторов потоков
ids[3]	int[]	Массив номеров потоков

Код программы

#include <stdio.h>
#include <pthread.h>

void *thread_function(void *arg) {
    int id = *(int *)arg;
    printf("Привет из потока %d!\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[3];
    int ids[3] = {1, 2, 3};
    
    // Создаём три потока
    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, &ids[i]) != 0) {
            fprintf(stderr, "Ошибка при создании потока %d\n", ids[i]);
            return 1;
        }
    }
    
    // Дожидаемся завершения всех потоков
    for (int i = 0; i < 3; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Ошибка при ожидании потока %d\n", ids[i]);
            return 1;
        }
    }
    
    printf("Все потоки завершились.\n");
    return 0;
}

Результаты работы
screenshots/task-12-1.png

Задание 2: Передача структуры в поток
Постановка задачи
Напишите программу, где каждый поток получает структуру с двумя числами и вычисляет их сумму.

Структура данных

typedef struct {
    int a;
    int b;
    int result;  // Сюда поток запишет результат
} ThreadArgs;
Список идентификаторов
Имя	Тип	Описание
args[4]	ThreadArgs[]	Массив структур с данными для потоков
threads[4]	pthread_t[]	Массив идентификаторов потоков

Код программы

#include <stdio.h>
#include <pthread.h>

typedef struct {
    int a;
    int b;
    int result;
} ThreadArgs;

void *thread_function(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
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
    
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, thread_function, &args[i]);
    }
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    for (int i = 0; i < 4; i++) {
        printf("Поток %d: %d + %d = %d\n", 
               i + 1, args[i].a, args[i].b, args[i].result);
    }
    
    return 0;
}

Результаты работы
screenshots/task-12-2.png

Задание 3: Наблюдение race condition
Постановка задачи
Создайте программу, демонстрирующую race condition: два потока увеличивают общий счётчик 100000 раз каждый.

Список идентификаторов
Имя	Тип	Описание
counter	int	Глобальный счётчик (общий ресурс)
t1, t2	pthread_t	Идентификаторы потоков

Код программы

#include <stdio.h>
#include <pthread.h>

int counter = 0;

void *increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        counter++;  // НЕБЕЗОПАСНАЯ операция!
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Ожидаемое значение: 200000\n");
    printf("Фактическое значение: %d\n", counter);
    
    return 0;
}

Результаты работы
screenshots/task-12-3.png

Задание 4: Исправление race condition с mutex
Постановка задачи
Исправьте программу из задания 3, добавив mutex для защиты переменной counter.

Список идентификаторов
Имя	Тип	Описание
counter	int	Глобальный счётчик (общий ресурс)
mutex	pthread_mutex_t	Мьютекс для защиты счётчика

Код программы

#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    pthread_mutex_destroy(&mutex);
    
    printf("Ожидаемое значение: 200000\n");
    printf("Фактическое значение: %d\n", counter);
    
    return 0;
}

Результаты работы
screenshots/task-12-4.png

Задание 5: Параллельное суммирование массива
Постановка задачи
Напишите программу для параллельного вычисления суммы элементов массива из 1,000,000 элементов (числа от 1 до 1,000,000) с использованием 4 потоков.

Математическая модель
Сумма чисел от 1 до n вычисляется по формуле арифметической прогрессии:
S= n×(n+1)
​      2
 
Для n = 1,000,000:

S= 1,000,000 × 1,000,001 = 500,000,500,000
             2
Структура данных

typedef struct {
    int *array;        // Указатель на массив
    int start;         // Начальный индекс
    int end;           // Конечный индекс
    long long partial_sum;  // Частичная сумма
} ThreadData;
Список идентификаторов
Имя	Тип	Описание
array	int*	Массив чисел
thread_data[4]	ThreadData[]	Данные для потоков
threads[4]	pthread_t[]	Идентификаторы потоков

Код программы

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 4

typedef struct {
    int *array;
    int start;
    int end;
    long long partial_sum;
} ThreadData;

void *sum_partial(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->partial_sum = 0;
    
    for (int i = data->start; i < data->end; i++) {
        data->partial_sum += data->array[i];
    }
    
    printf("Поток %d: сумма [%d, %d) = %lld\n", 
           data->start / (ARRAY_SIZE / NUM_THREADS),
           data->start, data->end, data->partial_sum);
    
    return NULL;
}

int main(void) {
    int *array = malloc(ARRAY_SIZE * sizeof(int));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }
    
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int chunk_size = ARRAY_SIZE / NUM_THREADS;
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].array = array;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * chunk_size;
        thread_data[i].partial_sum = 0;
        
        pthread_create(&threads[i], NULL, sum_partial, &thread_data[i]);
    }
    
    long long total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].partial_sum;
    }
    
    long long expected_sum = (long long)ARRAY_SIZE * (ARRAY_SIZE + 1) / 2;
    
    printf("\nОбщая сумма: %lld\n", total_sum);
    printf("Проверка: %lld\n", expected_sum);
    printf("Результат: %s\n", (total_sum == expected_sum) ? "ВЕРНО" : "ОШИБКА");
    
    free(array);
    return 0;
}

Результаты работы
screenshots/task-12-5.png

Задание 6: Счётчик с несколькими потоками
Постановка задачи
Создайте программу со счётчиком, который увеличивается 10 потоками (каждый по 10000 раз). Используйте mutex для защиты.

Список идентификаторов
Имя	Тип	Описание
counter	int	Глобальный счётчик
mutex	pthread_mutex_t	Мьютекс для защиты
threads[10]	pthread_t[]	Массив идентификаторов потоков

Код программы

#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 10
#define INCREMENTS_PER_THREAD 10000

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int thread_id;
} ThreadData;

void *increment_counter(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int id = data->thread_id;
    
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    
    printf("Поток %d завершил работу\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    struct timespec start, end;
    
    printf("Запуск %d потоков, каждый увеличивает счётчик %d раз\n", 
           NUM_THREADS, INCREMENTS_PER_THREAD);
    printf("Ожидаемое значение: %d\n\n", NUM_THREADS * INCREMENTS_PER_THREAD);
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i + 1;
        pthread_create(&threads[i], NULL, increment_counter, &thread_data[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double time_spent = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    
    pthread_mutex_destroy(&mutex);
    
    printf("\n✅ Итоговое значение: %d\n", counter);
    printf("✅ Ожидаемое значение: %d\n", NUM_THREADS * INCREMENTS_PER_THREAD);
    printf("Время выполнения: %.3f секунд\n", time_spent);
    
    return 0;
}

Результаты работы
screenshots/task-12-6.png