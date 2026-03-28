#include "first_transition.h"



/*
 * This function takes a string as input and checks if the first character is alphabetic.
 * If the first character is alphabetic, the function returns 1 to indicate no error.
 * Otherwise, the function returns 0 to indicate an error.
 */
int starts_with_alpha(const char *str)
{
    /* If the first character is not alphabetic, return 0 to indicate error */
    if(!isalpha(*str))
    {  
       return 0;   
    }
   
    /* If the first character is alphabetic, return 1 to indicate no error */
    return 1; 
}


/*
 * This function takes a string as input and checks if the first character is a dot.
 * If the first character is a dot, the function returns 1 to indicate an error.
 * Otherwise, the function returns 0 to indicate no error.
 */
int starts_with_dot(const char *str)
{

    /* If the first character is not a dot, return 0 to indicate no error */
    if(*str != '.')
    {
        return 0;
    }

    /* If the first character is a dot, return 1 to indicate an error */
    return 1;
}


/*
 * This function takes a string as input, excluding the first and last characters, and checks if it contains any non-alphanumeric characters.
 * If the string contains any non-alphanumeric characters, the function returns 1 to indicate an error.
 * Otherwise, the function returns 0 to indicate no error.
 * The line_counter parameter is not used in the function and can be ignored.
 */
int contains_non_alpha_chars(char *str)
{
    int i;

    unsigned int len = strlen(str);


    /* Loop through the string (excluding the first and last characters) */
    for(i = 1; i < len-1; i++)
    {
        /* If the current character is not alphanumeric, return 1 to indicate an error */
        if(!isalnum(str[i]))
            return 1;

    }

    /* If all characters are alphanumeric, return 0 to indicate no error */
    return 0;
}


/*
 * This function takes a string and a line counter as input.
 * It checks if the string ends with a colon. If it does, the function returns 1.
 * Otherwise, if the print flag is set, it prints an error message with the line number and returns 0.
 * If the string is empty, the function returns 0.
 */
int end_with_colon(char *str, int line_number, char print, char *file_name)
{
    unsigned int len = strlen(str);


    /* If the string is not empty and the last character is a colon, return 1 */
    if(len > 0 && str[len-1] == ':')
    {
        return 1;
    }

    /* If the print flag is set, print an error message with the line number */
    if(print)
        fprintf(stdout, "Error in file \"%s.am\" in line %d: label should end with ':' character\n", file_name, line_number);


    /* Return 0 to indicate that the string does not end with a colon */
    return 0;
}

/*
 * This function removes leading spaces and tabs from a given string by shifting the rest of the characters forward.
 * It takes a char pointer as an argument and returns void, meaning it modifies the original string directly.
 * void remove_leading_spaces(char *str)
 */
void remove_leading_spaces(char *str)
{
    int i = 0, j = 0;


    /* Find the first character that is not ' ' or '\t' */
    while(str[i] == ' ' || str[i] == '\t')
    {
        i++;
    }


    /* Copy the rest of the string from the start */
    while(str[i] != '\0')
    {
        str[j] = str[i];

        i++;

        j++;
    }

    str[j] = '\0';
}


/*
 * This function checks if the given string contains two or more consecutive commas. *
 * str: the string to check for consecutive commas. *
 * Returns:
 * 1 if the string contains consecutive commas.
 * 0 if the string does not contain consecutive commas.
 */
int check_consecutive_commas(char *str)
{
    unsigned int len = strlen(str);

    int i, count = 0;

    /* check if there is two or more consecutive commas */
    for(i = 0; i < len; i++)
    {
        if(str[i] == ',')
        {
            count++;

            /* skip over any blanks between consecutive commas */
            while(i+1 < len && isspace(str[i+1]))
            {
                i++;
            }

            if(count >= 2)
            {
                return 1; /* the string does have consecutive commas */
            }
        }

        else
        {
            count = 0;
        }
    }

    return 0; /* the string doesn't have consecutive commas */
}


/*
 * Check if a string ends with a comma.
 * str: the string to check
 * returns: 1 if the string ends with a comma, 0 otherwise
 */
int ends_with_comma(char *str)
{
    unsigned int len = strlen(str);


    /* check if the string ends with a comma */
    if(str[len - 1] == ',')
    {
        return 1; /* the string ends with a comma */
    }


    return 0; /* the string doesn't end with a comma */
}

/*
 * This function checks the syntax of a comma-separated line of text to determine whether it is legal or not.
 * It checks for consecutive commas and for a line that ends with a comma.
 * If either of these conditions is met, the function prints an error message and returns 0, indicating that the syntax is illegal.
 * Otherwise, the function returns 1, indicating that the syntax is legal.
 */
int is_comma_syntax_legal(char *str, int line_counter, char *file_name)
{

    /* Check for consecutive commas */
    if(check_consecutive_commas(str))
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: the line has consecutive commas\n", file_name, line_counter);

        return 0;
    }

    /* Check for line ending with a comma */
    if(ends_with_comma(str))
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: the line ends with a comma\n", file_name, line_counter);

        return 0;
    }

    /* Comma syntax is legal */
    return 1;

}

/*
 * This function checks if a given label name is valid according to the following rules:
 * The label's name must not be longer than 30 characters.
 * The label's name must only contain alphabetic characters or numbers.
 * The label's name must not be one of the register names.
 * The label's name must not be one of the operation names.
 * If any of these conditions is not met, the function returns 0, indicating that the label name is invalid.
 * If the label name is valid, the function returns 1.
 * If the 'print' parameter is set to a non-zero value, the function prints an error message to stdout for any invalid condition found.
 */
int is_label_name_valid(char *str, int line_number, char print, char *file_name)
{

    /* Check if label name is longer than 30 characters. */
    if(strlen(str) > MAX_LABEL_NAME)
    {
        if(print)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: label's name can't be longer then 30 characters - \"%s\"\n", file_name, line_number, str);
        }

        return 0;
    }


    /* Check if label name contains non-alphanumeric characters. */
    if(contains_non_alpha_chars(str))
    {
        if(print)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: label's name should contain only alphabetic characters or numbers - \"%s\"\n", file_name, line_number, str);
        }

        return 0;
    }


    /* Check if label name is one of the registers names. */
    if(is_register_name(str) != -1)
    {   
        if(print)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: label's name can't be one of the registers name - \"%s\"\n", file_name, line_number, str);
        }

        return 0;
    }


    /* Check if label name is one of the operation names. */
    if(get_op_name(str))
    {
        if(print)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: label's name can't be one of the operations name - \"%s\"\n", file_name, line_number, str);
        }

        return 0;
    }


    /* Label name is valid. */
    return 1;

}


/*
 * This function checks if a string is a legal number operand,
 * which means that it starts with the '#' character followed by an optional '+' or '-' sign and then by one or more digits.
 * If the string is not a legal number operand, the function returns 0.
 * If the string is a legal number operand, the function returns 1.
 */
int is_operand_legal_number(const char* str)
{
    int i = 1;


    /* Check if the string starts with '#' */
    if (str[0] != '#')
    {
        return 0;
    }


    /* Skip over an optional '+' or '-' sign */
    if (str[i] == '+' || str[i] == '-')
    {
        i++;
    }


    /* Check that the rest of the string contains only digits */
    while (str[i] != '\0')
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return 0;
        }
        i++;
    }


    return 1;
}


/*
 * This function receives a string containing comma-separated tokens and checks whether each token is a legal number (positive/negative).
 * It splits the string using "," and " " as delimiters and checks each token for validity.
 * If a token is empty or contains non-digit characters (except for an optional '+' or '-' sign at the beginning),
 * the function considers it illegal and returns an error code.
 * If all tokens are legal, the function returns the number of legal parameters found.
 */
int is_parameters_legal_numbers(char *str, int line_number, char *file_name)
{
    char *token;

    int count = 0, i;

    /* Split the string using "," and " " as delimiters */
    token = strtok(str, ",\n");

    if(token == NULL)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: no parameters for directive - \"%s\"\n", file_name, line_number, DATA);

        return 0; 
    }

    while(token != NULL)
    {
        unsigned int len;

        remove_leading_spaces(token);

        remove_trailing_whitespace(token);

        len = strlen(token);

        /* Check if token is a valid number (can be positive/negative) */
        if(*token != '+' && *token != '-' && !isdigit(*token))
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal parameter - \"%s\" for directive - \"%s\"\n", file_name, line_number, token, DATA);

            return 0;
        }

        /* Check if the rest of the characters in token are digits */
        else 
        {
            long int num = strtol(token, NULL, DECI);

            for (i = 1; i < len; i++) 
            {
                if (!isdigit(token[i])) 
                {
                    fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal parameter - \"%s\" for directive - \"%s\"\n", file_name, line_number, token, DATA);
 
                    return 0;
                }
            }

            /* Check if token can be represented in 14 bits */          
            if (num > MAX_14BIT_INT || num < MIN_14BIT_INT)
            {
                fprintf(stdout, "Error in file \"%s.am\" in line %d: the parameter is out of range - \"%s\" for directive - \"%s\"\n", file_name, line_number, token, DATA);

                return 0;
            }
        }

        /* Increment count if token is legal */
        count++;

        /* Move on to the next token */
        token = strtok(NULL, ",");
    }

    /* Return the number of legal parameters found */
    return count;
}





/*
 * This function checks whether a given string is a legal string operand in the assembly language.
 * A legal string operand must be enclosed in double quotes and contain only printable ASCII characters.
 * function returns the length of the string operand (including the null terminator) if it is legal,
 * -1 if it contains a non-printable ASCII character, and 0 if it is not enclosed in double quotes.
 */
int is_string_legal(char *str, int line_number, char *file_name)
{
    int count = 0;

    /* Check if the string is enclosed in double quotes and adjust the start and end pointers accordingly */
    char *end = str + strlen(str) - 1;


    if(*str != '"')
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: string should start with '\"' in \"%s\" line\n", file_name, line_number, STRING);

        return 0;
    }

    if(*end != '"')
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: string should end with '\"' in \"%s\" line\n", file_name, line_number, STRING);

        return 0;
    }

    str++;

    end--;

    /* Iterate over the characters between the double quotes and check if they are printable ASCII characters */
    while(str <= end)
    {
        if(*str < 0 || *str > MAX_CHAR_VALUE)
        {   
            fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal string \"%s\" for \"%s\" line\n", file_name, line_number, str, STRING);

            return 0; 
        }

        str++;

        count++;
    }

    /* If the string is empty */
    if(count == 0)
    {  
        fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal string \"%s\" for \"%s\" line\n", file_name, line_number, str, STRING);

        return 0;
    }

    /* Return the length of the string operand, including the null terminator */
    return count+1;
}


/*
 * The function removes a trailing colon from a given string and returns a new string without the colon.
 * If the input string does not end with a colon, it returns NULL.
 * The function dynamically allocates memory for the new string without the colon using the safe_malloc function.
 * char* remove_colon(char *str)
 */
char* remove_colon(char *str)
{
    size_t len = strlen(str);

    /* Check if the last character of the string is a colon */
    if(len > 0 && str[len-1] == ':')
    {
        /* Allocate memory for the new string without the colon */
        char *new_str = safe_malloc(len);

        /* Copy the original string to the new string, except for the last character (the colon) */
        memcpy(new_str, str, len-1);

        /* Add a null terminator to the end of the new string */
        new_str[len-1] = '\0';

        /* Return the new string without the colon */
        return new_str;
    }

    /* Return NULL if the string does not end with a colon */
    return NULL;
}


/*
 * This function takes a string str, a line_number, and a print flag as input.
 * It uses the get_op_name function to extract the name of the operation from the string.
 * If the operation name is not valid, the function prints an error message to stdout and returns NULL.
 * Otherwise, it returns the operation name.
 */
char* check_op(char *str, int line_number, char print, char *file_name)
{
    /* Extract the name of the operation from the string using the get_op_name function. */
    char *op_name = get_op_name(str);

    /* If the operation name is NULL (the string is not a valid operation name) */
    if(op_name == NULL)
    {
        if(print)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: no such operation: \"%s\"\n", file_name, line_number, str);
        }

        return NULL;
    }

    /* If the operation name is valid return it */
    return op_name;
}


/*
 * This function checks if the given operation name op_name has no operands.
 * If the operation has no operands, the function returns a value that indicates that the line encoding for this operation contains no operand.
 * If the operation is invalid (i.e., not "rts" or "stop"),
 * the function prints an error message to stdout indicating that the operation is missing operand(s) and returns -1.
 */
int operations_with_no_operands(char *op_name, int line_number, char *file_name)
{
    /* If the operation has no operands, return 0 */
    if(strcmp(op_name, RTS) == 0 || strcmp(op_name, STOP) == 0)
        return ENCODING_LINES_NO_OPERAND;

    /* If the operation is invalid, print an error message and return -1. */
    fprintf(stdout, "Error in file \"%s.am\" in line %d: missing operand(s) for operation -  \"%s\"\n", file_name, line_number, op_name);

    return -1;
}


/*
 * This function checks if the given operation name op_name has one operand and determines the number of encoding lines needed for the operation.
 * If the operation requires a register name, label name, or a number than the function returns the number of encoding lines needed.
 * If the operation is invalid, the function prints an error message to stdout indicating the error and returns -1.
 */
int operations_with_one_operand(char *operand, char *op_name, int line_number, char *file_name)
{   

    int op_number = get_op_code_by_name(op_name);

    /* For operations that take only one operand and require a register name, label name, or a number input. */
    if(op_number == NOT_OP_CODE || op_number == CLR_OP_CODE || op_number == INC_OP_CODE || op_number == DEC_OP_CODE || op_number == RED_OP_CODE)
    {
        /* If the operand is a register, return the number of encoding lines needed. */
        if(is_register_name(operand) != -1)
            return ENCODING_LINES_ONE_OPERAND;

        /* If the operand is a valid label name, return the number of encoding lines needed. */
        if(starts_with_alpha(operand) && is_label_name_valid(operand, line_number, 0, file_name))
            return ENCODING_LINES_ONE_OPERAND;

        /* If the operand is invalid, print an error message and return -1. */
        fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal operand -  \"%s\" for operation \"%s\"\n", file_name, line_number, operand, op_name);

        return -1;
    }

        /* For operations with parameters. */
    else if(op_number == JMP_OP_CODE || op_number == BNE_OP_CODE || op_number == JSR_OP_CODE)
    {
        /* Check the validity of the parameters in the operand and return the number of encoding lines needed. */
        return operations_with_parameters(operand, line_number, file_name);
    }

        /* For operations that take only one operand and require a number or a valid label name as input. */
    else if(op_number == PRN_OP_CODE)
    {
        /*  If the operand is a number or a valid label name, return the number of encoding lines needed. */
        if(!is_operand_legal_number(operand) && (!starts_with_alpha(operand) || !is_label_name_valid(operand, line_number, 0, file_name)) 
           && is_register_name(operand) == -1)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal operand: \"%s\" for operation \"%s\"\n", file_name, line_number, operand, op_name);

            return -1;
        }

        return ENCODING_LINES_ONE_OPERAND;
    }

    /* If the operation is not recognized, print an error message and return -1. */
    fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal operation - \"%s\" for line with one operand\n", file_name, line_number, op_name);

    return -1;
}


/*
 * This function checks the validity of an operand passed as a string parameter,
 * which may contain up to two parameters separated by a comma and enclosed in parentheses.
 * It returns an integer value indicating the type of encoding line required for the operand.
 * If the operand is invalid, the function prints an error message to stdout and returns -1.
 */
int operations_with_parameters(char *operand, int line_number, char *file_name)
{
    char *token1, *token2;

    int result = check_parentheses(operand);


    if(!starts_with_alpha(operand))
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal operand - \"%s\" for jump operation\n", file_name, line_number, operand);

        return -1;
    }

    if(result== -1)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal parentheses in operand - \"%s\"\n", file_name, line_number, operand);

        return -1;
    }

    /* Check if the operand has no parameters, if yes then check if the operand is a legal label */
    if(result == 0)
    {
        if(!starts_with_alpha(operand) || end_with_colon(operand, line_number, 0, file_name) || !is_label_name_valid(operand, line_number, 0, file_name))
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal label name - \"%s\" for jump operation\n", file_name, line_number, operand);

            return -1;
        }

        return 1;
    }

    /* Tokenize the operand string and check for illegal label names and parameters. */
    token1 = strtok(operand, "(" " " "\n");

    if(!starts_with_alpha(token1) || end_with_colon(token1, line_number, 0, file_name) || !is_label_name_valid(token1, line_number,0, file_name))
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal label name - \"%s\"\n", file_name, line_number, token1);

        return -1;
    }

    token1 = strtok(NULL, "," " " "\n");

    if(token1 == NULL)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal syntax of operand - \"%s\"\n", file_name, line_number, operand);

        return -1;
    }

    if(!is_operand_legal_number(token1) && (!starts_with_alpha(token1) || !is_label_name_valid(token1, line_number, 0, file_name)) && is_register_name(token1) == -1)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal first parameter - \"%s\" in operand \"%s\"\n", file_name, line_number, token1, operand);

        return -1;
    }

    token2 = strtok(NULL, ")" " " "\n");

    if(token2 == NULL)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal syntax of operand - \"%s\"\n", file_name, line_number, operand);

        return -1;
    }

    if(!is_operand_legal_number(token2) && (!starts_with_alpha(token2) || !is_label_name_valid(token2, line_number, 0, file_name)) && is_register_name(token2) == -1)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal second parameter - \"%s\" in operand \"%s\"\n", file_name, line_number, token2, operand);

        return -1;
    }

    /* Return 2 if both parameters are valid register names, 3 otherwise. */
    if(is_register_name(token1) != -1 && is_register_name(token2) != -1)
        return ENCODING_LINES_WITH_PARAMETERS - 1;

    return ENCODING_LINES_WITH_PARAMETERS;
}


/*
 * This function checks if the parentheses in the given string are balanced and if they are in the correct position.
 * The function returns 0 if the string has no parentheses, 1 if the string has a single pair of balanced parentheses,
 * and -1 if the parentheses are unbalanced or if the string has multiple pairs of parentheses or a single unbalanced parenthesis.
 */
int check_parentheses(char *str)
{
    int left_paren_count = 0, right_paren_count = 0, i;

    unsigned int len = strlen(str);

    /* Count the number of left and right parentheses */
    for(i = 0; str[i] != '\0'; i++)
    {
        if(str[i] == '(')
        {
            left_paren_count++;
        }

        else if(str[i] == ')')
        {
            right_paren_count++;
        }
    }

    /* Parentheses are not balanced */
    if(left_paren_count != right_paren_count)
    {
        return -1;
    }

    /* No parentheses in string */
    else if(left_paren_count == 0 && right_paren_count == 0)
    {
        return 0;
    }

        /* Single pair of parentheses */
    else if(left_paren_count == 1 && right_paren_count == 1)
    {
        /* Parentheses are not in correct position */
        if(*str == '(' || str[len - 1] != ')')
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

        /* Multiple pairs of parentheses */
    else
    {
        return -1;
    }
}

/*
 * The function operations_with_two_operands checks the legality of two operands and an operation name.
 * It first finds the operation number by matching the operation name to an array of structs,
 * then removes commas from the operands. It checks the legality of the first operand depending on the operation,
 * and the legality of the second operand also depending on the operation.
 * It returns the number of lines required to encode the operation, taking into account whether the two operands are registers or not.
 */
int operations_with_two_operands(char *first_operand, char *second_operand, char *op_name, int line_number, char *file_name)
{
    int op_number = get_op_code_by_name(op_name);

    /* Remove commas from the operands */
    remove_comma(first_operand);

    remove_comma(second_operand);


    /* Check first operand legality */
    if(op_number == MOV_OP_CODE || op_number == CMP_OP_CODE || op_number == ADD_OP_CODE || op_number == SUB_OP_CODE)
    {
        /* First operand should be a legal number, a label, or a register */
        if(!is_operand_legal_number(first_operand) && (!starts_with_alpha(first_operand) || !is_label_name_valid(first_operand, line_number, 0, file_name)) 
           && is_register_name(first_operand) == -1)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal first operand: \"%s\"\n", file_name, line_number, first_operand);

            return -1;
        }
    }

    else if(op_number == LEA_OP_CODE)
    {
        /* For lea operation, first operand should be a label */
        if(!starts_with_alpha(first_operand) || !is_label_name_valid(first_operand, line_number, 0, file_name))
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal first operand: \"%s\"\n", file_name, line_number, first_operand);

            return -1;
        }
    }

        /* Illegal operation name */
    else
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal operation name: \"%s\" for line with two operand\n", file_name, line_number, op_name);

        return -1;
    }

    /* Check second operand legality */
    if(op_number == MOV_OP_CODE || op_number == ADD_OP_CODE || op_number == SUB_OP_CODE || op_number == LEA_OP_CODE)
    {
        /* Second operand should be a label, or a register */
        if((!starts_with_alpha(second_operand) || !is_label_name_valid(second_operand, line_number, 0, file_name)) && is_register_name(second_operand) == -1)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal second operand: \"%s\"\n", file_name, line_number, second_operand);

            return -1;
        }
    }

    else
    {
        /* For cmp operation, second operand should be a legal number, a label, or a register */
        if(!is_operand_legal_number(second_operand) && (!starts_with_alpha(second_operand) || !is_label_name_valid(second_operand, line_number, 0, file_name))
           && is_register_name(second_operand) == -1)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal second operand: \"%s\"\n", file_name, line_number, second_operand);

            return -1;
        }
    }

    /* Return the number of lines required to encode the current operation */
    if(is_register_name(first_operand) != -1 && is_register_name(second_operand) != -1)
        return ENCODING_LINES_TWO_OPERANDS - 1; /* Registers share a line */

    return ENCODING_LINES_TWO_OPERANDS; /* One line for each operand */
}



/*
 * This function takes a string as input and removes the first comma found in it, if any.
 * If a comma is found, it uses the memmove() function to shift all characters in the
 * string to the left by one position.
 */
void remove_comma(char *str)
{
    char *comma_pos = strchr(str, ',');

    if(comma_pos != NULL)
    {
        memmove(comma_pos, comma_pos + 1, strlen(comma_pos));
    }
}


/*
 * This function receives the directive name and operand as input parameters,
 * along with other parameters related to the current line being processed,
 * and a pointer to a linked list of labels. It then checks the directive type and validates the operand accordingly.
 * For the "data" directive, it verifies that the operand is a comma-separated list of legal numbers.
 * For the "string" directive, it verifies that the operand is a legal string.
 * For the "entry" and "extern" directives,
 * it checks that the operand is a valid label name and adds it to the linked list of labels with the appropriate properties.
 * If a label was already defined with this name or the label name is invalid, it returns an error.
 */
int directive_operand(char *directive_name, char *operand, int line_number, char after_label, node **head, char *file_name)
{
    /* Initialize the result variable to 1 */
    int num_code_lines = 1;


    /* Check if the directive is "data" */
    if(strcmp(directive_name, DATA) == 0)
    {
        /* If the operand is not a comma-separated list of legal numbers, return an error */
        num_code_lines = is_parameters_legal_numbers(operand, line_number, file_name);

        if(num_code_lines  == 0)        
            return -1;        

    }

        /* Check if the directive is "string" */
    else if(strcmp(directive_name, STRING) == 0)
    {
        /* If the operand is not a legal string, return an error */
        if((num_code_lines = is_string_legal(operand, line_number, file_name)) == 0)
        {          
            return -1;
        }

    }

        /* Check if the directive is "entry" or "extern" */
    else if(strcmp(directive_name, ENTRY) == 0 || strcmp(directive_name, EXTERN) == 0)
    {
        char* label_name;

        /* If a label was already defined on this line, return an error */
        if(after_label)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: label in the start of  \"%s\" line\n", file_name, line_number,directive_name);

            return -1;
        }

        /* If the operand is not a valid label name, return an error */
        if(starts_with_alpha(operand) == 0 || is_label_name_valid(operand, line_number, 0, file_name) == 0)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal label name - \"%s\" for directive \"%s\"\n", file_name, line_number, operand, directive_name);

            return -1;
        }

        /* Allocate memory for the label name */
        label_name = safe_malloc(strlen(operand) + 1);

        /* Copy the label name to the allocated memory */
        strcpy(label_name, operand);

        if(strcmp(directive_name, EXTERN) == 0)
        {
            int result;

            if((result = add_label_to_list(label_name, 1, head, 1, 0, line_number, file_name)) == -1)
            {
                /* Free the allocated memory and return an error */
                free(label_name);

                return -1;
            }

            if(result == 0)
               free(label_name);
        }

        /* If the directive is "entry" */
        else
        {
            int result;

            /* If a label with the same name already exists in the list and the field entry was 1 */
            if((result = add_label_to_list(label_name, 1, head, 0, 1, line_number, file_name)) == -1)
            {
                free(label_name);

                return -1;
            }

            /* If a label with the same name already exists in the list, the function does not add it again and update the entry field */
            if(result == 0)
                free(label_name);
        }

        return 0;
    }


    /* Return the result */
    return num_code_lines;
}

/*
 * The add_label_to_list function adds a new label to a linked list of labels,
 * and returns an integer indicating the success or failure of the operation.
 * It first checks if the label already exists in the list, and if so,
 * either returns an error or updates the label's line number. If the label is new,
 * it creates a new node for it, sets its properties, and adds it to the end of the list.
 * The function also handles the special cases of entry and extern labels.
*/
int add_label_to_list(char *label_name, int label_line, node **head, char is_extern, char is_entry, int line_number, char *file_name)
{
    node *current = *head;

    node *new_label_node;

    /* Traverse the linked list to check if the label already exists */
    while(current != NULL)
    {
        /* If the label is an entry, return its line number */
        if(strcmp(get_label_name(current), label_name) == 0)
        {
            /* If the label is an extern, return its line number */
            if(is_extern == 1)
                return extern_line(current, line_number, file_name);

            if(is_entry == 1)
                return entry_line(current, line_number, file_name);

                /* If the label is not an entry */
            else
            {
                /* If the label was previously defined on the first pass, update its line number */
                if(get_label_line(current) == 1)
                {
                    set_label_line(current, label_line);

                    return 0;
                }

                    /* If the label was previously defined on the second pass, return an error */
                else
                {
                    fprintf(stdout, "Error in file \"%s.am\" in line %d: label - \"%s\" is already defined\n", file_name, line_number, label_name);

                    return -1;
                }
            }
        }

        current = get_next_label(current);
    }


    /* Allocate memory for a new label node */
    new_label_node = create_label(label_name, label_line, is_extern, is_entry);


    /* If the linked list is empty, set the head to the new node */
    if(*head == NULL)
    {
        *head = new_label_node;

    }

    /* Traverse the linked list and insert the new node at the end */
    else
    {
        current = *head;

        while(get_next_label(current) != NULL)
        {
            current = get_next_label(current);
        }

        set_next_label(current, new_label_node);
    }

    return 1;
}


/*
 * This function takes a node pointer as an argument
 * and updates the is_entry flag of the node to indicate that the label is an entry point.
 * It also checks for errors, such as if the label has already been marked as an external.
 */
int entry_line(node *current, int line_number, char *file_name)
{

    if(get_is_entry(current) == 1)
       return 0;    

    if(get_is_extern(current) == 1)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: entry and extern lines for the same label - \"%s\"\n", file_name, line_number, get_label_name(current));

        return -1;
    }

    set_is_entry(current, 1);

    return 0;
}


/*
 * This function takes a node pointer as an argument and checks for errors
 * in the case that the label has already been defined in this file it returns -1.
 */
int extern_line(node *current, int line_number, char *file_name)
{
    if(get_label_line(current) != 1)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: label - \"%s\" is already defined in this file, cannot be extern line\n", file_name, line_number, get_label_name(current));

        return -1;
    }

    return 0;
}


