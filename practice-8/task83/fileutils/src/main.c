#include <stdio.h>
#include "fileutils.h"

void print_separator() {
    printf("--------------------------------------------------\n");
}

void demo_path_functions() {
    printf("\n=== ФУНКЦИИ ДЛЯ РАБОТЫ С ПУТЯМИ ===\n");
    print_separator();
    
    const char *paths[] = {
        "/home/user/docs/file.txt",
        "/usr/local/bin/",
        "relative/path/file.c",
        "C:\\Windows\\System32\\cmd.exe",
        "file_without_extension"
    };
    
    for (size_t i = 0; i < sizeof(paths)/sizeof(paths[0]); i++) {
        const char *p = paths[i];
        printf("\nПуть: %s\n", p);
        
        char *dir = path_dirname(p);
        char *base = path_basename(p);
        char *ext = path_extension(p);
        char *no_ext = path_without_extension(p);
        char *norm = path_normalize(p);
        
        printf("  dirname:  %s\n", dir);
        printf("  basename: %s\n", base);
        printf("  extension: %s\n", ext);
        printf("  without ext: %s\n", no_ext);
        printf("  normalized: %s\n", norm);
        printf("  is_absolute: %s\n", path_is_absolute(p) ? "да" : "нет");
        
        free(dir);
        free(base);
        free(ext);
        free(no_ext);
        free(norm);
    }
    
    // Демонстрация path_join
    printf("\nДемонстрация path_join:\n");
    char *joined1 = path_join("/home/user", "docs/file.txt");
    char *joined2 = path_join("/home/user/", "/docs/file.txt");
    char *joined3 = path_join("/home/user", "");
    char *joined4 = path_join("", "file.txt");
    
    printf("  join('/home/user', 'docs/file.txt'): %s\n", joined1);
    printf("  join('/home/user/', '/docs/file.txt'): %s\n", joined2);
    printf("  join('/home/user', ''): %s\n", joined3);
    printf("  join('', 'file.txt'): %s\n", joined4);
    
    free(joined1);
    free(joined2);
    free(joined3);
    free(joined4);
}

void demo_io_functions() {
    printf("\n=== ФУНКЦИИ ВВОДА/ВЫВОДА ===\n");
    print_separator();
    
    const char *test_file = "test.txt";
    
    // Запись в файл
    printf("Запись в файл '%s'...\n", test_file);
    const char *content = "Строка 1\nСтрока 2\nСтрока 3\nСтрока 4\n";
    if (file_write_all(test_file, content) == 0) {
        printf("  Запись выполнена успешно\n");
    } else {
        printf("  Ошибка записи\n");
    }
    
    // Проверка существования
    printf("Файл существует: %s\n", file_exists(test_file) ? "да" : "нет");
    
    // Размер файла
    long size = file_size(test_file);
    printf("Размер файла: %ld байт\n", size);
    
    // Чтение файла целиком
    printf("\nЧтение файла целиком:\n");
    size_t read_size;
    char *content_read = file_read_all(test_file, &read_size);
    if (content_read) {
        printf("  Прочитано %zu байт:\n", read_size);
        printf("  ---\n%s---\n", content_read);
        free(content_read);
    }
    
    // Построчное чтение
    printf("\nПострочное чтение:\n");
    size_t line_count;
    char **lines = file_read_lines(test_file, &line_count);
    if (lines) {
        printf("  Прочитано %zu строк:\n", line_count);
        for (size_t i = 0; i < line_count; i++) {
            printf("  [%zu] '%s'\n", i, lines[i]);
        }
        file_free_lines(lines, line_count);
    }
    
    // Дописывание в файл
    printf("\nДописывание в файл...\n");
    if (file_append(test_file, "Строка 5\n") == 0) {
        printf("  Дописывание выполнено\n");
    }
    
    // Проверка после дописывания
    lines = file_read_lines(test_file, &line_count);
    if (lines) {
        printf("  После дописывания: %zu строк\n", line_count);
        file_free_lines(lines, line_count);
    }
    
    // Копирование файла
    const char *copy_file = "test_copy.txt";
    printf("\nКопирование в '%s'...\n", copy_file);
    if (file_copy(test_file, copy_file) == 0) {
        printf("  Копирование выполнено\n");
        
        // Проверка копии
        size_t copy_size;
        char *copy_content = file_read_all(copy_file, &copy_size);
        if (copy_content) {
            printf("  Размер копии: %zu байт\n", copy_size);
            free(copy_content);
        }
    }
    
    // Перемещение файла
    const char *moved_file = "test_moved.txt";
    printf("\nПеремещение в '%s'...\n", moved_file);
    if (file_move(copy_file, moved_file) == 0) {
        printf("  Перемещение выполнено\n");
        printf("  Исходный файл существует: %s\n", 
               file_exists(copy_file) ? "да" : "нет");
        printf("  Новый файл существует: %s\n", 
               file_exists(moved_file) ? "да" : "нет");
    }
    
    // Удаление файлов
    printf("\nУдаление файлов...\n");
    file_delete(test_file);
    file_delete(moved_file);
    printf("  Все файлы удалены\n");
}

void demo_path_info() {
    printf("\n=== ИНФОРМАЦИЯ О ПУТЯХ ===\n");
    print_separator();
    
    // Проверка существующих файлов
    printf("Проверка существующих файлов/директорий:\n");
    
    // Текущая директория
    printf("  '.' существует: %s\n", path_exists(".") ? "да" : "нет");
    printf("  '.' является директорией: %s\n", 
           path_is_directory(".") ? "да" : "нет");
    printf("  '.' является файлом: %s\n", 
           path_is_file(".") ? "да" : "нет");
    
    // Проверка несуществующего пути
    printf("  'nonexistent' существует: %s\n", 
           path_exists("nonexistent") ? "да" : "нет");
}

int main() {
    printf("========================================\n");
    printf("   БИБЛИОТЕКА ФАЙЛОВЫХ УТИЛИТ\n");
    printf("========================================\n");
    
    demo_path_functions();
    demo_io_functions();
    demo_path_info();
    
    printf("\n========================================\n");
    printf("   ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА\n");
    printf("========================================\n");
    
    return 0;
}