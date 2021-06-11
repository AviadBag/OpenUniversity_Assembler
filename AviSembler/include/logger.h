#ifndef _LOGGER_H
#define _LOGGER_H

/**
 * Prints a message to the log.
 * @param module     The name of the module that prints that message. (For example - "parser").
 * @param type       The type of the log message. (For example - "error").
 * @param line       The number of the current line of code.
 * @param message    The message itself.
 * @param argumants  The additional argumants that should be inserted into the message. (printf style)
 */
void logger_log(char* module, char* type, int line, char* message, ...);

#endif