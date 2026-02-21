#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Matrix* mat_create(size_t rows, size_t cols) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    if (!m) return NULL;
    
    m->rows = rows;
    m->cols = cols;
    m->data = (double*)calloc(rows * cols, sizeof(double));
    
    if (!m->data) {
        free(m);
        return NULL;
    }
    
    return m;
}

Matrix* mat_identity(size_t n) {
    Matrix *m = mat_create(n, n);
    if (!m) return NULL;
    
    for (size_t i = 0; i < n; i++) {
        m->data[i * n + i] = 1.0;
    }
    
    return m;
}

void mat_free(Matrix *m) {
    if (m) {
        free(m->data);
        free(m);
    }
}

Matrix* mat_copy(const Matrix *m) {
    if (!m) return NULL;
    
    Matrix *copy = mat_create(m->rows, m->cols);
    if (!copy) return NULL;
    
    memcpy(copy->data, m->data, m->rows * m->cols * sizeof(double));
    return copy;
}

double mat_get(const Matrix *m, size_t row, size_t col) {
    if (!m || row >= m->rows || col >= m->cols) return 0;
    return m->data[row * m->cols + col];
}

void mat_set(Matrix *m, size_t row, size_t col, double value) {
    if (!m || row >= m->rows || col >= m->cols) return;
    m->data[row * m->cols + col] = value;
}

int mat_add(Matrix *result, const Matrix *a, const Matrix *b) {
    if (!result || !a || !b) return -1;
    if (result->rows != a->rows || result->cols != a->cols ||
        a->rows != b->rows || a->cols != b->cols) return -1;
    
    size_t total = a->rows * a->cols;
    for (size_t i = 0; i < total; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
    
    return 0;
}

int mat_sub(Matrix *result, const Matrix *a, const Matrix *b) {
    if (!result || !a || !b) return -1;
    if (result->rows != a->rows || result->cols != a->cols ||
        a->rows != b->rows || a->cols != b->cols) return -1;
    
    size_t total = a->rows * a->cols;
    for (size_t i = 0; i < total; i++) {
        result->data[i] = a->data[i] - b->data[i];
    }
    
    return 0;
}

int mat_mul(Matrix *result, const Matrix *a, const Matrix *b) {
    if (!result || !a || !b) return -1;
    if (a->cols != b->rows) return -1;
    if (result->rows != a->rows || result->cols != b->cols) return -1;
    
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }
    
    return 0;
}

void mat_scale(Matrix *m, double scalar) {
    if (!m) return;
    
    size_t total = m->rows * m->cols;
    for (size_t i = 0; i < total; i++) {
        m->data[i] *= scalar;
    }
}

Matrix* mat_transpose(const Matrix *m) {
    if (!m) return NULL;
    
    Matrix *t = mat_create(m->cols, m->rows);
    if (!t) return NULL;
    
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            t->data[j * t->cols + i] = m->data[i * m->cols + j];
        }
    }
    
    return t;
}

// Вспомогательная рекурсивная функция для вычисления определителя
static double det_recursive(const double *data, size_t n, size_t stride) {
    if (n == 1) return data[0];
    if (n == 2) return data[0] * data[stride + 1] - data[1] * data[stride];
    
    double det = 0;
    int sign = 1;
    
    for (size_t k = 0; k < n; k++) {
        // Создаём минор
        double *minor = (double*)malloc((n - 1) * (n - 1) * sizeof(double));
        if (!minor) return 0;
        
        for (size_t i = 1; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                if (j < k) {
                    minor[(i - 1) * (n - 1) + j] = data[i * stride + j];
                } else if (j > k) {
                    minor[(i - 1) * (n - 1) + (j - 1)] = data[i * stride + j];
                }
            }
        }
        
        det += sign * data[k] * det_recursive(minor, n - 1, n - 1);
        sign = -sign;
        
        free(minor);
    }
    
    return det;
}

double mat_determinant(const Matrix *m) {
    if (!m || m->rows != m->cols) return 0;
    return det_recursive(m->data, m->rows, m->cols);
}

Matrix* mat_inverse(const Matrix *m) {
    if (!m || m->rows != m->cols) return NULL;
    
    size_t n = m->rows;
    double det = mat_determinant(m);
    
    if (fabs(det) < 1e-10) {
        printf("Ошибка: матрица вырождена, определитель = %.10f\n", det);
        return NULL;
    }
    
    // Создаём расширенную матрицу [A | I]
    double *augmented = (double*)malloc(n * 2 * n * sizeof(double));
    if (!augmented) return NULL;
    
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            augmented[i * (2 * n) + j] = m->data[i * n + j];
            augmented[i * (2 * n) + n + j] = (i == j) ? 1.0 : 0.0;
        }
    }
    
    // Метод Гаусса-Жордана
    for (size_t i = 0; i < n; i++) {
        // Поиск максимального элемента в столбце для устойчивости
        size_t max_row = i;
        for (size_t k = i + 1; k < n; k++) {
            if (fabs(augmented[k * (2 * n) + i]) > 
                fabs(augmented[max_row * (2 * n) + i])) {
                max_row = k;
            }
        }
        
        // Перестановка строк
        if (max_row != i) {
            for (size_t j = 0; j < 2 * n; j++) {
                double temp = augmented[i * (2 * n) + j];
                augmented[i * (2 * n) + j] = augmented[max_row * (2 * n) + j];
                augmented[max_row * (2 * n) + j] = temp;
            }
        }
        
        // Нормализация текущей строки
        double pivot = augmented[i * (2 * n) + i];
        for (size_t j = 0; j < 2 * n; j++) {
            augmented[i * (2 * n) + j] /= pivot;
        }
        
        // Обнуление других строк
        for (size_t k = 0; k < n; k++) {
            if (k != i) {
                double factor = augmented[k * (2 * n) + i];
                for (size_t j = 0; j < 2 * n; j++) {
                    augmented[k * (2 * n) + j] -= factor * augmented[i * (2 * n) + j];
                }
            }
        }
    }
    
    // Извлекаем обратную матрицу
    Matrix *inv = mat_create(n, n);
    if (inv) {
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                inv->data[i * n + j] = augmented[i * (2 * n) + n + j];
            }
        }
    }
    
    free(augmented);
    return inv;
}

void mat_print(const Matrix *m) {
    if (!m) return;
    
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.4f ", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}