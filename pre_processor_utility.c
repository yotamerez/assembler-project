#include "pre_processor.h"


/*
 * Check if the given macro name is legal and can be used.
 * Returns 0 if the macro name is legal, otherwise returns 1.
 */
int check_if_macro_name_legal(char *macro_name, int line_number, macro_node **head)
{
    /* Check if macro name is a register name */
    if(is_register_name(macro_name) != -1)
    {
        fprintf(stdout, "Error in line %d: macro name cannot be one of the register names.\n", line_number);

        return 0;
    }

    /* Check if macro name is already used by another macro */
    if(find_macro_by_name(macro_name, head))
    {
        fprintf(stdout, "Error in line %d: macro name '%s' is already used by another macro.\n", line_number, macro_name);

        return 0;
    }

    /* Check if macro name is a directive name */
    if(get_directive_name(macro_name) != NULL)
    {
        fprintf(stdout, "Error in line %d: macro name '%s' cannot be a directive name.\n", line_number, macro_name);

        return 0;
    }

    /* Check if macro name is an operation name */
    if(get_op_name(macro_name) != NULL)
    {
        fprintf(stdout, "Error in line %d: macro name '%s' cannot be an operation name.\n", line_number, macro_name);

        return 0;
    }

    /* Macro name is legal */
    return 1;
}

macro_node* find_macro_by_name(char *name, macro_node **head)
{

    macro_node *temp = *head;


    /* Trim any trailing whitespace from the macro name */
    remove_trailing_whitespace(name);

    /* Check the linked list of macro definitions */
    while(temp != NULL)
    {
        /* If a macro with the same name is found, return a pointer to the node */
        if(strcmp(name, get_macro_name(temp)) == 0)
        {
            return temp;
        }

        temp = get_macro_next(temp);
    }


    /* The macro name is unique */
    return NULL;

}

int print_macro_content_by_name(char *name, macro_node **head, FILE *dest_file)
{
    macro_node *temp = *head;

    int i = 0, found = 0;


    /* trim any trailing whitespace */
    remove_trailing_whitespace(name);

    /* Check each node in the linked list for a macro with the given name */
    while(temp != NULL)
    {    
        if(strcmp(name, get_macro_name(temp)) == 0)
        {   
            char **content = get_macro_content(temp);

            /* If a macro with the given name is found, print its content to the output file */
            while(content[i] != NULL)
            {   
                fprintf(dest_file, "%s", content[i]);

                i++;
            }

            found = 1;

        }

        temp = get_macro_next(temp);
    }

    return found;

}



char** get_macro_content_from_file(FILE *source_file, char *line_number, char *error_status)
{

    /* Initialize variables */
    char** lines = NULL;

    char** new_lines;

    char line[LENGTH_OF_LINE];

    int num_lines = 0;

    int lines_capacity = 0;


    /* Loop through the file lines */
    while(fgets(line, LENGTH_OF_LINE - 1, source_file) != NULL)
    {

        /* Remove extra spaces from the line */
        remove_extra_spaces(line);


        /* Check if the line contains the "endmcr" string */
        if(strstr(line, "endmcr") != NULL)
        {

            /* If the "endmcr" string is followed by non-white characters, print an error message and set the error_status variable to 1*/
            if(has_non_white_chars_after_string(line, "endmcr"))
            {
                fprintf(stdout, "note in line %d: after endmcr there is non white characters\n", *++line_number);                
            }

            break;
        }


        /* Check if the line is empty or a comment; if so, skip it */
        if(*line == '\n' || *line == ';')
        {
            continue;
        }


        /* Ensure there is enough capacity in the lines array for the current line */
        lines = ensure_capacity(lines, num_lines, &lines_capacity);

        if(lines == NULL)
        {
            fprintf(stdout, "Error: out of memory\n");

            free_macro_content(lines, num_lines);

            return NULL;
        }


        /* Allocate memory for the current line and copy it into the lines array */
        lines[num_lines] = safe_malloc(strlen(line) + 1);

        strcpy(lines[num_lines], line);


        /* Increment the line count */
        num_lines++;

        (*line_number)++;

    }


    /* Check if fgets failed and free memory if it did */
    if(ferror(source_file))
    {
        fprintf(stdout, "Error: can't read from file\n");

        free_macro_content(lines, num_lines);

        return NULL;
    }


    /* Add a null terminator to the end of the lines array */
    new_lines = realloc(lines, (num_lines + 1) * sizeof(char*));

    if(new_lines == NULL)
    {
        fprintf(stdout, "Error: out of memory\n");

        exit(EXIT_FAILURE);
    }

    new_lines[num_lines] = NULL;

    lines = new_lines;


    /* Return a pointer to the lines array */
    return lines;
}


char** ensure_capacity(char **lines, int num_lines, int *capacity)
{

    /* Check if there is enough capacity in the array for the required number of elements */
    if (num_lines >= *capacity)
    {

        /* Calculate the new capacity for the array */
        int new_capacity = *capacity + ADD_EXTRA_LINES;

        /* Reallocate the array with the new capacity */
        char **new_lines = realloc(lines, new_capacity * sizeof(char*));

        /* If the reallocation failed, print an error message and return NULL */
        if(new_lines == NULL)
        {
            fprintf(stdout, "Error: out of memory\n");

            exit(EXIT_FAILURE);
        }


        /* Update the capacity of the array and its pointer */
        *capacity = new_capacity;

        lines = new_lines;
    }


    /* Return a pointer to the reallocated or original array */
    return lines;
}

void free_macro_content(char **lines, int num_lines)
{
    int i;

    /* Free each line in the array */
    for (i = 0; i < num_lines; i++)
    {
        free(lines[i]);
    }

    /* Free the lines array itself */
    free(lines);
}

char* get_macro_name_from_file(char *str, int line_number)
{
    int i;

    char name[MAX_LENGTH_OF_MACRO_NAME + 1];

    char *name_copy;


    /* Check if the input string is longer than MAX_LENGTH_OF_MACRO_NAME */
    if(strlen(str) > MAX_LENGTH_OF_MACRO_NAME)
    {
        fprintf(stdout, "Error in line %d: macro name is longer than %d characters.\n", line_number, MAX_LENGTH_OF_MACRO_NAME);
    }


    /* Loop through the characters in the input string until the end of the string
       or MAX_LENGTH_OF_MACRO_NAME - 1 (to leave room for the null terminator) */
    for(i = 0; i < MAX_LENGTH_OF_MACRO_NAME - 1 && str[i] != '\0'; i++)
    {
        name[i] = str[i]; /* Copy the character from the input string to the name array */
    }


    /* Add the null terminator to the end of the name array */
    name[i] = '\0';


    /* Allocate memory for the copy of the name */
    name_copy = safe_malloc(strlen(name) + 1);


    /* Copy the contents of the name array to the name_copy array */
    for (i = 0; i <= strlen(name); i++)
    {
        name_copy[i] = name[i];
    }


    return name_copy;
}

/* Adds a new node to the beginning of the linked list containing macros */
void add_macro_node(macro_node **head, char **macro_content, char *macro_name)
{  
    /* Allocate memory for the new node */
    macro_node *temp = create_macro(macro_name, macro_content);
    
    /* If the linked list is empty, set the head to the new node */
    if(*head == NULL)
    {
        *head = temp;

    } 
   
    else
    {
        set_macro_next(temp, head);

        *head = temp;    
    }  
}



