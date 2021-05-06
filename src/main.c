#include <stdio.h>
#include "dictionary.h"

void callback(char* key, void* value)
{
    printf("dict[%s] = %s\n", key, (char*) value);
}

int main()
{
    dictionary d = dictionary_create();

    dictionary_put(&d, "key1", &"value1");
    dictionary_put(&d, "key2", &"value2");
    dictionary_put(&d, "key3", &"value3");
    dictionary_put(&d, "key4", &"value4");
    dictionary_put(&d, "key5", &"value5");

    for_each(d, callback);

    dictionary_free(d);

    return 0;
}