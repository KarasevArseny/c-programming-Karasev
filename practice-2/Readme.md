Практическая работа 2: Типы данных, арифметика, условные операторы
Студент: Карасев Арсений Алексеевич
Группа: 1зб_ИВТ-25
Дата: 15 февраля 2026 г.
Задание 2.1: Вычисление формулы
Постановка задачи
Напишите программу, которая вычисляет значение выражения (см в задании)

для значений: a = 0.12, b = 3.5, c = 2.4, x = 1.4.

Математическая модель
Формула разбивается на две части:

Первое слагаемое: $-\frac{x-a}{\sqrt[3]{x^2+a^2}}$

Второе слагаемое: $-\frac{\sqrt[4]{(x^2+b^2)^3}}{2+a+b+\sqrt[3]{(x-c)^2}}$

Для вычислений используются степени с дробными показателями:

Кубический корень: $\sqrt[3]{x} = x^{1/3}$

Корень четвёртой степени: $\sqrt[4]{x} = x^{1/4}$

Список идентификаторов
Имя	Тип	Описание
a	double	Параметр a (0.12)
b	double	Параметр b (3.5)
c	double	Параметр c (2.4)
x	double	Переменная x (1.4)
num1	double	Числитель первого слагаемого (x - a)
denom1	double	Знаменатель первого слагаемого
term1	double	Первое слагаемое
num2	double	Числитель второго слагаемого
denom2	double	Знаменатель второго слагаемого
term2	double	Второе слагаемое
h	double	Результат вычисления функции

Код программы

#include <stdio.h>
#include <math.h>

int main(void) {
    double a = 0.12, b = 3.5, c = 2.4, x = 1.4;
    
    double num1 = x - a; 
    double denom1 = pow(x*x + a*a, 1.0/3.0); 
    double term1 = num1 / denom1; 
    
    double num2 = pow(pow(x*x + b*b, 3), 1.0/4.0);
    double denom2 = 2 + a + b + pow(pow(x - c, 2), 1.0/3.0);
    double term2 = num2 / denom2;
    
    double h = -term1 - term2;
    
    printf("h(%.2lf) = %.6lf\n", x, h);
    
    return 0;
}

Результаты работы
(screenshots/task-2-1.png)

Задание 2.2: Чётное или нечётное
Постановка задачи
Напишите программу, которая определяет, является ли введённое число чётным или нечётным.

Математическая модель
Число является чётным, если остаток от деления на 2 равен 0:

n%2=0⇒чётное

Список идентификаторов
Имя	Тип	Описание
number	int	Введённое пользователем число
Код программы
#include <stdio.h>

int main(void) {
    int number;
    
    printf("Enter number: ");
    scanf("%d", &number);
    
    if (number % 2 == 0) {
        printf("%d is even\n", number);
    } else {
        printf("%d is odd\n", number);
    }
    
    return 0;
}

Результаты работы
(screenshots/task-2-2.png)

Задание 2.3: Максимум из трёх
Постановка задачи
Напишите программу, которая находит максимальное из трёх введённых чисел.

Список идентификаторов
Имя	Тип	Описание
a	int	Первое число
b	int	Второе число
c	int	Третье число
max	int	Максимальное из трёх чисел

Код программы
#include <stdio.h>

int main(void) {
    int a, b, c;
    int max;
    
    printf("Enter three numbers: ");
    scanf("%d %d %d", &a, &b, &c);
    
    if (a >= b && a >= c) {
        max = a;
    } else if (b >= a && b >= c) {
        max = b;
    } else {
        max = c;
    }
    
    printf("Maximum: %d\n", max);
    
    return 0;
}
Результаты работы
(screenshots/task-2-3.png)

Задание 2.4: Тип треугольника
Постановка задачи
Напишите программу, которая по трём сторонам определяет тип треугольника:

Равносторонний — все стороны равны

Равнобедренный — две стороны равны

Разносторонний — все стороны разные

Не существует — если сумма двух сторон <= третьей

Математическая модель
Условие существования треугольника:

a+b>cиa+c>bиb+c>a

Список идентификаторов
Имя	Тип	Описание
a	float	Первая сторона треугольника
b	float	Вторая сторона треугольника
c	float	Третья сторона треугольника
Код программы
#include <stdio.h>

int main(void) {
    float a, b, c;
    
    printf("Enter sides a, b, c: ");
    scanf("%f %f %f", &a, &b, &c);
    
    if (a + b <= c || a + c <= b || b + c <= a) {
        printf("Triangle does not exist\n");
    }
    else if (a == b && b == c) {
        printf("Equilateral triangle\n");
    }
    else if (a == b || a == c || b == c) {
        printf("Isosceles triangle\n");
    }
    else {
        printf("Scalene triangle\n");
    }
    
    return 0;
}

Результаты работы
(screenshots/task-2-4.png)

Задание 2.5: Квадратное уравнение
Постановка задачи
Напишите программу для решения квадратного уравнения $ax^2 + bx + c = 0$.

Математическая модель
Дискриминант:

D=b 2 −4ac

Корни уравнения:

Если $D > 0$: два действительных корня

x 1,2 = −b±D
        2а​
 
​
Если $D = 0$: один корень (кратности 2)

x= −b
​    2а
 
Если $D < 0$: действительных корней нет

Список идентификаторов
Имя	Тип	Описание
a	double	Коэффициент a
b	double	Коэффициент b
c	double	Коэффициент c
D	double	Дискриминант
x1, x2	double	Корни уравнения
Код программы
#include <stdio.h>
#include <math.h>

int main(void) {
    double a, b, c;
    double D, x1, x2;
    
    printf("Enter a, b, c: ");
    scanf("%lf %lf %lf", &a, &b, &c);
    
    if (a == 0) {
        printf("This is not a quadratic equation (a = 0)\n");
        return 1;
    }
    
    D = b * b - 4 * a * c;
    printf("D = %.2f\n", D);
    
    if (D > 0) {
        x1 = (-b + sqrt(D)) / (2 * a);
        x2 = (-b - sqrt(D)) / (2 * a);
        printf("x1 = %.2f\n", x1);
        printf("x2 = %.2f\n", x2);
    } 
    else if (D == 0) {
        x1 = -b / (2 * a);
        printf("x = %.2f\n", x1);
    } 
    else {
        printf("No real roots\n");
    }
    
    return 0;
}
Результаты работы
(screenshots/task-2-5.png)

Задание 2.6: Калькулятор
Постановка задачи
Напишите программу-калькулятор, которая:

Запрашивает два числа

Запрашивает операцию (+, -, *, /)

Выводит результат

Используйте switch для выбора операции.

Список идентификаторов
Имя	Тип	Описание
num1	double	Первое число
num2	double	Второе число
result	double	Результат операции
operation	char	Операция (+, -, *, /)

Код программы
#include <stdio.h>

int main(void) {
    double num1, num2, result;
    char operation;
    
    printf("Enter first number: ");
    scanf("%lf", &num1);
    
    printf("Enter second number: ");
    scanf("%lf", &num2);
    
    printf("Enter operation (+, -, *, /): ");
    scanf(" %c", &operation);
    
    switch (operation) {
        case '+':
            result = num1 + num2;
            printf("Result: %.2lf + %.2lf = %.2lf\n", num1, num2, result);
            break;
        case '-':
            result = num1 - num2;
            printf("Result: %.2lf - %.2lf = %.2lf\n", num1, num2, result);
            break;
        case '*':
            result = num1 * num2;
            printf("Result: %.2lf * %.2lf = %.2lf\n", num1, num2, result);
            break;
        case '/':
            if (num2 == 0) {
                printf("Error: Division by zero!\n");
            } else {
                result = num1 / num2;
                printf("Result: %.2lf / %.2lf = %.2lf\n", num1, num2, result);
            }
            break;
        default:
            printf("Error: Invalid operation! Please use +, -, *, or /\n");
            break;
    }
    
    return 0;
}
Результаты работы
(screenshots/task-2-6.png)

Задание 2.7: Високосный год
Постановка задачи
Напишите программу, определяющую, является ли год високосным.

Правила:

Год делится на 4 — високосный

НО: год делится на 100 — не високосный

НО: год делится на 400 — високосный

Список идентификаторов
Имя	Тип	Описание
year	int	Введённый год
Код программы
#include <stdio.h>

int main(void) {
    int year;
    
    printf("Enter year: ");
    scanf("%d", &year);
    
    if (year % 400 == 0) {
        printf("%d is a leap year\n", year);
    } else if (year % 100 == 0) {
        printf("%d is NOT a leap year\n", year);
    } else if (year % 4 == 0) {
        printf("%d is a leap year\n", year);
    } else {
        printf("%d is NOT a leap year\n", year);
    }
    
    return 0;
}
Результаты работы
(screenshots/task-2-7.png)
Задание 2.8: Интеграл методом трапеций
Постановка задачи
Вычислите определённый интеграл методом трапеций

Математическая модель
Метод трапеций: площадь под кривой приближённо равна сумме площадей трапеций.

Формула метода трапеций и аналитическое решение

Список идентификаторов
Имя	Тип	Описание
a	double	Нижний предел интегрирования (0)
b	double	Верхний предел интегрирования (1)
n	int	Количество разбиений (1000)
h	double	Шаг интегрирования
sum	double	Сумма для метода трапеций
integral	double	Численное значение интеграла
exact	double	Точное значение интеграла
Код программы
#include <stdio.h>
#include <math.h>

double f(double x) {
    return exp(x + 2); 
}

int main(void) {
    double a = 0, b = 1; 
    int n = 1000; 
    double h = (b - a) / n;
    
    double sum = (f(a) + f(b)) / 2.0;
    
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);
    }
    
    double integral = h * sum;
    
    printf("Integral of e^(x+2) from %.1f to %.1f:\n", a, b);
    printf("Numerical result: %.6f\n", integral);
    
    double exact = exp(b + 2) - exp(a + 2);
    printf("Exact result: %.6f\n", exact);
    printf("Error: %.6f\n", fabs(integral - exact));
    
    return 0;
}
Результаты работы
(screenshots/task-2-8.png)

Задание 2.9: Интеграл синуса
Постановка задачи
Вычислите интеграл методом трапеций

Математическая модель
Аналитическое решение:

∫ 0 π sin(x)dx=[−cos(x)] 0 π =−cos(π)−(−cos(0))=1+1=2

Список идентификаторов
Имя	Тип	Описание
a	double	Нижний предел интегрирования (0)
b	double	Верхний предел интегрирования (π)
n	int	Количество разбиений (1000)
h	double	Шаг интегрирования
sum	double	Сумма для метода трапеций
integral	double	Численное значение интеграла
exact	double	Точное значение интеграла (2)
Код программы
#include <stdio.h>
#include <math.h>

double f(double x) {
    return sin(x);
}

int main(void) {
    double a = 0, b = M_PI;    
    int n = 1000;              
    double h = (b - a) / n;    
    
    double sum = (f(a) + f(b)) / 2.0;
    
    for (int i = 1; i < n; i++) {
        sum += f(a + i * h);
    }
    
    double integral = h * sum;
    double exact = 2.0;
    
    printf("Integral of sin(x) from 0 to π:\n");
    printf("Numerical result: %.10f\n", integral);
    printf("Exact result:     %.10f\n", exact);
    printf("Error:            %.10f\n", fabs(integral - exact));
    
    printf("\nAnalytical verification:\n");
    printf("-cos(π) = %.6f\n", -cos(M_PI));
    printf("-cos(0) = %.6f\n", -cos(0));
    printf("Result: -cos(π) - (-cos(0)) = %.2f\n", -cos(M_PI) - (-cos(0)));
    
    return 0;
}
Результаты работы
(screenshots/task-2-9.png)