#include <stdio.h>
#include <pthread.h>

// Глобальная переменная-счётчик (общий ресурс)
int counter = 0;

// Функция, выполняемая потоками
void *increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        counter++;  // НЕБЕЗОПАСНАЯ операция!
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    
    // Создаём два потока
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    // Ожидаем завершения потоков
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    // Ожидаемое значение: 200000 (100000 * 2)
    printf("Ожидаемое значение: 200000\n");
    printf("Фактическое значение: %d\n", counter);
    
    return 0;
}