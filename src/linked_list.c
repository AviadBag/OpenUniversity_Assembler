#include "linked_list.h"

#include <stdlib.h>
#include <string.h>

/**
 * An important note:
 * In the outer interface for the function, I call "node*" - "linked_list",
 * in order to hide implementation details. But in the implementaion code, I just refer to that
 * like a simple node pointer.
 */

linked_list linked_list_create()
{
    return (node *)NULL; /* When head=null, the list is empty */
}

linked_list_status linked_list_append(node **nod, void *data)
{
    /* The new node to add to the end of the list */
    node *new_node = malloc(sizeof(node));
    if (new_node == NULL)
        return LINKED_LIST_NOT_ENOUGH_MEMORY;
    memset(new_node, 0, sizeof(node));

    new_node->data = data;
    new_node->next = NULL;

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

    return LINKED_LIST_OK;
}

void linked_list_free(node *nod)
{
    while (nod != NULL)
    {
        /* Holds the next node, because I won't be able to access it after I free the current node. */
        node *next = nod->next;
        free(nod);
        nod = next;
    }
}

void linked_list_free_elements(node *nod) 
{
    while (nod != NULL)
    {
        free(nod->data);
        nod = nod->next;
    }
}

int linked_list_length(node* nod)
{
    int count = 0;

    while (nod != NULL)
    {
        count++;
        nod = nod->next;
    }

    return count;
}

void* linked_list_get(node* nod, int index)
{   
    int count = 0;

    while (nod != NULL)
    {
        if (count == index)
            return nod->data;

        count++;
        nod = nod->next;
    }

    return NULL;
}