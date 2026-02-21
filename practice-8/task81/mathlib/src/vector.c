#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Vector* vec_create(size_t size) {
    Vector *v = (Vector*)malloc(sizeof(Vector));
    if (!v) return NULL;
    
    v->size = size;
    v->data = (double*)calloc(size, sizeof(double));
    
    if (!v->data) {
        free(v);
        return NULL;
    }
    
    return v;
}

Vector* vec_from_array(const double *arr, size_t size) {
    Vector *v = vec_create(size);
    if (!v) return NULL;
    
    memcpy(v->data, arr, size * sizeof(double));
    return v;
}

void vec_free(Vector *v) {
    if (v) {
        free(v->data);
        free(v);
    }
}

Vector* vec_copy(const Vector *v) {
    if (!v) return NULL;
    return vec_from_array(v->data, v->size);
}

int vec_add(Vector *result, const Vector *a, const Vector *b) {
    if (!result || !a || !b) return -1;
    if (result->size != a->size || a->size != b->size) return -1;
    
    for (size_t i = 0; i < a->size; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
    
    return 0;
}

int vec_sub(Vector *result, const Vector *a, const Vector *b) {
    if (!result || !a || !b) return -1;
    if (result->size != a->size || a->size != b->size) return -1;
    
    for (size_t i = 0; i < a->size; i++) {
        result->data[i] = a->data[i] - b->data[i];
    }
    
    return 0;
}

void vec_scale(Vector *v, double scalar) {
    if (!v) return;
    
    for (size_t i = 0; i < v->size; i++) {
        v->data[i] *= scalar;
    }
}

double vec_dot(const Vector *a, const Vector *b) {
    if (!a || !b || a->size != b->size) return 0;
    
    double result = 0;
    for (size_t i = 0; i < a->size; i++) {
        result += a->data[i] * b->data[i];
    }
    
    return result;
}

double vec_norm(const Vector *v) {
    if (!v) return 0;
    return sqrt(vec_dot(v, v));
}

int vec_normalize(Vector *v) {
    if (!v) return -1;
    
    double norm = vec_norm(v);
    if (norm < 1e-10) return -1;
    
    vec_scale(v, 1.0 / norm);
    return 0;
}

void vec_print(const Vector *v) {
    if (!v) return;
    
    printf("[");
    for (size_t i = 0; i < v->size; i++) {
        printf("%.4f", v->data[i]);
        if (i < v->size - 1) printf(", ");
    }
    printf("]");
}