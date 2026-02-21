#include <stdio.h>
#include <string.h>
typedef int (*Comparator)(const void*, const void*);
void bubble_sort(void *base, size_t n, size_t size, Comparator cmp) {
char *arr = (char*)base;
for (size_t i = 0; i < n - 1; i++) {
for (size_t j = 0; j < n - i - 1; j++) {
void *a = arr + j * size;
void *b = arr + (j + 1) * size;
if (cmp(a, b) > 0) {
char tmp[size];
memcpy(tmp, a, size);
memcpy(a, b, size);
memcpy(b, tmp, size);
}
}
}
}
int cmp_int_asc(const void *a, const void *b) {
return *(int*)a - *(int*)b;
}
int cmp_int_desc(const void *a, const void *b) {
return *(int*)b - *(int*)a;
}
typedef struct {
char name[50];
int age;
double salary;
} Employee;
int cmp_employee_salary(const void *a, const void *b) {
double sa = ((Employee*)a)->salary;
double sb = ((Employee*)b)->salary;
return (sa > sb) - (sa < sb);  // -1, 0, 1
}
int main(void) {
printf("=== Числа ===\n");
int nums[] = {5, 2, 8, 1, 9};
int n = sizeof(nums) / sizeof(nums[0]);    
printf("Исходный: ");
for (int i = 0; i < n; i++) printf("%d ", nums[i]);    
bubble_sort(nums, n, sizeof(int), cmp_int_asc);
printf("\nВозрастание: ");
for (int i = 0; i < n; i++) printf("%d ", nums[i]);   
bubble_sort(nums, n, sizeof(int), cmp_int_desc);
printf("\nУбывание: ");
for (int i = 0; i < n; i++) printf("%d ", nums[i]);
printf("\n\n=== Сотрудники по зарплате ===\n");
Employee emps[] = {
{"Иван", 25, 45000},
{"Мария", 30, 55000},
{"Пётр", 28, 60000}
};
int m = sizeof(emps) / sizeof(emps[0]);    
printf("До:\n");
for (int i = 0; i < m; i++) 
printf("  %s, %d, %.0f\n", emps[i].name, emps[i].age, emps[i].salary);    
bubble_sort(emps, m, sizeof(Employee), cmp_employee_salary);    
printf("После (по зарплате):\n");
for (int i = 0; i < m; i++) 
printf("  %s, %d, %.0f\n", emps[i].name, emps[i].age, emps[i].salary);
return 0;
}