#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

void logger_log(char* module, char* type, int line, char* message, ...)
{
    va_list args;

    printf("%s: %s (line %d) -> ", module, type, line);

    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    printf("!\n");
}