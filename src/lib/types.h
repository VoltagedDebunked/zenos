#ifndef TYPES_H
#define TYPES_H

// Unsigned integer types
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

// Signed integer types
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef signed long long    int64_t;

// Size type
typedef unsigned int        size_t;

typedef unsigned int uintptr_t;

// Boolean type
typedef enum {
    false = 0,
    true = 1
} bool;

#define NULL 0


#endif // TYPES_H