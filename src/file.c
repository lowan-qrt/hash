// Reads text files, independently of the user interface.
#include <stdio.h>
#include <stdlib.h>

#include "file.h"

char *file_read(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);

    const long size = ftell(file);
    if (size < 0)
    {
        fclose(file);
        return NULL;
    }

    rewind(file);

    char *content = malloc(size + 1);
    if (content == NULL)
    {
        fclose(file);
        return NULL;
    }

    const size_t read_count = fread(content, 1, size, file);

    content[read_count] = '\0';

    fclose(file);

    return content;
}