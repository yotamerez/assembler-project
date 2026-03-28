#include "definitions.h"


/* This function takes a file name and an extension string as input and returns a new string
 * that is the original file name with the extension appended to it. The new string is
 * allocated dynamically and must be freed by the caller. If an error occurs during memory allocation,
 * NULL is returned. 
 */
char* add_extension(char *file_name, char* extension)
{
    /* Allocate memory for the new file name string, including space for the extension and a null terminator. */
    char *new_file_name = malloc(strlen(file_name) + strlen(extension) + 1);

    /* Check if the memory allocation was successful. */
    if (new_file_name == NULL)
    {
        /* Print an error message to stderr. */
        fprintf(stderr, "Error, out of memory\n");

        /* Return NULL to indicate an error. */
        return NULL;
    }

    /* Copy the original file name into the new string. */
    strcpy(new_file_name, file_name);

    /* Append the extension string to the new string. */
    strcat(new_file_name, extension);

    /* Return the new string to the caller. */
    return new_file_name;
}


/*
 * This function takes in a file name and an extension and creates a new file with that name and extension. 
 * It returns a pointer to the newly created file if the creation was successful, or NULL if an error occurred.
 */
FILE* create_new_file(char *file_name, char *extension)
{
    FILE *new_file;

    char *new_file_name;


    /* Add the extension to the file name */
    new_file_name = add_extension(file_name, extension);

    /* Create the new file */
    new_file = fopen(new_file_name, "w+");


    /* Free the memory allocated for the new file name */
    free(new_file_name);


    /* Check if the new file was created successfully */
    if(new_file == NULL)
    {
        fprintf(stderr, "Error, cannot create a new source file for %s\n", file_name);

        return NULL;
    }


    /* Return a pointer to the new file */
    return new_file;
}


/*
 * This function checks whether a file is empty or not by getting the size of the file. 
 * If the file is empty, it prints an error message and returns 1, otherwise, it returns 0.
 */
int check_if_file_is_empty(FILE *file, char *file_name)
{
    unsigned int size;


    /* Move the file pointer to the end of the file */
    fseek(file, 0, SEEK_END);


    /* Get the current position of the file pointer to determine the file size */
    size = ftell(file);


    /* Print an error message if the file is empty and return 1*/
    if(size == 0)
    {
        fprintf(stderr, "Error, the file \"%s\" is empty\n", file_name);

        return 1;

    }


    /* Return 0 if the file is not empty */
    return 0;
}


/*
 * This function takes a file name as input, attempts to open the file in read-only mode, 
 * and returns a pointer to the file if successful. 
 * If the file cannot be opened, it returns NULL and prints an error message to the standard error stream.
 */
FILE* open_source_file(char *file_name)
{

    FILE *file = fopen(file_name, "r");

    /* Cannot open the file, return NULL and print an error message */
    if(file == NULL)
    {
        fprintf(stderr, "Error, cannot open file %s\n", file_name);

        return NULL;
    }

    return file;
}


