#include "definitions.h"



/*
 * This function takes a string as input and checks if it is a valid register name.
 * A valid register name is a string of exactly 2 characters, where the first character is 'r'
 * and the second character is a digit between 0 and 7 inclusive.
 * If the input string is a valid register name, the function returns the corresponding register number.
 * Otherwise, the function returns -1.
 */
int is_register_name(char *str)
{
   
    /* Check if the length of the string is at least 2 characters */
    if(strlen(str) < LENGTH_OF_REGISTER_NAME)
    {
        return -1;
    }

    /* Check if the first character is 'r' */
    if(*str++ != 'r')
    {
        return -1;
    }

    /* Check if the second character is a digit between 0 and 7 */
    if(*str < '0' || *str > '7')
    {
        return -1;
    }
    
    /* Check if the third character (if exists) is ':' and ignore it */
    if(strlen(str) == LENGTH_OF_REGISTER_NAME && *(str + 1) == ':' && *(str + 2) == '\0')
    {   
        return(*str - '0');
    }

    /* the string is a register name  */
    if(strlen(str) == 1 && *(str + 1) == '\0')
    {
        return(*str - '0');
    }

    return -1;
}

/*
 * This function takes in a character pointer 'str' as input and returns
 * a character pointer to the name of the directive (DATA, STRING, ENTRY or EXTERN)
 * if the input 'str' is a valid directive name, otherwise it returns NULL.
 */
char* get_directive_name(char *str)
{
    /* Check if 'str' is NULL, and return NULL if it is. */
    if(str == NULL)
    {
        return NULL;
    }

    /* Check if the first character of 'str' is '.'. If it's not, then
     * 'str' is not a valid directive name, so return NULL. */
    if(*str != '.')
    {
        return NULL;
    }

    /* Check if 'str' is equal to the DATA directive. If it is, then return
     * a pointer to the string "DATA". */
    if(strcmp(str, DATA) == 0)
        return DATA;

    /* Check if 'str' is equal to the STRING directive. If it is, then return
     * a pointer to the string "STRING". */
    if(strcmp(str, STRING) == 0)
        return STRING;

    /* Check if 'str' is equal to the ENTRY directive. If it is, then return
     * a pointer to the string "ENTRY". */
    if(strcmp(str, ENTRY) == 0)
        return ENTRY;

    /* Check if 'str' is equal to the EXTERN directive. If it is, then return
     * a pointer to the string "EXTERN". */
    if(strcmp(str, EXTERN) == 0)
        return EXTERN;

    /* If 'str' is not a valid directive name, then return NULL. */
    return NULL;
}



/*
 * This function takes in a character pointer 'str' as input, and modifies
 * the string by removing any trailing whitespace characters (spaces, tabs, and newlines)
 * from the end of the string. The modified string is
 * stored back in the same memory location pointed to by 'str'.
 */
void remove_trailing_whitespace(char *str)
{
    /* Get the length of the input string */
    unsigned int len = strlen(str);

    /* Start at the last character in the string */
    int i = len - 1;


    /* Checking over the string from the end to the beginning, removing trailing whitespace */
    while (i >= 0 && isspace(str[i]))
    {
        str[i] = '\0'; /* replace the current character with a null terminator */

        i--; /* move to the previous character */
    }

}

/*
 * This function takes in a character pointer 'str' as input, and modifies
 * the string by removing extra spaces and tabs, and replacing them with a
 * single space. The modified string is stored back in the same memory location
 * pointed to by 'str'.
 */
void remove_extra_spaces(char* str)
{
    /* Flag to keep track of whether a space has been seen */
    int seen_spaces = 0;

    /* Pointer to the start of the string that we want to modify */
    char* dest = str;


    /* Skip any leading spaces or tabs in the input string */
    while (*str == ' ' || *str == '\t')
    {
        str++;
    }


    /* Going over the rest of the string, replacing extra spaces and tabs with a single space */
    while (*str != '\0')
    {

        if (*str == ' ' || *str == '\t')
        {
            seen_spaces = 1; /* mark that a space has been seen */
        }

        else
        {
            if (seen_spaces)
            {
                *dest++ = ' '; /* if we've seen extra spaces, add a single space to the output string */

                seen_spaces = 0; /* reset the seen_spaces flag */
            }

            *dest++ = *str; /* copy the current character to the output string */
        }

        str++; /* move to the next character in the input string */
    }


    /* remove any trailing spaces or tabs from the output string */
    if (seen_spaces && dest > str)
    {
        dest--;
    }

    *dest = '\0'; /* add null character to the end of the output string */
}

/*
 * This function takes in two character pointers 'line' and 'string' as input,
 * and returns an integer value indicating whether there are any non-white
 * characters in the input 'line' after the input 'string'.
 * If there are non-white characters after the input 'string', the function
 * returns 1. Otherwise, it returns 0.
 */
int has_non_white_chars_after_string(char *line, char *string)
{
    /* Find the position of the input string in the string */
    char *string_pos = strstr(line, string);


    /* Get the length of the input string */
    unsigned int string_len = strlen(string);


    /* Get the position of the first non-white character after the input string */
    char *non_white_char_pos = string_pos + string_len;


    /* Check if there are any non-white characters after the input string */
    while(*non_white_char_pos != '\0')
    {
        if(!isspace(*non_white_char_pos))
        {
            return 1;
        }

        non_white_char_pos++;
    }


    return 0;
}

