#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include "boolean.h"

/**
 * This module implements a simple linked list.
 */

typedef struct s_node
{
    void *data; /**< The data of this node */
    struct s_node *next; /**< A pointer to the next node in the linked list */
} node;

typedef node *linked_list;

typedef enum e_linked_list_status
{
    LINKED_LIST_NOT_ENOUGH_MEMORY,
    LINKED_LIST_KEY_DOES_NOT_EXIST,
    LINKED_LIST_OK
} linked_list_status;

/**
 * Creates an empty linked_list.
 * @return The empty linked_list.
 */
linked_list linked_list_create();

/**
 * Appends a new entry to the end of the given linked_list.
 * @param linked_list The linked_list to put the new entry in.
 * @param data        The data to append.
 * @return            LINKED_LIST_NOT_ENOUGH_MEMORY or LINKED_LIST_OK.
 */
linked_list_status linked_list_append(linked_list *d, void *data);

/**
 * Releases the memory taken by the given linked_list. (Doesn't realese the data.)
 * @param l The linked_list to realese.
 */
void linked_list_free(linked_list l);

/**
 * @brief Returns the length of the given linked list
 * 
 * @param l    The linked list
 * @return int The length
 */
int linked_list_length(linked_list l);

/**
 * @brief Returns linked_list[index]
 * 
 * @param index  The index
 * @param l      The linked list.
 * @return void* The data at linked_list[index]. If the index is out of bounds - returns null.
 */
void* linked_list_get(linked_list l, int index);

#endif