#include "lib/string.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int memcmp(const void *s1, const void *s2, unsigned int n) {
    const unsigned char *p1 = s1;
    const unsigned char *p2 = s2;

    for (unsigned int i = 0; i < n; i++) {
        if (p1[i] != p2[i])
            return p1[i] - p2[i];
    }

    return 0;
}

int strncmp(const char* s1, const char* s2, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0')
            return (unsigned char)s1[i] - (unsigned char)s2[i];
    }
    return 0;
}

char* strncpy(char* dest, const char* src, unsigned int n) {
    unsigned int i = 0;
    for (; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

char* strchr(const char* str, int c) {
    while (*str) {
        if (*str == (char)c)
            return (char*)str;
        str++;
    }

    if (c == '\0')
        return (char*)str;

    return 0;
}

void* memset(void* dest, int value, unsigned int count) {
    unsigned char* ptr = (unsigned char*)dest;
    for (unsigned int i = 0; i < count; i++) {
        ptr[i] = (unsigned char)value;
    }
    return dest;
}

void* memcpy(void* dest, const void* src, unsigned int count) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    for (unsigned int i = 0; i < count; i++) {
        d[i] = s[i];
    }

    return dest;
}
