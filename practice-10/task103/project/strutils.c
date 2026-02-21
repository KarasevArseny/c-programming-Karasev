#include "strutils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *allocate_string_copy(const char *s, size_t len) {
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    if (s) {
        memcpy(result, s, len);
        result[len] = '\0';
    } else {
        result[0] = '\0';
    }
    
    return result;
}

char *str_reverse(const char *s) {
    if (!s) return NULL;
    
    size_t len = strlen(s);
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = s[len - 1 - i];
    }
    result[len] = '\0';
    
    return result;
}

char *str_to_upper(const char *s) {
    if (!s) return NULL;
    
    size_t len = strlen(s);
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = toupper(s[i]);
    }
    result[len] = '\0';
    
    return result;
}

char *str_to_lower(const char *s) {
    if (!s) return NULL;
    
    size_t len = strlen(s);
    char *result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(s[i]);
    }
    result[len] = '\0';
    
    return result;
}

int str_word_count(const char *s) {
    if (!s) return 0;
    
    int count = 0;
    int in_word = 0;
    
    while (*s) {
        if (isspace(*s)) {
            in_word = 0;
        } else {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        }
        s++;
    }
    
    return count;
}

char *str_trim(const char *s) {
    if (!s) return NULL;
    
    const char *start = s;
    while (*start && isspace(*start)) {
        start++;
    }
    
    if (*start == '\0') {
        return allocate_string_copy("", 0);
    }
    
    const char *end = s + strlen(s) - 1;
    while (end > start && isspace(*end)) {
        end--;
    }
    
    size_t len = end - start + 1;
    return allocate_string_copy(start, len);
}