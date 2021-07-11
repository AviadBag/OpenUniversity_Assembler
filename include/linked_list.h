#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

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
 * Calls the given callback with each entry of the table.
 * @param l        The linked_list to iterate on.
 * @param callback The callback.
 */
void linked_list_for_each(linked_list l, void (*callback)(void *data));

#endif