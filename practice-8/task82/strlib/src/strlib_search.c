#include "strlib.h"
#include <string.h>

int str_find(const String *s, const char *substr) {
    if (!s || !substr) return -1;
    
    const char *pos = strstr(s->data, substr);
    if (pos) {
        return pos - s->data;
    }
    return -1;
}

int str_find_last(const String *s, const char *substr) {
    if (!s || !substr) return -1;
    
    const char *last = NULL;
    const char *p = s->data;
    size_t sub_len = strlen(substr);
    
    while ((p = strstr(p, substr)) != NULL) {
        last = p;
        p += sub_len;
    }
    
    if (last) {
        return last - s->data;
    }
    return -1;
}

int str_find_char(const String *s, char c) {
    if (!s) return -1;
    
    const char *pos = strchr(s->data, c);
    if (pos) {
        return pos - s->data;
    }
    return -1;
}

bool str_contains(const String *s, const char *substr) {
    return str_find(s, substr) != -1;
}

bool str_starts_with(const String *s, const char *prefix) {
    if (!s || !prefix) return false;
    
    size_t prefix_len = strlen(prefix);
    if (prefix_len > s->length) return false;
    
    return strncmp(s->data, prefix, prefix_len) == 0;
}

bool str_ends_with(const String *s, const char *suffix) {
    if (!s || !suffix) return false;
    
    size_t suffix_len = strlen(suffix);
    if (suffix_len > s->length) return false;
    
    return strncmp(s->data + s->length - suffix_len, suffix, suffix_len) == 0;
}

size_t str_count(const String *s, const char *substr) {
    if (!s || !substr) return 0;
    
    size_t count = 0;
    const char *p = s->data;
    size_t sub_len = strlen(substr);
    
    while ((p = strstr(p, substr)) != NULL) {
        count++;
        p += sub_len;
    }
    
    return count;
}