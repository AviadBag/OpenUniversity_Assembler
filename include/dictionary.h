/**
 * This module implements a simple dictionary, using a linked list.
 */

struct s_dictionary_node
{
    char *key;
    void *data;

    struct s_dictionary_node *next;
};

typedef enum e_dictionary_error
{
    NOT_ENOUGH_MEMORY,
    KEY_DOES_NOT_EXIST,
    SUCCESS
} dictionary_error;

typedef struct s_dictionary_node *dictionary;

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
 * @return           NOT_ENOUGH_MEMORY, if it happens. Else - SUCCESS.
 */
dictionary_error dictionary_put(dictionary *d, char *key, void *data);

/**
 * Retrives a value from the given dictionary, by the given key.
 * @param d    The dictionary to retreive from.
 * @param key  The desired key.
 * @param data A pointer to where to put the retreived data in.
 * @return     KEY_DOES_NOT_EXIST, if it happens. Else - SUCCESS.
 */
dictionary_error dictionary_get(dictionary d, char *key, void **data);

/**
 * Releases the memory taken by the given dictionary. (Doesn't realese the data or the keys.)
 * @param d The dictionary to realese.
 */
void dictionary_free(dictionary d);
