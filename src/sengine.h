#ifndef SENGINE_H
#define SENGINE_H
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

typedef struct SearchResult {
    char dir[1024];
    char name[1024];

    char data[1024];
} SearchResult;

SearchResult* engine_init(char* dir, char* name);
int engine_find_first(SearchResult* result);
void engine_destroy(SearchResult* result);
void engine_print_data(SearchResult* result);

#ifdef SENGINE_IMPL

SearchResult* engine_init(char* dir, char* name)
{
    SearchResult* result = malloc(sizeof(char) * 1024 * 3 + sizeof(size_t));
    strcpy(result->dir, dir);
    strcpy(result->name, name);
    return result;
}

// TODO: Better search algorithm, not based on occurrences.
int engine_find_first(SearchResult* result)
{
    char* startDir = result->dir;
    DIR* dirP = opendir(result->dir);
    if (dirP == NULL) return 0;

    struct dirent* dirent;
    float percent = 0;
    while ((dirent = readdir(dirP)) != NULL)
    {
        if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0) continue;
        
        char* name = calloc(1024, sizeof(char));
        strcpy(name, startDir);
        name = strcat(name, "/");
        name = strcat(name, dirent->d_name);

        DIR* innerDir = opendir(name);
        free(name);
        if (innerDir == NULL) continue;
        
        struct dirent* innerDirent;
        while ((innerDirent = readdir(innerDir)) != NULL)
        {
            if (strcmp(innerDirent->d_name, ".") == 0 || strcmp(innerDirent->d_name, "..") == 0) continue;
            
            char* split = strtok(innerDirent->d_name, ".");
            if (split == NULL) continue;

            size_t len = strlen(innerDirent->d_name);
            size_t occurrences = 0;
            for (size_t i = 0; i < len; ++i)
            {
                for (size_t j = 0; j < strlen(result->name); ++j)
                {
                    if (innerDirent->d_name[i] == result->name[j])
                    {
                        ++occurrences;
                    }
                }
            }

            float newPerc = occurrences / len;
            if (newPerc > percent) {
                percent = newPerc;
                strcpy(result->data, innerDirent->d_name);
            }
            
            /*if (strcasestr(innerDirent->d_name, result->name) != NULL) {
                strcpy(result->data, innerDirent->d_name);
            }*/
        }
        closedir(innerDir);
    }
    closedir(dirP);
    return 1;
}

void engine_destroy(SearchResult* result)
{
    free(result);
}

void engine_print_data(SearchResult* result)
{
    printf("%s\n", result->data);
}

#endif // SENGINE_IMPL
#endif // SENGINE_H
