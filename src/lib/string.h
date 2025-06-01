#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
int memcmp(const void *s1, const void *s2, unsigned int n);
int strncmp(const char* s1, const char* s2, unsigned int n);
char* strncpy(char* dest, const char* src, unsigned int n);
char* strchr(const char* str, int c);
void* memset(void* dest, int value, unsigned int count);
void* memcpy(void* dest, const void* src, unsigned int count);

#endif
