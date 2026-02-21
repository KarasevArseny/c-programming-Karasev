#include <stdio.h>
int main(void) {
int value = 100;
int *p1 = &value;
int *p2 = &value;    
printf("=== Два указателя на одну переменную ===\n\n");    
printf("Исходное состояние:\n");
printf("value = %d\n", value);
printf("Адрес value: %p\n\n", (void*)&value);    
printf("Указатель p1:\n");
printf("  p1 = %p (адрес value)\n", (void*)p1);
printf("  *p1 = %d (значение value)\n\n", *p1);    
printf("Указатель p2:\n");
printf("  p2 = %p (адрес value)\n", (void*)p2);
printf("  *p2 = %d (значение value)\n\n", *p2);    
printf("Изменяем value через p1:\n");
printf("*p1 = 200;\n");
*p1 = 200;    
printf("\nРезультат после изменения:\n");
printf("value = %d (изменилось!)\n", value);
printf("*p1 = %d (отражает изменение)\n", *p1);
printf("*p2 = %d (тоже отражает изменение!)\n", *p2);    
return 0;
}