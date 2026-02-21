#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

// ============ КОНСТАНТЫ ============

#define IMG_MAGIC 0x494D4731  // "IMG1"
#define MAX_PIXEL_VALUE 255

// ============ СТРУКТУРЫ ДАННЫХ ============

typedef struct {
    uint32_t magic;
    uint32_t width;
    uint32_t height;
    uint8_t bits_per_pixel;  // 8 (grayscale), 24 (RGB), 32 (RGBA)
    uint8_t compression;      // 0 = none
    uint16_t reserved;
} ImageHeader;

typedef struct {
    uint8_t r, g, b, a;
} Pixel;

typedef struct {
    ImageHeader header;
    Pixel *pixels;  // Массив пикселей в формате row-major
} Image;

// ============ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ============

// Проверка валидности заголовка
int is_valid_header(const ImageHeader *h) {
    return h->magic == IMG_MAGIC && 
           (h->bits_per_pixel == 8 || h->bits_per_pixel == 24 || h->bits_per_pixel == 32);
}

// Получение индекса пикселя в массиве
int pixel_index(int x, int y, int width) {
    return y * width + x;
}

// Создание пикселя из компонентов
Pixel pixel_create(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    Pixel p = {r, g, b, a};
    return p;
}

// Быстрое насыщение значения (clamping)
uint8_t clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// ============ СОЗДАНИЕ/ЗАГРУЗКА/СОХРАНЕНИЕ ============

// Создание нового изображения
Image* image_create(int width, int height, int bits_per_pixel) {
    if (width <= 0 || height <= 0) {
        printf("Ошибка: неверные размеры изображения\n");
        return NULL;
    }
    
    if (bits_per_pixel != 8 && bits_per_pixel != 24 && bits_per_pixel != 32) {
        printf("Ошибка: неподдерживаемая глубина цвета %d\n", bits_per_pixel);
        return NULL;
    }
    
    Image *img = (Image*)malloc(sizeof(Image));
    if (!img) return NULL;
    
    // Заполняем заголовок
    img->header.magic = IMG_MAGIC;
    img->header.width = width;
    img->header.height = height;
    img->header.bits_per_pixel = bits_per_pixel;
    img->header.compression = 0;
    img->header.reserved = 0;
    
    // Выделяем память под пиксели
    int pixel_count = width * height;
    img->pixels = (Pixel*)malloc(pixel_count * sizeof(Pixel));
    
    if (!img->pixels) {
        free(img);
        return NULL;
    }
    
    // Инициализируем чёрным цветом
    memset(img->pixels, 0, pixel_count * sizeof(Pixel));
    
    printf("Изображение создано: %dx%d, %d бит\n", width, height, bits_per_pixel);
    
    return img;
}

// Загрузка изображения из файла
Image* image_load(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return NULL;
    }
    
    Image *img = (Image*)malloc(sizeof(Image));
    if (!img) {
        fclose(file);
        return NULL;
    }
    
    // Читаем заголовок
    if (fread(&img->header, sizeof(ImageHeader), 1, file) != 1) {
        printf("Ошибка: не удалось прочитать заголовок\n");
        free(img);
        fclose(file);
        return NULL;
    }
    
    // Проверяем заголовок
    if (!is_valid_header(&img->header)) {
        printf("Ошибка: неверный формат файла (magic: 0x%X)\n", img->header.magic);
        free(img);
        fclose(file);
        return NULL;
    }
    
    // Выделяем память под пиксели
    int pixel_count = img->header.width * img->header.height;
    img->pixels = (Pixel*)malloc(pixel_count * sizeof(Pixel));
    
    if (!img->pixels) {
        free(img);
        fclose(file);
        return NULL;
    }
    
    // Читаем пиксели
    if (fread(img->pixels, sizeof(Pixel), pixel_count, file) != pixel_count) {
        printf("Ошибка: не удалось прочитать данные пикселей\n");
        free(img->pixels);
        free(img);
        fclose(file);
        return NULL;
    }
    
    fclose(file);
    
    printf("Изображение загружено: %s, %dx%d, %d бит\n", 
           filename, img->header.width, img->header.height, img->header.bits_per_pixel);
    
    return img;
}

// Сохранение изображения в файл
int image_save(const Image *img, const char *filename) {
    if (!img || !img->pixels) return -1;
    
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Ошибка: не удалось создать файл %s\n", filename);
        return -1;
    }
    
    // Записываем заголовок
    if (fwrite(&img->header, sizeof(ImageHeader), 1, file) != 1) {
        printf("Ошибка: не удалось записать заголовок\n");
        fclose(file);
        return -1;
    }
    
    // Записываем пиксели
    int pixel_count = img->header.width * img->header.height;
    if (fwrite(img->pixels, sizeof(Pixel), pixel_count, file) != pixel_count) {
        printf("Ошибка: не удалось записать пиксели\n");
        fclose(file);
        return -1;
    }
    
    fclose(file);
    printf("Изображение сохранено: %s\n", filename);
    
    return 0;
}

// Освобождение памяти
void image_free(Image *img) {
    if (img) {
        if (img->pixels) free(img->pixels);
        free(img);
        printf("Изображение освобождено\n");
    }
}

// ============ ДОСТУП К ПИКСЕЛЯМ ============

// Получение пикселя с проверкой границ
Pixel image_get_pixel(const Image *img, int x, int y) {
    Pixel black = {0, 0, 0, 255};
    
    if (!img || !img->pixels) return black;
    if (x < 0 || x >= img->header.width || y < 0 || y >= img->header.height) {
        return black;
    }
    
    int idx = pixel_index(x, y, img->header.width);
    return img->pixels[idx];
}

// Установка пикселя с проверкой границ
void image_set_pixel(Image *img, int x, int y, Pixel p) {
    if (!img || !img->pixels) return;
    if (x < 0 || x >= img->header.width || y < 0 || y >= img->header.height) return;
    
    int idx = pixel_index(x, y, img->header.width);
    img->pixels[idx] = p;
}

// ============ ПРЕОБРАЗОВАНИЯ ============

// Инвертирование цветов (негатив)
void image_invert(Image *img) {
    if (!img || !img->pixels) return;
    
    int pixel_count = img->header.width * img->header.height;
    
    for (int i = 0; i < pixel_count; i++) {
        img->pixels[i].r = MAX_PIXEL_VALUE - img->pixels[i].r;
        img->pixels[i].g = MAX_PIXEL_VALUE - img->pixels[i].g;
        img->pixels[i].b = MAX_PIXEL_VALUE - img->pixels[i].b;
        // Альфа-канал не инвертируем
    }
    
    printf("Применён негатив\n");
}

// Преобразование в оттенки серого
void image_grayscale(Image *img) {
    if (!img || !img->pixels) return;
    
    int pixel_count = img->header.width * img->header.height;
    
    for (int i = 0; i < pixel_count; i++) {
        // Формула яркости: 0.299R + 0.587G + 0.114B
        uint8_t gray = (uint8_t)(0.299 * img->pixels[i].r + 
                                  0.587 * img->pixels[i].g + 
                                  0.114 * img->pixels[i].b);
        img->pixels[i].r = gray;
        img->pixels[i].g = gray;
        img->pixels[i].b = gray;
    }
    
    printf("Применено grayscale\n");
}

// Изменение яркости
void image_brightness(Image *img, int delta) {
    if (!img || !img->pixels) return;
    
    int pixel_count = img->header.width * img->header.height;
    
    for (int i = 0; i < pixel_count; i++) {
        img->pixels[i].r = clamp(img->pixels[i].r + delta, 0, MAX_PIXEL_VALUE);
        img->pixels[i].g = clamp(img->pixels[i].g + delta, 0, MAX_PIXEL_VALUE);
        img->pixels[i].b = clamp(img->pixels[i].b + delta, 0, MAX_PIXEL_VALUE);
    }
    
    printf("Яркость изменена на %d\n", delta);
}

// Изменение контрастности
void image_contrast(Image *img, float factor) {
    if (!img || !img->pixels) return;
    
    int pixel_count = img->header.width * img->header.height;
    
    // Находим среднюю яркость
    double avg_r = 0, avg_g = 0, avg_b = 0;
    for (int i = 0; i < pixel_count; i++) {
        avg_r += img->pixels[i].r;
        avg_g += img->pixels[i].g;
        avg_b += img->pixels[i].b;
    }
    avg_r /= pixel_count;
    avg_g /= pixel_count;
    avg_b /= pixel_count;
    
    // Применяем контрастность
    for (int i = 0; i < pixel_count; i++) {
        img->pixels[i].r = clamp((int)(avg_r + (img->pixels[i].r - avg_r) * factor), 0, MAX_PIXEL_VALUE);
        img->pixels[i].g = clamp((int)(avg_g + (img->pixels[i].g - avg_g) * factor), 0, MAX_PIXEL_VALUE);
        img->pixels[i].b = clamp((int)(avg_b + (img->pixels[i].b - avg_b) * factor), 0, MAX_PIXEL_VALUE);
    }
    
    printf("Контрастность изменена в %.2f раз\n", factor);
}

// ============ ГЕОМЕТРИЯ ============

// Обрезка изображения
Image* image_crop(const Image *img, int x, int y, int w, int h) {
    if (!img || !img->pixels) return NULL;
    
    // Проверка границ
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + w > img->header.width) w = img->header.width - x;
    if (y + h > img->header.height) h = img->header.height - y;
    
    if (w <= 0 || h <= 0) return NULL;
    
    // Создаём новое изображение
    Image *cropped = image_create(w, h, img->header.bits_per_pixel);
    if (!cropped) return NULL;
    
    // Копируем пиксели
    for (int dy = 0; dy < h; dy++) {
        for (int dx = 0; dx < w; dx++) {
            Pixel p = image_get_pixel(img, x + dx, y + dy);
            image_set_pixel(cropped, dx, dy, p);
        }
    }
    
    printf("Обрезка выполнена: (%d,%d) %dx%d\n", x, y, w, h);
    
    return cropped;
}

// Изменение размера (простейшая интерполяция - ближайший сосед)
Image* image_resize(const Image *img, int new_width, int new_height) {
    if (!img || !img->pixels || new_width <= 0 || new_height <= 0) return NULL;
    
    Image *resized = image_create(new_width, new_height, img->header.bits_per_pixel);
    if (!resized) return NULL;
    
    float scale_x = (float)img->header.width / new_width;
    float scale_y = (float)img->header.height / new_height;
    
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            int src_x = (int)(x * scale_x);
            int src_y = (int)(y * scale_y);
            if (src_x >= img->header.width) src_x = img->header.width - 1;
            if (src_y >= img->header.height) src_y = img->header.height - 1;
            
            Pixel p = image_get_pixel(img, src_x, src_y);
            image_set_pixel(resized, x, y, p);
        }
    }
    
    printf("Размер изменён: %dx%d -> %dx%d\n", 
           img->header.width, img->header.height, new_width, new_height);
    
    return resized;
}

// Поворот на 90 градусов по часовой стрелке
Image* image_rotate_90(const Image *img) {
    if (!img || !img->pixels) return NULL;
    
    Image *rotated = image_create(img->header.height, img->header.width, img->header.bits_per_pixel);
    if (!rotated) return NULL;
    
    for (int y = 0; y < img->header.height; y++) {
        for (int x = 0; x < img->header.width; x++) {
            Pixel p = image_get_pixel(img, x, y);
            image_set_pixel(rotated, img->header.height - 1 - y, x, p);
        }
    }
    
    printf("Поворот на 90 градусов выполнен\n");
    
    return rotated;
}

// Отражение по горизонтали
void image_flip_horizontal(Image *img) {
    if (!img || !img->pixels) return;
    
    int width = img->header.width;
    int height = img->header.height;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 2; x++) {
            Pixel p1 = image_get_pixel(img, x, y);
            Pixel p2 = image_get_pixel(img, width - 1 - x, y);
            image_set_pixel(img, x, y, p2);
            image_set_pixel(img, width - 1 - x, y, p1);
        }
    }
    
    printf("Отражение по горизонтали выполнено\n");
}

// Отражение по вертикали
void image_flip_vertical(Image *img) {
    if (!img || !img->pixels) return;
    
    int width = img->header.width;
    int height = img->header.height;
    
    for (int y = 0; y < height / 2; y++) {
        for (int x = 0; x < width; x++) {
            Pixel p1 = image_get_pixel(img, x, y);
            Pixel p2 = image_get_pixel(img, x, height - 1 - y);
            image_set_pixel(img, x, y, p2);
            image_set_pixel(img, x, height - 1 - y, p1);
        }
    }
    
    printf("Отражение по вертикали выполнено\n");
}

// ============ РИСОВАНИЕ ============

// Алгоритм Брезенхема для рисования линии
void image_draw_line(Image *img, int x1, int y1, int x2, int y2, Pixel color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        image_set_pixel(img, x1, y1, color);
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Рисование прямоугольника (контур)
void image_draw_rect(Image *img, int x, int y, int w, int h, Pixel color) {
    // Верхняя горизонтальная линия
    image_draw_line(img, x, y, x + w - 1, y, color);
    // Нижняя горизонтальная линия
    image_draw_line(img, x, y + h - 1, x + w - 1, y + h - 1, color);
    // Левая вертикальная линия
    image_draw_line(img, x, y, x, y + h - 1, color);
    // Правая вертикальная линия
    image_draw_line(img, x + w - 1, y, x + w - 1, y + h - 1, color);
}

// Заливка прямоугольника
void image_fill_rect(Image *img, int x, int y, int w, int h, Pixel color) {
    for (int dy = 0; dy < h; dy++) {
        for (int dx = 0; dx < w; dx++) {
            image_set_pixel(img, x + dx, y + dy, color);
        }
    }
}

// ============ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ДЕМОНСТРАЦИИ ============

// Создание градиентного изображения
void create_gradient_image(Image *img) {
    int width = img->header.width;
    int height = img->header.height;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t r = (uint8_t)((float)x / width * MAX_PIXEL_VALUE);
            uint8_t g = (uint8_t)((float)y / height * MAX_PIXEL_VALUE);
            uint8_t b = (uint8_t)((float)(x + y) / (width + height) * MAX_PIXEL_VALUE);
            Pixel p = {r, g, b, 255};
            image_set_pixel(img, x, y, p);
        }
    }
}

// Вывод изображения в текстовом формате (для демонстрации)
void image_print_ascii(const Image *img, int max_width, int max_height) {
    int width = img->header.width;
    int height = img->header.height;
    
    printf("\n=== ASCII представление ===\n");
    printf("Размер: %dx%d\n", width, height);
    
    const char *chars = " .:-=+*#%@";
    int char_count = 10;
    
    int step_x = (width > max_width) ? width / max_width : 1;
    int step_y = (height > max_height) ? height / max_height : 1;
    
    for (int y = 0; y < height; y += step_y) {
        for (int x = 0; x < width; x += step_x) {
            Pixel p = image_get_pixel(img, x, y);
            uint8_t gray = (p.r + p.g + p.b) / 3;
            int idx = gray * (char_count - 1) / MAX_PIXEL_VALUE;
            printf("%c", chars[idx]);
        }
        printf("\n");
    }
}

// ============ ДЕМОНСТРАЦИЯ ============

int main() {
    printf("========================================\n");
    printf("   ПРОСТОЙ ФОРМАТ ИЗОБРАЖЕНИЯ\n");
    printf("========================================\n\n");
    
    // Создаём изображение
    Image *img = image_create(100, 80, 24);
    if (!img) return 1;
    
    // Рисуем градиент
    create_gradient_image(img);
    
    // Рисуем прямоугольник
    Pixel red = {255, 0, 0, 255};
    image_draw_rect(img, 10, 10, 30, 20, red);
    
    // Рисуем закрашенный прямоугольник
    Pixel blue = {0, 0, 255, 255};
    image_fill_rect(img, 50, 30, 20, 15, blue);
    
    // Рисуем линию
    Pixel green = {0, 255, 0, 255};
    image_draw_line(img, 5, 70, 95, 75, green);
    
    // Выводим ASCII представление
    image_print_ascii(img, 60, 40);
    
    // Сохраняем
    image_save(img, "test.img");
    
    // Применяем преобразования
    printf("\n=== ПРЕОБРАЗОВАНИЯ ===\n");
    
    image_invert(img);
    image_save(img, "test_inverted.img");
    
    image_grayscale(img);
    image_save(img, "test_gray.img");
    
    image_brightness(img, 30);
    image_save(img, "test_brighter.img");
    
    // Геометрические преобразования
    Image *cropped = image_crop(img, 20, 20, 50, 40);
    if (cropped) {
        image_save(cropped, "test_cropped.img");
        image_free(cropped);
    }
    
    Image *rotated = image_rotate_90(img);
    if (rotated) {
        image_save(rotated, "test_rotated.img");
        image_free(rotated);
    }
    
    Image *resized = image_resize(img, 50, 40);
    if (resized) {
        image_save(resized, "test_resized.img");
        image_free(resized);
    }
    
    // Зеркальные отражения
    image_flip_horizontal(img);
    image_save(img, "test_flip_h.img");
    
    image_flip_vertical(img);
    image_save(img, "test_flip_v.img");
    
    // Освобождаем память
    image_free(img);
    
    return 0;
}