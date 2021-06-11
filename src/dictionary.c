#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

/**
 * An important note:
 * In the outer interface for the function, I call "struct s_dictionary_node*" - "dictionary",
 * in order to hide implementation details. But in the implementaion code, I just refer to that
 * like a simple node pointer.
 */

typedef struct s_dictionary_node node;

dictionary dictionary_create()
{
    return (node *)NULL; /* When head=null, the list is empty */
}

dictionary_status dictionary_put(node **nod, char *key, void *data)
{
    /* The new node to add to the end of the list */
    node *new_node = malloc(sizeof(node));
    if (new_node == NULL)
        return DICTIONARY_NOT_ENOUGH_MEMORY;

    new_node->key = key;
    new_node->data = data;

    if (*nod == NULL)
        *nod = new_node;
    else
    {
        /* "tmp" holds the actual value of the head; Not a pointer to it. Makes life simpler. */
        node *tmp = *nod;
        /* Increment nod to point to the last node in the linked list. */
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = new_node;
    }

    return DICTIONARY_OK;
}

dictionary_status dictionary_get(node *nod, char *key, void **data)
{
    while (nod != NULL)
    {
        if (strcmp(nod->key, key) == 0)
        {
            *data = nod->data;
            return DICTIONARY_OK;
        }

        nod = nod->next;
    }

    return DICTIONARY_KEY_DOES_NOT_EXIST;
}

void dictionary_free(node *nod)
{
    while (nod != NULL)
    {
        /* Holds the next node, because I won't be able to access it after I free the current node. */
        node *next = nod->next;
        free(nod);
        nod = next;
    }
}

void dictionary_for_each(node* nod, void (*callback)(char* key, void* data))
{
    while (nod != NULL)
    {
        callback(nod->key, nod->data);

        nod = nod->next;
    }
}