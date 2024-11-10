#include "types.h"

int strlen(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* strcpy(char *dest, const char *src) {
    char *start = dest;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return start;
}

char* strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
        --n;
    }
    if (n == 0) return 0;
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

char* strcat(char *dest, const char *src) {
    char *start = dest;
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return start;
}

char* strchr(const char *str, int c) {
    while (*str) {
        if (*str == (char)c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

char* strtok(char *str, const char *delim) {
    static char *last_str = NULL;
    char *token_start;

    // If a new string is passed
    if (str != NULL) {
        last_str = str;
    }

    // If no more tokens
    if (last_str == NULL) {
        return NULL;
    }

    // Skip leading delimiters
    while (*last_str && strchr(delim, *last_str)) {
        last_str++;
    }

    // If we've reached the end of the string
    if (!*last_str) {
        last_str = NULL;
        return NULL;
    }

    // Mark the start of the token
    token_start = last_str;

    // Find the end of the token
    while (*last_str && !strchr(delim, *last_str)) {
        last_str++;
    }

    // Mark the end of the token
    if (*last_str) {
        *last_str = '\0';
        last_str++;
    } else {
        last_str = NULL;
    }

    return token_start;
}