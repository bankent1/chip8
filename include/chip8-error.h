/*
 * chip8-error.h
 *
 * Author: Travis Banken
 *
 * Header file for useful error macros and definitions
 */

#define PRINT_ERROR(function, reason, ...) \
    fprintf(stderr, "Error [%s]: ", function); \
    fprintf(stderr, reason, __VA_ARGS__);

#define EXIT_ERROR(function) fprintf(stderr, "[%s] failed, exiting...\n", function);



// ERROR CODES
#define CHIP8_SUCCESS 0
#define CHIP8_ERROR 1

