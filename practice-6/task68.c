#include <stdio.h>
#include <stdlib.h>
typedef struct {
void *ptr;
size_t ref_count;
void (*destructor)(void*);
} SharedPtr;
SharedPtr* shared_ptr_new(void *data, void (*destructor)(void*)) {
if (!data) return NULL;
SharedPtr *sp = malloc(sizeof(SharedPtr));
sp->ptr = data;
sp->ref_count = 1;
sp->destructor = destructor;
printf("Создан указатель на %p (счётчик = %zu)\n", data, sp->ref_count);
return sp;
}
SharedPtr* shared_ptr_clone(SharedPtr *sp) {
if (!sp) return NULL;
sp->ref_count++;
printf("Клонирован указатель на %p (счётчик = %zu)\n", sp->ptr, sp->ref_count);
return sp;
}
void shared_ptr_release(SharedPtr *sp) {
if (!sp) return;
sp->ref_count--;
printf("Освобождена ссылка на %p (счётчик = %zu)\n", sp->ptr, sp->ref_count);
if (sp->ref_count == 0) {
if (sp->destructor) sp->destructor(sp->ptr);
else free(sp->ptr);
free(sp);
}
}
void* shared_ptr_get(const SharedPtr *sp) {
return sp ? sp->ptr : NULL;
}
void int_destructor(void *data) {
printf("  Удаление int: %d\n", *(int*)data);
free(data);
}
int main(void) {
int *num = malloc(sizeof(int));
*num = 42;    
SharedPtr *sp1 = shared_ptr_new(num, int_destructor);
printf("Значение: %d\n", *(int*)shared_ptr_get(sp1));    
SharedPtr *sp2 = shared_ptr_clone(sp1);
printf("Значение через клон: %d\n", *(int*)shared_ptr_get(sp2));    
shared_ptr_release(sp1);
shared_ptr_release(sp2);    
return 0;
}