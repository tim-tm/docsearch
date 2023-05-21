#include <stdio.h>

#define SENGINE_IMPL
#include "sengine.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Syntax: docsearch (directory) (query)\n");
        return 1;
    }
    char* dir = argv[1];
    char* name = argv[2];
    printf("%s, %s\n", dir, name);

    SearchResult* result = engine_init(dir, name);      
    engine_find_first(result);
    engine_print_data(result);
    engine_destroy(result);
    return 0;
}
