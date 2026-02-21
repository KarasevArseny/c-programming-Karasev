#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "fileutils.h"

void test_path_functions() {
    printf("Тестирование функций работы с путями...\n");
    
    // path_join
    char *joined = path_join("/home/user", "docs/file.txt");
    assert(strcmp(joined, "/home/user/docs/file.txt") == 0);
    free(joined);
    
    joined = path_join("/home/user/", "/docs/file.txt");
    assert(strcmp(joined, "/home/user/docs/file.txt") == 0);
    free(joined);
    
    // path_dirname
    char *dir = path_dirname("/home/user/docs/file.txt");
    assert(strcmp(dir, "/home/user/docs") == 0);
    free(dir);
    
    dir = path_dirname("/usr/local/bin/");
    assert(strcmp(dir, "/usr/local") == 0);
    free(dir);
    
    // path_basename
    char *base = path_basename("/home/user/docs/file.txt");
    assert(strcmp(base, "file.txt") == 0);
    free(base);
    
    base = path_basename("/usr/local/bin/");
    assert(strcmp(base, "bin") == 0);
    free(base);
    
    // path_extension
    char *ext = path_extension("/home/user/docs/file.txt");
    assert(strcmp(ext, ".txt") == 0);
    free(ext);
    
    ext = path_extension("file_without_extension");
    assert(strcmp(ext, "") == 0);
    free(ext);
    
    // path_without_extension
    char *no_ext = path_without_extension("/home/user/docs/file.txt");
    assert(strcmp(no_ext, "file") == 0);
    free(no_ext);
    
    no_ext = path_without_extension("file_without_extension");
    assert(strcmp(no_ext, "file_without_extension") == 0);
    free(no_ext);
    
    // path_normalize
    char *norm = path_normalize("/home//user///docs/./file.txt");
    assert(strcmp(norm, "/home/user/docs/file.txt") == 0);
    free(norm);
    
    norm = path_normalize("/usr/local/bin/");
    assert(strcmp(norm, "/usr/local/bin") == 0);
    free(norm);
    
    // path_is_absolute
    assert(path_is_absolute("/home/user") == true);
    assert(path_is_absolute("relative/path") == false);
    
    printf("✓ Функции работы с путями прошли тест\n");
}

void test_io_functions() {
    printf("Тестирование функций ввода/вывода...\n");
    
    const char *test_file = "test_io.txt";
    const char *test_content = "Line 1\nLine 2\nLine 3\n";
    
    // Запись
    assert(file_write_all(test_file, test_content) == 0);
    
    // Существование
    assert(file_exists(test_file) == true);
    
    // Размер
    long size = file_size(test_file);
    assert(size > 0);
    
    // Чтение целиком
    size_t read_size;
    char *content = file_read_all(test_file, &read_size);
    assert(content != NULL);
    assert(read_size == strlen(test_content));
    assert(strcmp(content, test_content) == 0);
    free(content);
    
    // Построчное чтение
    size_t line_count;
    char **lines = file_read_lines(test_file, &line_count);
    assert(lines != NULL);
    assert(line_count == 3);
    assert(strcmp(lines[0], "Line 1") == 0);
    assert(strcmp(lines[1], "Line 2") == 0);
    assert(strcmp(lines[2], "Line 3") == 0);
    file_free_lines(lines, line_count);
    
    // Дописывание
    assert(file_append(test_file, "Line 4\n") == 0);
    
    lines = file_read_lines(test_file, &line_count);
    assert(line_count == 4);
    file_free_lines(lines, line_count);
    
    // Копирование
    const char *copy_file = "test_copy.txt";
    assert(file_copy(test_file, copy_file) == 0);
    assert(file_exists(copy_file) == true);
    
    // Перемещение
    const char *moved_file = "test_moved.txt";
    assert(file_move(copy_file, moved_file) == 0);
    assert(file_exists(copy_file) == false);
    assert(file_exists(moved_file) == true);
    
    // Удаление
    assert(file_delete(test_file) == 0);
    assert(file_delete(moved_file) == 0);
    assert(file_exists(test_file) == false);
    assert(file_exists(moved_file) == false);
    
    printf("✓ Функции ввода/вывода прошли тест\n");
}

int main() {
    printf("========================================\n");
    printf("   ТЕСТИРОВАНИЕ БИБЛИОТЕКИ FILEUTILS\n");
    printf("========================================\n\n");
    
    test_path_functions();
    test_io_functions();
    
    printf("\n✅ Все тесты пройдены успешно!\n");
    
    return 0;
}