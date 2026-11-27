#ifndef C_PROJECT_FILE_H
#define C_PROJECT_FILE_H

/**
 * Reads the whole content of a text file.
 *
 * The returned string is allocated: the caller must release it with free().
 *
 * @param path path of the file to read
 * @return the file content as a null-terminated string, or NULL on error
 */
char *file_read(const char *path);

/**
 * Writes a string into a text file, replacing its previous content.
 *
 * @param path path of the file to write
 * @param content null-terminated string to write
 * @return 0 on success, -1 on error
 */
int file_write(const char *path, const char *content);

#endif
