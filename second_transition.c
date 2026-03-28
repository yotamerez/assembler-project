#include "second_transition.h"

/*
 * This function takes a file pointer to the ".am" file, 
 * a pointer to the head of a linked list containing the symbol list, the file name, 
 * and the counts of the instruction and data lines as input. 
 * It performs the second transition of the assembler, 
 * encoding the operation and directive lines and updating the machine code arrays. 
 * It then creates the output files if there were no errors. Finally, it frees the machine code arrays.
 */
void second_transition(FILE *am_file, node **head, char *file_name, int IC, int DC)
{

    /* Declare variables */
    char line[MAX_LINE_LENGTH], *temp_line, *token, *is_label;

    int error_flag = 0;

    int i = 0, j = 0;

    /* Array for the encoded binary operation lines  */
    short int *operation_machine_code_array = (short int*)safe_calloc(IC, sizeof(short int));

    /* Array for the encoded binary directive lines  */
    short int *directive_machine_code_array = (short int*)safe_calloc(DC, sizeof(short int));

    int line_number = 0;


    /* Move file pointer to the beginning of the file */
    fseek(am_file, 0, SEEK_SET);


    /*  Loop through each line in the file */
    while(fgets(line, MAX_LINE_LENGTH, am_file) != NULL)
    {
        /* Create a temporary copy of the line */
        temp_line = (char*)safe_malloc(strlen(line) + 1);

        strcpy(temp_line, line);

        /* Get the first token (label or operation/directive) if it is, get the next token (operation/directive) */
        token = strtok(temp_line, " " "\n");

        /* Check if the first token is a label */
        if((is_label = strchr(token, ':')) != NULL)
            token = strtok(NULL, " " "\n");

        /* Check if the token is an operation */
        if(get_op_name(token) != NULL)
        {
            /* Encode the operation line and update the machine code array */
            if(encode_operation_line(line, head, operation_machine_code_array, &i, j, line_number++) == 0)
            {
                error_flag = 1; /* Set the error flag if there is an error (usage of labels that were not declared) */
            }
        }

            /* The token is a directive */
        else
        {
            /* Encode the directive line and update the machine code array */
            encode_directive_line(line, directive_machine_code_array, &j);

            line_number++;
        }

        /* Free the temporary line copy */
        free(temp_line);
    }   

    /* If there were no errors, create the output files */
    if(!error_flag)
        create_output_files(head, operation_machine_code_array, directive_machine_code_array, file_name, IC, DC);

    /* Free the machine code arrays */
    free(operation_machine_code_array);

    free(directive_machine_code_array);
}


/*
 * This function encodes a single line of a directive in the second transition of the assembler. 
 * It checks if the directive is ".extern" or ".entry" and if so, returns without encoding. 
 * If the directive is ".data" or ".string", it extracts the relevant data and calls functions to encode them into the machine code array.
 */
void encode_directive_line(char *line, short int *directive_machine_code_array, int *j)
{
    char *token;

    /* Get the first token in the line */
    token = strtok(line, " " "\n");

    /* If the token is not a directive name (label), get the next token */
    if(get_directive_name(token) == NULL)
        token = strtok(NULL, " " "\n");

    /* If the directive is ".extern" or ".entry", return without encoding */
    if(strcmp(token, EXTERN) == 0 || strcmp(token, ENTRY) == 0)
        return;


        /* If the directive is ".data", encode the data directive */
    else if(strcmp(token, DATA) == 0)
    {
        /* Get the next token, which should contain the data parameters */
        token = strtok(NULL, "\n");

        /* Encode the data directive */
        encode_data_directive(token, directive_machine_code_array, j);
    }

        /* If the directive is ".string", encode the string directive */
    else if(strcmp(token, STRING) == 0)
    {
        /* Get the next token, which should contain the string */
        token = strtok(NULL, "\n");

        /* Encode the string directive */
        encode_string_directive(token, directive_machine_code_array, j);
    }

}

/*
 * This function encodes an operation line of an assembly code by parsing the line,
 * identifying the operation command and its operands, and encoding them into a machine code array. 
 * It calls other functions to handle different types of operations (with and without parameters, one and two operands). 
 * The encoded machine code array is stored in the operation_machine_code_array parameter. 
 * The function returns a value indicating whether the operation command is stop or rts (in which case no further action is needed).
 */
int encode_operation_line(char *line, node **head, short int *operation_machine_code_array, int *i, int j, int line_number)
{
    char *token;

    int op_number; /* Holds the opcode number of the operation command */

    int result;


    token = strtok(line, " " "\n"); /* Get the first token in the line */

    /*If the first token is not an operation command (It's a label), get the next token */
    if(get_op_name(token) == NULL)
        token = strtok(NULL, " " "\n");


    /* Get the opcode number of the operation command */
    op_number = get_op_code_by_name(token);


    /* Encode the opcode number in the operation machine code array */
    operation_machine_code_array[*i] |= SHIFT_LEFT(op_number, OPCODE_SHIFT);

    /* If the operation command is stop or rts, no further action is needed */
    if(strcmp(token, STOP) == 0 || strcmp(token, RTS) == 0)
        result = 1;

    /* If the operation command is jmp, bne or jsr */
    else if(strcmp(token, JMP) == 0 || strcmp(token, BNE) == 0 || strcmp(token, JSR) == 0)
    {
        token = strtok(NULL, " " "\n");  /* get the next token, which is the operand for the operation command */

        /* Call the function to encode an operation command that may contain parameters */
        result = encode_operation_with_parameters(token, head, operation_machine_code_array, i, j, line_number);
    }

    /* If the operation command is not, clr, inc, dec, red or prn */
    else if(strcmp(token, NOT) == 0 || strcmp(token, CLR) == 0 || strcmp(token, INC) == 0 || strcmp(token, DEC) == 0 || strcmp(token, RED) == 0 ||
            strcmp(token, PRN) == 0)
    {
        char *token2;

        token2 = strtok(NULL, " " "\n"); /* get the next token, which is the operand for the operation command */

        /* Call the function to encode an operation command with one operand */
        result = encode_operation_with_one_operand(token2, token, head, operation_machine_code_array, i, j, line_number);
    }

        /* If the operation command is mov, cmp, add, sub or lea */
    else
    {
        char *token2, *token3;

        token2 = strtok(NULL, " " "," "\n"); /* Get the next token, which is the first operand for the operation command */

        token3 = strtok(NULL, "," " " "\n"); /* Get the next token, which is the second operand for the operation command */

        result = encode_operation_with_two_operands(token2, token3, head, operation_machine_code_array, i, j, line_number);
    }

    (*i)++; /* Update the index in the operation_machine */

    return result;

}


