#ifndef PRINTF_CONFIG_H
#define PRINTF_CONFIG_H
// --- Configuration for mpaland/printf library optimization ---
// These defines control various features and buffer sizes for the printf implementation.


// ntoa (integer) conversion buffer size. This must be big enough to hold one
// converted numeric number including leading zeros. Normally 32 is a sufficient value.
// Created on the stack.
#define PRINTF_NTOA_BUFFER_SIZE                 32

// ftoa (float) conversion buffer size. This must be big enough to hold one
// converted float number including leading zeros. Normally 32 is a sufficient value.
// Created on the stack.
#define PRINTF_FTOA_BUFFER_SIZE                 32

// Define the default floating point precision.
#define PRINTF_DEFAULT_FLOAT_PRECISION          6

// Define the largest suitable value to be printed with %f, before using
// exponential representation.
#define PRINTF_MAX_FLOAT                        1e9

// Define this to disable floating point (%f) support.
// Uncomment this if you want to disable float support to save flash.
// #define PRINTF_DISABLE_SUPPORT_FLOAT

// Define this to disable exponential floating point (%e) support.
// Uncomment this if you want to disable exponential support for floats.
// #define PRINTF_DISABLE_SUPPORT_EXPONENTIAL

// Define this to disable long long (%ll) support.
// Uncomment this if you want to disable 64-bit integer support to save flash.
// #define PRINTF_DISABLE_SUPPORT_LONG_LONG

// Define this to disable ptrdiff_t (%t) support.
// Uncomment this if you want to disable ptrdiff_t support to save flash.
// #define PRINTF_DISABLE_SUPPORT_PTRDIFF_T


#endif // PRINTF_CONFIG_H
