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