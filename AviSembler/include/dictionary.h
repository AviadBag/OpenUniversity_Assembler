#ifndef _DICTIONARY_H
#define _DICTIONARY_H

/**
 * This module implements a simple dictionary, using a linked list.
 */

struct s_dictionary_node
{
    char *key;                      /* The key of this node */
    void *data;                     /**< The data of this node */

    struct s_dictionary_node *next; /**< A pointer to the next node in the linked list */
};

typedef struct s_dictionary_node *dictionary;

typedef enum e_dictionary_status
{
    DICTIONARY_NOT_ENOUGH_MEMORY,
    DICTIONARY_KEY_DOES_NOT_EXIST,
    DICTIONARY_OK
} dictionary_status;

/**
 * Creates an empty dictionary.
 * @return The empty dictionary.
 */
dictionary dictionary_create();

/**
 * Puts a new entry in the given dictionary.
 * @param dictionary The dictionary to put the new entry in.
 * @param key        The key of the data.
 * @param data       The data itself.
 * @return           NOT_ENOUGH_MEMORY, if it happens. Else - DICTIONARY_OK.
 */
dictionary_status dictionary_put(dictionary *d, char *key, void *data);

/**
 * Retrives a value from the given dictionary, by the given key.
 * @param d    The dictionary to retreive from.
 * @param key  The desired key.
 * @param data A pointer to where to put the address of the retreived data.
 * @return     KEY_DOES_NOT_EXIST, if it happens. Else - DICTIONARY_OK.
 */
dictionary_status dictionary_get(dictionary d, char *key, void **data);

/**
 * Releases the memory taken by the given dictionary. (Doesn't realese the data or the keys.)
 * @param d The dictionary to realese.
 */
void dictionary_free(dictionary d);

/**
 * Calls the given callback with each entry of the table.
 * @param d The dictionary to iterate on.
 * @param callback The callback.
 */
void dictionary_for_each(dictionary d, void (*callback)(char* key, void* data));

#endif