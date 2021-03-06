#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "boolean.h"
#include "linked_list.h"

/* This module defines the symbol sturct, with represents a single symbol in the symbols table */

#define LABEL_MAX_LENGTH 31

typedef enum e_symbol_type
{
    CODE,
    DATA,
    EXTERNAL
} symbol_type;

typedef struct s_symbol
{
    char name[LABEL_MAX_LENGTH + 1];   /**< The name of the symbol. (+1 for the null terminator) */
    symbol_type type;                  /**< The type of the symbol */
    unsigned long value;               /**< The address of the symbol */
    boolean is_entry;                  /**< Is this symbol defined as entry? */
    linked_list instructions_using_me; /**< What instructions are using me (Type is like value's type)? ONLY USED WHEN type=EXTERNAL */
} symbol;

#endif