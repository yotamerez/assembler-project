#include "first_transition.h"


/*
 * This function performs the first transition of the file .am assembly code
 * During the first transition, the function reads each line of the file, analyzes it, and updates the relevant information
 * in the symbol table and the data table. The function also calculates the number of encoded lines that are required
 * for the assembly code and updates the instruction counter (IC) and the data counter (DC) accordingly. If an error occurs during the processing of a line,
 * the function sets an error flag to indicate that an error has occurred. The function returns the error flag to the calling function.
 */
int first_transition(FILE *am_file, char *file_name, node **head, int *IC, int *DC)
{

    char error_flag = 0; /* flag to indicate if an error occurred during the first transition */

    int label_line = START; /* the line number of the correct encoded line of the label */

    char line[MAX_LINE_LENGTH]; /* buffer to hold each line of the input file */

    int line_number = 0; /* the line number being processed */

    int extra_lines; /* the number of extra lines consumed by a directive */


    /*  move the file position indicator to the beginning of the file */
    fseek(am_file, 0, SEEK_SET);



    while(fgets(line, MAX_LINE_LENGTH, am_file) != NULL)
    {
        /* remove leading spaces from the current line */
        remove_leading_spaces(line);

        remove_trailing_whitespace(line);

        /* skip empty or comment lines, and lines with illegal comma syntax */
        if(!is_comma_syntax_legal(line, line_number, file_name))
        {
            error_flag = 1;

            line_number++;

            continue;
        }

        /*  update label_line */
        if((extra_lines = line_analysis(line, line_number, label_line, head, DC, file_name)) != -1)
        {
            label_line +=  extra_lines;
        }
            /* set error flag if an error occurred while processing the line */
        else
        {
            error_flag = 1;
        }

        /* increment line_number to move to the next line */
        line_number++;
    }

    /* Updating the IC with the number of encoded lines that are required */
    (*IC) = label_line - START - (*DC);

    /* Check if the number of encoded instruction lines plus the number of data lines exceeds the maximum */
    if((*IC) + (*DC) > MAX_ENCODED_LINES)
    {
        fprintf(stdout, "Error in file \"%s.am\": the required lines to be encoded is over 155\n", file_name);

        error_flag = 1;
    }
    /* return the error flag indicating if an error occurred during the first transition */
    return error_flag;
}


/* 
 * This function analyzes a line of code in the assembly language, 
 * identifying whether it is a label definition, an operation, a directive line. 
 * It uses other functions to check the syntax of each type of line and updates the linked list of labels. 
 * The function also updates the DC. 
 *  if the line is valid return the number required to encode the line, -1 otherwise.
 */
int line_analysis(char *line, int line_number, int label_line, node **head, int *DC, char *file_name)
{
    char *temp_line; /* Temporary copy of the line */

    char *token; /* Current token being processed */


    /* Allocate memory for the temporary copy of the line */
    temp_line = (char*)safe_malloc(strlen(line) + 1);

    /* Copy the line to the temporary buffer */
    strcpy(temp_line, line);

    /* Check if the line starts with an alphabetic character */
    if(starts_with_alpha(temp_line))
    {
        /* Get the first token (which should be the operation or label) */
        token = strtok(temp_line, " " "\n");

        /* Check if the token ends with a colon (indicating a label) */
        if(end_with_colon(token, line_number, 0, file_name))
        {
            char *label_name; /* Name of the label */

            int encoded_lines, result; /* The lines required for this line */

            /* Check if the label name is valid */
            if(!is_label_name_valid(token, line_number, 1, file_name))
            {
                free(temp_line);

                return -1;
            }

            /* Check if the rest of the line is a valid line and store the amount of encoded lines required*/
            if((encoded_lines = line_start_with_label(line, line_number, head, DC, file_name)) == -1)
            {
                free(temp_line);

                return -1;
            }

            /* Remove the colon from the label name */
            label_name = remove_colon(token);

            if(label_name == NULL)
                return -1;

            result = add_label_to_list(label_name, label_line, head, 0, 0, line_number, file_name);

            /* Add the label to the label list */
            if(result == 0)
                free(label_name);

            else if(result == -1)
            {
                free(temp_line);

                free(label_name);

                return -1;
            }

            /* Free the temporary buffer and return the number of encoded lines required */
            free(temp_line);

            return encoded_lines;
        }

            /* If the token doesn't end with a colon, it must be an operation */
        else
        {
            char *op_name; /* Name of the operation */

            int result;

            /* Check if the operation name is valid */
            if((op_name = check_op(token, line_number, 0, file_name)) == NULL)
            {
                fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal start of a line - \"%s\"\n", file_name, line_number, token);

                free(temp_line);

                return -1;
            }

            /* Free the temporary buffer and process the operation line */
            free(temp_line);

            result = op_line(line, op_name, line_number, file_name);

            if(result == 0)
                return -1;

            return result;
        }

    }

        /* If the line starts with a dot, it must be a directive */
    else if(starts_with_dot(line))
    {
        char *directive_name; /* Name of the directive */

        int result;

        /* Get the directive name */
        token = strtok(temp_line, " " "\n");

        /* Check if the directive name is valid */
        if((directive_name = get_directive_name(token)) == NULL)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: no such directive name \"%s\"\n", file_name, line_number, token);

            free(temp_line);

            return -1;
        }

        /* Free the temporary buffer and process the directive line */
        free(temp_line);

        result = directive_line(line, directive_name, line_number, head, file_name);

        (*DC) += result;

        return result;
    }

    token = strtok(line, " ");

    /*  If the line doesn't start with an alphabetic character or a dot, it's invalid */
    fprintf(stdout, "Error in file \"%s.am\" in line %d: illegal start of a line - \"%s\"\n", file_name, line_number, token);

    free(temp_line);

    return -1;

}


/*
 * This function processes a line that starts with a label. 
 * It extracts the operation or directive from the line and then calls the appropriate function to check the syntax of the line.
 * The function returns the number of encoded lines required for this line and updates the data counter (DC).
 */
int line_start_with_label(char *line, int line_number, node **head, int *DC, char *file_name)
{
    char *token1, *token2, *token3;

    char *op_name, *directive_name;

    int result;

    /* Skipping the label name */
    token1 = strtok(line, " " "\n");

    /* Get the second token */
    token1 = strtok(NULL, " " "\n");

    if(token1 == NULL)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: missing command or directive after label\n", file_name, line_number);

        return -1;
    }

    /* Check if the second token is a directive */
    if((directive_name = get_directive_name(token1)) != NULL)
    {
        token2 = strtok(NULL, "\n");

        if(token2 == NULL)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: missing operands for directive - \"%s\"\n", file_name, line_number, directive_name);

            return -1;
        }

        /* Return the number of encoded lines required for this line and update DC*/
        result = directive_operand(directive_name, token2, line_number, 1, head, file_name);

        if(result == -1)
            return -1;

        (*DC) += result;

        return result;
    }

    /* Check if the second token is an operation */
    if((op_name = check_op(token1, line_number, 0, file_name)) == NULL)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: no operation or directive found - \"%s\"\n", file_name, line_number, token1);

        return -1;
    }

    /* Check if the operation is jmp, bne or jsr */
    if(strcmp(token1, JMP) == 0|| strcmp(token1, BNE) == 0|| strcmp(token1, JSR) == 0)
    {
        char *ptr;

        /* Check for blanks between parameters */
        token2 = strtok(NULL, " \n");


        /* Check the operation's operand */
        if(token2 == NULL)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: missing operand for operation - \"%s\"\n", file_name, line_number, op_name);

            return -1;
        }

        remove_trailing_whitespace(token2);

        ptr = strchr(token2, ' ');

        if(ptr != NULL)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: extra spaces between parameters for operation - \"%s\"\n", file_name, line_number, op_name);

            return -1;
        }

        result = operations_with_parameters(token2, line_number, file_name);

        if(result == -1)
            return -1;

        return result + 1;
    }

    /* Get the second of operand */
    token1 = strtok(NULL, " \n,");

    /* If there is no operand */
    if(token1 == NULL)
        result = operations_with_no_operands(op_name, line_number, file_name);

    else
    {
        /* Get the second of operand */
        token2 = strtok(NULL, " \n,");

        /* If there is one operand  */
        if(token2 == NULL)
            result = operations_with_one_operand(token1, op_name, line_number, file_name);
      
        else
        {
            token3 = strtok(NULL, "\n"); 
            {
                if(token3 != NULL)
                {
                    fprintf(stdout, "Error in file \"%s.am\" in line %d: extra operand for operation \"%s\"\n", file_name, line_number, op_name);

                    return -1;
                }  
            }

            /* If there is two operands */            
            result = operations_with_two_operands(token1, token2, op_name, line_number, file_name);
        }
    }

    if(result == -1)
        return -1;

    return result + 1;
}


/*
 * This function receives a line that begins with operation.
 * It first checks for specific operations (jmp, bne, jsr) that have a different syntax than other operations.
 * If the operation is not one of these, it expects either one or two operands separated by a comma. 
 * The function then calls different functions depending on the number of operands and the type of operation. 
 * It returns an integer indicating the number of lines of code consumed by the operation, or -1 if there was an error.
 * The errors include missing or extra operands, illegal syntax, and extra spaces between parameters.
 */
int op_line(char *line, char *op_name, int line_number, char *file_name)
{
    char *token1, *token2, *token3;

    /* Extract the first token from the input line */
    token1 = strtok(line, " \n");

    /* Check if the operation is jmp, bne or jsr */
    if(strcmp(token1, JMP) == 0|| strcmp(token1, BNE) == 0|| strcmp(token1, JSR) == 0)
    {
        char *ptr;

        /* Check for blanks between parameters */
        token2 = strtok(NULL, "\n");

        /* Check the operation's operand */
        if(token2 == NULL)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: missing operand for operation - \"%s\"\n", file_name, line_number, op_name);

            return -1;
        }

        remove_trailing_whitespace(token2);

        ptr = strchr(token2, ' ');

        if(ptr != NULL)
        {
            fprintf(stdout, "Error in file \"%s.am\" in line %d: extra spaces between parameters for operation - \"%s\"\n", file_name, line_number, op_name);

            return -1;
        }

        return operations_with_parameters(token2, line_number, file_name) + 1;
    }

    /* Get the first of operand */
    token1 = strtok(NULL, " \n,");

    /* If there is no operand */
    if(token1 == NULL)
        return operations_with_no_operands(op_name, line_number, file_name) + 1;

    /* Get the second of operand */
    token2 = strtok(NULL, "," " " "\n");

    /* If there is one operand  */
    if(token2 == NULL)
        return operations_with_one_operand(token1, op_name, line_number, file_name) + 1;

    token3 = strtok(NULL, " \n,");
   
    if(token3 != NULL) 
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: extra operand for operation \"%s\"\n", file_name, line_number, op_name);

        return -1;
    }

    /* If there is two operands  */
    return operations_with_two_operands(token1, token2, op_name, line_number, file_name) + 1;
}

/*
 * This function processes a line that begins with a directive name. 
 * It extracts the operand from the line and passes it to the "directive_operand" function for further processing. 
 * The "directive_operand" function returns the number of encoded lines required for the this line. 
 * If there is an error in the input line, the function prints an error message and returns -1.
*/
int directive_line(char *line, char *directive_name, int line_number, node **head, char *file_name)
{
    char *token;

    /* Skipping the directive name */
    token = strtok(line, " \n");

    /* Extract the operand from the line */
    token = strtok(NULL, "\n");

    if(token == NULL)
    {
        fprintf(stdout, "Error in file \"%s.am\" in line %d: missing operand for directive \"%s\"\n", file_name, line_number, directive_name);

        return -1;
    }

    /* Call the directive_operand function to process the operand and return the number of encoded lines required for this line*/
    return directive_operand(directive_name, token, line_number, 0, head, file_name);
}
