// Reads text files, independently of the user interface.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int file_write(const char *path, const char *content)
{
    FILE *file = fopen(path, "wb");
    if (file == NULL) return -1;

    const size_t length = strlen(content);

    const size_t written_count = fwrite(content, 1, length, file);

    const int close_result = fclose(file);

    if (written_count != length || close_result != 0) return -1;

    return 0;
}