#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "boolean.h"

/* This module defines the symbol sturct, with represents a single symbol in the symbols table */

typedef enum e_symbol_type
{
    CODE,
    DATA,
    EXTERNAL
} symbol_type;

typedef struct s_symbol
{
    char* name;       /**< The name of the symbol */
    symbol_type type; /**< The type of the symbol */
    boolean is_entry; /**< Is this symbol defined as entry? */
} symbol;

#endif