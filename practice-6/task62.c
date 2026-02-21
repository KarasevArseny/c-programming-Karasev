#include <stdio.h>
#include <math.h>
#define PI 3.14159265358979323846
typedef enum {
SHAPE_CIRCLE,
SHAPE_RECTANGLE,
SHAPE_TRIANGLE
} ShapeType;
typedef struct {
double x, y;
} Point;
typedef struct {
Point center;
double radius;
} Circle;
typedef struct {
Point top_left;
double width, height;
} Rectangle;
typedef struct {
Point a, b, c;
} Triangle;
typedef struct {
ShapeType type;
union {
Circle circle;
Rectangle rectangle;
Triangle triangle;
} data;
} Shape;
double area(const Shape *s) {
switch (s->type) {
case SHAPE_CIRCLE:
return PI * s->data.circle.radius * s->data.circle.radius;
case SHAPE_RECTANGLE:
return s->data.rectangle.width * s->data.rectangle.height;
case SHAPE_TRIANGLE: {
Point a = s->data.triangle.a;
Point b = s->data.triangle.b;
Point c = s->data.triangle.c;
return fabs((b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y)) / 2.0;
}
default: return 0;
}
}
double perimeter(const Shape *s) {
switch (s->type) {
case SHAPE_CIRCLE:
return 2 * PI * s->data.circle.radius;
case SHAPE_RECTANGLE:
return 2 * (s->data.rectangle.width + s->data.rectangle.height);
case SHAPE_TRIANGLE: {
Point a = s->data.triangle.a;
Point b = s->data.triangle.b;
Point c = s->data.triangle.c;
double ab = hypot(b.x - a.x, b.y - a.y);
double bc = hypot(c.x - b.x, c.y - b.y);
double ca = hypot(a.x - c.x, a.y - c.y);
return ab + bc + ca;
}
default: return 0;
}
}
void print_shape(const Shape *s) {
switch (s->type) {
case SHAPE_CIRCLE:
printf("Круг: центр (%.2f, %.2f), радиус %.0f\n", 
s->data.circle.center.x, s->data.circle.center.y, 
s->data.circle.radius);
printf("Площадь: %.2f\n", area(s));
printf("Периметр: %.2f\n", perimeter(s));
break;            
case SHAPE_RECTANGLE:
printf("Прямоугольник: (%.2f, %.2f), %.0f x %.0f\n",
s->data.rectangle.top_left.x, s->data.rectangle.top_left.y,
s->data.rectangle.width, s->data.rectangle.height);
printf("Площадь: %.0f\n", area(s));
printf("Периметр: %.0f\n", perimeter(s));
break;            
case SHAPE_TRIANGLE:
printf("Треугольник: (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n",
s->data.triangle.a.x, s->data.triangle.a.y,
s->data.triangle.b.x, s->data.triangle.b.y,
s->data.triangle.c.x, s->data.triangle.c.y);
printf("Площадь: %.2f\n", area(s));
printf("Периметр: %.2f\n", perimeter(s));
break;
}
}
int main(void) {
Shape circle = {SHAPE_CIRCLE, .data.circle = {{0, 0}, 5}};
Shape rectangle = {SHAPE_RECTANGLE, .data.rectangle = {{0, 0}, 4, 3}};
Shape triangle = {SHAPE_TRIANGLE, .data.triangle = {{0, 0}, {4, 0}, {2, 3}}};    
print_shape(&circle);
printf("\n");
print_shape(&rectangle);
printf("\n");
print_shape(&triangle);    
return 0;
}