#include "pre_processor.h"

/* This function creates a new file to hold the text after the macro layout.
   It takes the name of the file to create and its length as input.
   It returns a pointer to the created file. */
FILE* macro_layout(char *file_name)
{
    /* Declare file pointers .*/
    FILE *dest_file, *source_file;

    /* Declare a status variable to keep track of errors. */
    char status;

    char *source_file_name;

    /* The extension for the new file. */
    char *extension = ".am";

    /* The extension for the source file.*/
    char *extension_source_file = ".as";


    /* Add the source file extension to the source file name. */
    source_file_name = add_extension(file_name, extension_source_file);

    /* Open the source file.*/
    source_file = open_source_file(source_file_name);

    free(source_file_name);

    /* If the source file cannot be opened, return NULL. */
    if(source_file == NULL)
    {
        return NULL;
    }


    /* Check if the source file is empty. */
    if(check_if_file_is_empty(source_file, file_name))
    {
        fclose(source_file);

        return NULL;
    }


    /* Rewind the source file. */
    rewind(source_file);

    /* Create the destination file. */
    if((dest_file = create_new_file(file_name, extension)) == NULL)
    {
        fclose(source_file);

        return NULL;
    }


    /* Process the macro layout. */
    status = replace_macro_definitions(source_file, dest_file, file_name);


    /* Close the source file. */
    fclose(source_file);


    /*  If an error occurred during macro layout processing, delete the destination file and return NULL. */
    if(status == 1)
    {
        fclose(dest_file);

        return NULL;
    }

    /* Return a pointer to the destination file. */
    return dest_file;
}


/*
 * This function reads a source file, identifies and processes macro definitions, 
 * and replaces macro names with their corresponding content in the destination file. 
 * It returns an error flag indicating if an error occurred during the macro processing.
 */
int replace_macro_definitions(FILE *source_file, FILE *dest_file, char *file_name)
{
    char line[LENGTH_OF_LINE]; /* a line of text from the source file */

    macro_node *head = NULL; /* the head of the linked list of macro definitions */

    char *macro_name; /* The name of the current macro being processed */

    char **macro_content; /* The content of the current macro being processed */

    char line_number = 0; /* The current line number being processed */

    char error_flag = 0; /* Flag to indicate if an error occurred while processing macros */

    char *token; /* A token extracted from the current line */

    while((fgets(line, LENGTH_OF_LINE-1, source_file)) != NULL)
    {   
        unsigned int len = strlen(line);
 
        char *ptr = strchr(line, '\n'); /* Find pointer to newline character */

        /* Check if line is longer than the buffer size */                
        if(len == LENGTH_OF_LINE - 1 && ptr == NULL)        
        {    
            fprintf(stdout, "Error in file %s.as in line %d: line is longer then 80 characters\n", file_name, line_number);  

            error_flag = 1;
        }

        remove_extra_spaces(line); /* Remove extra spaces from line */

        /* Check if line is a comment or an empty line */
        if(*line == ';' || *line == '\n')
        {
            /* If line does not end with a newline character, read more lines until newline character is found.
               This loop takes care of a comment line with more than 80 characters. */
            while(ptr == NULL)
            {
                fgets(line, LENGTH_OF_LINE-1, source_file);

                ptr = strchr(line, '\n');
            }

            /* Increment line number counter */
            line_number++;

            /* Continue to next iteration of the loop */
            continue;
        }         

        token = strtok(line, " " "\t");

        /*If the first word in the line is "mcr", it's an indicator that the next word is the macro name
          and the macro definition in the next lines until the "endmcr" word is found. */
        if(strcmp(token, "mcr") == 0)
        {           
            token = strtok(NULL, " " "\t");

            /*  Save the macro name. */
            macro_name = get_macro_name_from_file(token, line_number);

            if(!check_if_macro_name_legal(macro_name, line_number, &head))
            {
                error_flag = 1;
            }

            remove_trailing_whitespace(macro_name);

            token = strtok(NULL,  " " "\t" "\n");

            /* Check if there are non-white characters after the macro name. */
            if(token != NULL)
            {
                fprintf(stdout, "note in line %d: there is non white-characters after macro name\n", line_number);               
            }

            /* Save the macro content. */
            macro_content = get_macro_content_from_file(source_file, &line_number, &error_flag);

            /* Create a new macro node in the linked list. */
            add_macro_node(&head, macro_content, macro_name);

        }

            /* The first word of the line is not "mcr". */
        else
        {

            /* If the first token is not a macro name, print the line.
               If the token is a macro name then the function print_macro_content_by_name will print the macro content */
            if(print_macro_content_by_name(token, &head, dest_file) == 0)
            {
                fprintf(dest_file, "%s", token);

                token = strtok(NULL, " " "\t" "\n");

                while(token)
                {
                    fprintf(dest_file, " %s", token);

                    token = strtok(NULL, " " "\t" "\n");
                }

                if(ptr != NULL)
                    fputc('\n', dest_file);
            }

        }

        line_number++;
    }


    /* Free memory used by the linked list of macros. */
    free_macros_memory(head);


    return error_flag;
}



