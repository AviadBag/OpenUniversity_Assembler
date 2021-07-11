#include "linked_list.h"

#include <stdio.h>

void callback(void* str)
{
    printf("%s\n", (char*) str);
}

int main()
{
    linked_list lk = linked_list_create();
    linked_list_append(&lk, "Hello, World!");
    linked_list_append(&lk, "My name is Aviad");
    linked_list_append(&lk, "Str 3");
    linked_list_for_each(lk, callback);

    return 0;
}