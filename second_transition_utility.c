#include "second_transition.h"


/*
 * This function encodes an operand string into a directive machine code array. 
 * It splits the operand into individual parameters using whitespace and commas as delimiters, 
 * converts each parameter to its numeric value, 
 * and ORs it with the current value in the directive machine code array. 
 * The index of the directive machine code array is then incremented to point to the next line, 
 * and the process is repeated for the next parameter in the operand.
 */
void encode_data_directive(char *operand, short int *directive_machine_code_array, int *j)
{
    /* Declare a pointer to store the current parameter being processed */
    char *parameter;

    /* Split the operand into individual parameters using whitespace and commas as delimiters */
    parameter = strtok(operand, "\t" " " "\n" ",");

    /* Process each parameter in the operand */
    while(parameter != NULL)
    {
        /* Convert the parameter to its numeric value and "|" it with the current value in the directive machine code array */
        directive_machine_code_array[*j] |= (get_number_value(parameter));

        /* Increment the index of the directive machine code array to point to the next line */
        (*j)++;

        /* Get the next parameter from the operand */
        parameter = strtok(NULL, " " "\n" ",");
    }
}


/*
 * This function encodes a string operand into a directive machine code array. 
 * It skips the first quote character of the string operand 
 * and then processes each character in the string until the closing quote is encountered. 
 * Each character is converted to its ASCII value and ORed with the current value in the directive machine code array. 
 * The index of the directive machine code array is then incremented to point to the next available memory address. 
 * Finally, the index of the directive machine code array is incremented one last time to reserve space for the terminating NULL character.
 */
void encode_string_directive(char *operand, short int *directive_machine_code_array, int *j)
{
    /* Increment the operand pointer to skip the first quote character */
    operand++;

    /* Process each character in the string until the closing quote is encountered */
    while(*operand != '"')
    {
        /* Convert the current character to its ASCII value and "|" it with the current value in the directive machine code array */
        directive_machine_code_array[*j] |= ((char)*operand);

        /* Increment the index of the directive machine code array to point to the next available memory address */
        (*j)++;

        /* Move the operand pointer to the next character in the string */
        operand++;
    }

    /*  Increment the index of the directive machine code array one last time to reserve space for the terminating NULL character */
    (*j)++;
}


/*
 * This function encodes a two-operand operation into a machine code array. 
 * It takes the two operands, a linked list of labels, the current line number, 
 * and the current position in the machine code array as input. 
 * The function first checks if both operands are registers and encodes them as register operands if they are. 
 * If not, it encodes each operand as either an immediate, register, or label operand depending on its type. 
 * The function returns 1 if the encoding is successful and 0 if it encounters an error.
 */
int encode_operation_with_two_operands(char *first_operand, char *second_operand, node **head, short int *operation_machine_code_array, int *i,
                                       int j, int line_number)
{
    node *temp;

    /* Initialize register numbers to -1 */
    int register_number1, register_number2;

    /* Calculate the address of the next label after the operation for the extern labels */
    int extern_line = (*i) + j;


    register_number1 = is_register_name(first_operand);

    register_number2 = is_register_name(second_operand);

    /* If both operands are registers  */
    if(register_number1 != -1 && register_number2 != -1)
    {
        /* Set the destination operand to register code */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(REGISTER_CODE, DEST_OP_SHIFT));

        /* Set the source operand to register code */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(REGISTER_CODE, SRC_OP_SHIFT));

        /* Encode the register number at the destination bits*/
        encode_register_number(&operation_machine_code_array[*i+1], (SHIFT_LEFT(register_number1, SRC_REG_SHIFT)));

        /* Encode the register number at the destination bits */
        encode_register_number(&operation_machine_code_array[*i+1], (SHIFT_LEFT(register_number2, DEST_REG_SHIFT)));

        /* Increment the instruction pointer (*i) by 1 to move to the next empty position in the machine code array and return 1 indicating successful encoding. */
        (*i) += 1;

        return 1;
    }

    /* If the first operand is a number, encode it as an immediate operand */
    if(*first_operand == '#')
    {
        /* Set the source operand to immediate code */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(NUMBER_CODE, SRC_OP_SHIFT));

        /* Encode the number value */
        operation_machine_code_array[*i+1] |= (SHIFT_LEFT(get_number_value(first_operand), SKIP_ERA_BITS));
    }

        /* If the first operand is a register, encode it as a register operand */
    else if(register_number1 != -1)
    {
        /* Set the source operand to register code */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(REGISTER_CODE, SRC_OP_SHIFT));

        /* Encode the register number */
        operation_machine_code_array[*i+1] |= (SHIFT_LEFT(register_number1, SRC_REG_SHIFT));
    }

        /* If the first operand is neither a number nor a register, search for the label in the label table */
    else
    {
        temp = search_label(head, first_operand, line_number);

        /* If the label is not found, return an error */
        if(temp == NULL)
            return 0;

        /* Encode label code as the source operand */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(LABEL_CODE, SRC_OP_SHIFT));

        /* Encode the label address */
        encode_label_address(temp, head, &operation_machine_code_array[*i+1], extern_line + 1);
    }

    /* If the second operand is a number, encode it as an immediate operand */
    if(*second_operand == '#')
    {
        /* Set the destination operand to immediate code */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(NUMBER_CODE, DEST_OP_SHIFT));

        /* Encode the number value */
        operation_machine_code_array[*i+2] |= (SHIFT_LEFT(get_number_value(second_operand), SKIP_ERA_BITS));
    }

        /* If the second operand is a register, encode it as a register operand */
    else if(register_number2 != -1)
    {
        /* Set the destination operand to register code */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(REGISTER_CODE, DEST_OP_SHIFT));

        /* Encode the register number */
        operation_machine_code_array[*i+2] |= (SHIFT_LEFT(register_number2, SRC_REG_SHIFT));
    }

        /* If the second operand is neither a number nor a register, search for the label in the label table */
    else
    {
        /* If the label is not found, return an error */
        temp = search_label(head, second_operand, line_number);

        if(temp == NULL)
            return 0;

        /* Encode label code as the destination operand */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(LABEL_CODE, DEST_OP_SHIFT));

        /* Encode the label address */
        encode_label_address(temp, head, &operation_machine_code_array[*i+2], extern_line + 2);
    }

    /* Increment the instruction pointer (*i) by 2 to move to the next empty  position in the machine code array and return 1 indicating successful encoding. */
    (*i) += 2;

    return 1;
}


/*
 * This function encodes an operation with one operand into machine code. 
 * It checks if the operand is a number or a register, and if not, 
 * searches for the operand in the label table. 
 * If the operand is found, the function encodes the label address as the destination operand. 
 * The function returns 1 if the encoding was successful, 0 otherwise.
 */
int encode_operation_with_one_operand(char *operand, char *op_name, node **head, short int *operation_machine_code_array, int *i, int j, int line_number)
{
    node  *temp;

    int register_number;

    /* Calculate the address of the next label after the operation for the extern labels */
    int extern_line = (*i) + j;

    /* Check if the operation is 'prn' and the operand is a number */
    if(strcmp(op_name, PRN) == 0 && *operand == '#')
    {
        /* Encode the number */
        operation_machine_code_array[*i+1] |= (SHIFT_LEFT(get_number_value(operand), SKIP_ERA_BITS));

        /* Increment the instruction pointer (*i) by 1 to move to the next empty position in the machine code array and return 1 indicating successful encoding. */
        (*i)++;

        return 1;
    }

    /* Check if the operand is a register */
    if((register_number = is_register_name(operand)) != -1)
    {
        /* Encode the register code number into the destination bits */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(REGISTER_CODE, DEST_OP_SHIFT));

        /* Encode the register number */
        operation_machine_code_array[*i+1] |= (SHIFT_LEFT(register_number, DEST_REG_SHIFT));

        (*i)++;

        return 1;
    }

    /* Search for the label in the label table */
    temp = search_label(head, operand, line_number);

    if(temp == NULL)
        return 0;


    /* Encode label code as the destination operand */
    operation_machine_code_array[*i] |= (SHIFT_LEFT(LABEL_CODE, DEST_OP_SHIFT));

    /* Encode the label address */
    encode_label_address(temp, head, &operation_machine_code_array[*i+1], extern_line + 1);

    /* Increment the instruction pointer (*i) by 1 to move to the next empty position in the machine code array and return 1 indicating successful encoding. */
    (*i)++;

    return 1;

}


/*
 * This function encodes a single operation with parameters into machine code. 
 * It takes in the operation parameters as a string, a pointer to a linked list of labels,
 * a pointer to the machine code array, an index for the current position in the array, 
 * the number of parameters expected by the operation, and the current line number. 
 * It returns 1 if encoding is successful, and 0 otherwise.
 */
int encode_operation_with_parameters(char *operand, node **head, short int *operation_machine_code_array, int *i, int j, int line_number)
{
    /* Initialize variables */
    char *token1, *token2;

    node *label;

    char register_number1, register_number2;

    /* Calculate the address of the next label after the operation for the extern labels */
    int extern_line = (*i) + j;


    token1 = strtok(operand, "(");

    token2 = strtok(NULL, ",");

    /* Find the label for token1 */
    label = search_label(head, token1, line_number);

    if(label == NULL)
    {
        return 0;
    }

    /* Encode the label address */
    encode_label_address(label, head, &operation_machine_code_array[*i+1], extern_line + 1);

    /* No second parameter */
    if(token2 == NULL)
    {
        /* Encode the label code as destination */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(LABEL_CODE, DEST_OP_SHIFT));

        /* Increment the instruction pointer (*i) by 1 to move to the next empty position in the machine code array and return 1 indicating successful encoding. */
        (*i)++;

        return 1;
    }

    /* There is a second parameter, encode the jump as destination */
    operation_machine_code_array[*i] |= (SHIFT_LEFT(JUMP_CODE, DEST_OP_SHIFT));

    token1 = strtok(NULL, ")");

    /* Check if both parameters are registers */
    if((register_number1 = is_register_name(token1)) != -1 && (register_number2 = is_register_name(token2)) != -1)
    {
        /* Encode the first register into the register destination field */
        encode_register_number(&operation_machine_code_array[*i+2], (SHIFT_LEFT(register_number1, DEST_REG_SHIFT)));

        /* Encode the first register into register source field */
        encode_register_number(&operation_machine_code_array[*i+2], (SHIFT_LEFT(register_number2, SRC_REG_SHIFT)));

        /* Increment the instruction pointer (*i) by 2 to move to the next empty position in the machine code array and return 1 indicating successful encoding. */
        (*i) += 2;

        return 1;
    }

    /* Check if the first parameter is a register */
    if(is_register_name(token2) != -1)
    {
        /* Encode the register code into the parameter 1 field */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(REGISTER_CODE, PARAM1_SHIFT));

        /* Encode the register number */
        encode_register_number(&operation_machine_code_array[*i+2], (SHIFT_LEFT(register_number2, SRC_REG_SHIFT)));
    }

        /* Check if the first parameter is a number */
    else if(*token2 == '#')
    {
        /* Encode the number code into the parameter 1 field */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(NUMBER_CODE, PARAM1_SHIFT));

        /* Encode the number value */
        operation_machine_code_array[*i+2] |= (SHIFT_LEFT(get_number_value(token2), SKIP_ERA_BITS));
    }

        /* If the first parameter is neither a number nor a register, search for the label in the label table */
    else
    {
        /* Encode the label code into the parameter 1 field */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(LABEL_CODE, PARAM1_SHIFT));

        /* Search for the label in the label table */
        label = search_label(head, token2, line_number);

        /* Return an error if there is no such label */
        if(label == NULL)
            return 0;

        /* Encode the label address */
        encode_label_address(label, head, &operation_machine_code_array[*i+2], extern_line + 2);
    }

    /* Check if the second parameter is a register */
    if(is_register_name(token1) != -1)
    {
        /* Encode the register code into the parameter 2 field */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(REGISTER_CODE, PARAM2_SHIFT));

        /* Encode the register number */
        encode_register_number(&operation_machine_code_array[*i+3], (SHIFT_LEFT(register_number1, DEST_REG_SHIFT)));
    }

        /* Check if the second parameter is a number */
    else if(*token1 == '#')
    {
        /* Encode the number code into the parameter 2 field */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(NUMBER_CODE, PARAM2_SHIFT));

        /* Encode the number value */
        operation_machine_code_array[*i+3] |= (SHIFT_LEFT(get_number_value(token1), SKIP_ERA_BITS));
    }

        /* If the second parameter is neither a number nor a register, search for the label in the label table */
    else
    {
        /* Encode the label code into the parameter 2 field */
        operation_machine_code_array[*i] |= (SHIFT_LEFT(LABEL_CODE, PARAM2_SHIFT));

        /* Search for the label in the label table */
        label = search_label(head, token1, line_number);

        /* Return an error if there is no such label */
        if(label == NULL)
            return 0;

        /* Encode the label address */
        encode_label_address(label, head, &operation_machine_code_array[*i+3], extern_line + 3);
    }

    /* Increment the instruction pointer (*i) by 3 to move to the next empty position in the machine code array and return 1 indicating successful encoding. */
    (*i) += 3;

    return 1;
}


/*
 * This function takes a null-terminated string of characters representing a decimal integer
 * and converts it to a signed char value. The string may optionally begin with the '#' character,
 * followed by an optional '+' or '-' character, and then a sequence of decimal digits. The function
 * loops through the digits of the number string, converts them to an integer value, and accumulates
 * the result. Finally, the function multiplies the integer value of the number by its sign and returns
 * it as a char value.
 */
char get_number_value(char* number_string)
{
    int sign = 1; /* Initialize the sign of the number to positive and the result to zero */

    int result = 0;

    /* Initialize a pointer to the beginning of the number string */
    char* ptr = number_string;

    /* Check if the number is starting with '#' character */
    if (*ptr == '#')
    {
        ptr++;
    }

    /* Check if there is an optional '-' character */
    if (*ptr == '-')
    {
        sign = -1;

        ptr++;
    }

        /* Check if there is an optional '+' character */
    else if (*ptr == '+')
    {
        ptr++;
    }

    /*  Loop through the digits of the number string and convert them to an integer value */
    while (*ptr != '\0')
    {
        result = result * DECI + (*ptr - '0');

        ptr++;
    }

    /* Multiply the integer value of the number by its sign and return it as a char */
    return result * sign;
}


/* This function encodes the address of a label. 
 * If the label is an external label, the function sets a flag in the address to indicate that it's an external label 
 * and updates the line number of the label node accordingly. 
 * If the label has already been used before, 
 * the function creates a new label node and appends it to the linked list with the line number of where it's being used. 
 * If the label is not an external label, 
 * the function encodes the label's line number into the address and sets a flag to indicate that the label is defined in the current file.
 */
void encode_label_address(node *label_node, node **head, short int *label_address_line, int line_for_extern)
{
    /* Check if the label is an external label */
    if(get_is_extern(label_node) == 1)
    {
        /* Set the EXTERN_LABEL bit in the label address line */
        *label_address_line |= EXTERN_LABEL;

        /* If the label been defined as extern before and not used before */
        if(get_label_line(label_node) == 1)
            set_label_line(label_node, line_for_extern + START); /* Set the label line to the line number of the  address it being used */

        /* If the label has already been used, creating a new label node with the address it being used */
        else
        {
            /* Allocate memory for a copy of the label name */
            size_t label_name_length = strlen(get_label_name(label_node)) + 1;

            char *copy_name = safe_malloc(label_name_length);

            node *new_label_node, *current;

            strncpy(copy_name, get_label_name(label_node), label_name_length);

            /* Create a new label node */
            new_label_node = create_label(copy_name, line_for_extern + START, 1, 0);

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
        }

        return;
    }

    /* If the label is not an external label encode the label address */
    *label_address_line |= (SHIFT_LEFT(get_label_line(label_node), SKIP_ERA_BITS));

    /* Encode the A,R,E to DEFINED_IN_FILE (10) */
    *label_address_line |= DEFINED_IN_FILE;

}


/* This function encode the register number */
void encode_register_number(short int *label_address_line, int register_number)
{
    *label_address_line |= register_number;
}

/*
 * The function searches the linked list for a node with the same label name as the given character pointer.
 * If a node with a matching label name is found, the function returns a pointer to that node. 
 * If no matching node is found, the function prints an error message to stderr and returns NULL.
 */
node* search_label(node **head, char *label_name, int line_number)
{
    /* Initialize a pointer to the first node of the linked list. */
    node *current = *head;

    /* Loop through the linked list until a node with a matching label_name is found or the end  */
    while(current != NULL)
    {
        /* Check if the label_name of the current node matches the given label_name. */
        if(strcmp(get_label_name(current), label_name) == 0)
        {
            return current; /* Return a pointer to the current node if it matches the label_name. */
        }

        /* Move on to the next node in the linked list. */
        current = get_next_label(current);
    }

    /* Print an error message to stderr if the label_name is not found in the linked list. */
    fprintf(stderr, "Error in line %d: unknown label name - \"%s\"\n", line_number, label_name);

    /* Return NULL if the label_name is not found in the linked list. */
    return NULL;
}

/*
 * This function creates output files for an assembler program, 
 * including an object file with machine code for instructions and data, as well as separate entry and extern files if necessary. 
 * The function takes in the head of a symbol table, arrays for operation and directive machine code,
 * the desired file name, and the instruction and data counters. 
 * It returns 1 if the files were created successfully and 0 otherwise.
 */
int create_output_files(node **head, short int *operation_machine_code_array, short int *directive_machine_code_array, char *file_name, int IC, int DC)
{
    /* Define the extension for the object file */
    char *object_file_extension = ".ob";

    /* Declare a pointer for the object file */
    FILE *object_file;
    
    /* Create a new object file with the given name and extension */
    if((object_file = create_new_file(file_name, object_file_extension)) == NULL)
    {
        return 0; /* If the file cannot be created, return 0 */
    }

    fprintf(object_file, "Base 10 address  Base 2 code\n\n");

    /* Print the value of the instruction counter and data counter to the object file */
    fprintf(object_file, "\t%d   %d\n", IC, DC);

    /* Print the machine code for the operations to the object file */
    print_machine_code(operation_machine_code_array, IC, object_file);

    /* Print the machine code for the directives to the object file */
    print_machine_code(directive_machine_code_array, DC, object_file);

    /* If there are any entry labels in the symbol table, create a new entry file */
    if(has_entry_label(head))
    {
        /* Declare a pointer for the entry file */
        FILE *entry_file;

        /* Define the extension for the entry file */
        char *entry_file_extension = ".ent";

        /* Create a new entry file with the given name and extension */
        if((entry_file = create_new_file(file_name, entry_file_extension)) == NULL)
        {
            return 0; /* If the file cannot be created, return 0 */
        }

        /* Print the entry labels and their addresses to the entry file */
        print_entry_labels(head, entry_file);

        /* Close the entry file */
        fclose(entry_file);
    }

    /* If there are any extern labels in the symbol table, create a new extern file */
    if(has_extern_label(head))
    {
        /* Declare a pointer for the extern file */
        FILE *extern_file;

        /* Define the extension for the extern file */
        char *extern_file_extension = ".ext";

        /* Create a new extern file with the given name and extension */
        if((extern_file = create_new_file(file_name, extern_file_extension)) == NULL)
        {
            return 0; /* If the file cannot be created, return 0 */
        }

        /* Print the extern labels and their addresses to the extern file */

        print_extern_labels(head, extern_file);

        /* Close the extern file */
        fclose(extern_file);
    }

    /*Close the object file */
    fclose(object_file);

    /* Return 1 to indicate that the files were created successfully */
    return 1;
}


/*
 * This function takes a pointer to the head of a linked list and a file pointer to an entry file as input. 
 * It traverses the linked list and prints the label names and line numbers of nodes that are marked as entries to the entry file.
 */
void print_entry_labels(node **head, FILE *entry_file)
{
    /* Start at the head of the linked list */
    node *current = *head;

    /* Traverse the linked list until the end is reached */
    while(current != NULL)
    {
        /* If the current node is marked as an entry, print its label name and line number to the entry file */
        if(get_is_entry(current))
        {
            fprintf(entry_file, "%s\t%d\n", get_label_name(current), get_label_line(current));
        }

        /* Move on to the next node in the linked list */
        current = get_next_label(current);
    }
}


/*
 * This function takes a pointer to the head of a linked list and a file pointer to an extern file as input. 
 * It traverses the linked list and prints the label names and line numbers of nodes that are marked as extern to the extern file.
 */
void print_extern_labels(node **head, FILE *extern_file)
{
    /* Start at the head of the linked list */
    node *current = *head;

    /* Traverse the linked list until the end is reached */
    while(current != NULL)
    {
        /* If the current node is marked as an extern, print its label name and line number to the entry file */
        if(get_is_extern(current))
        {
            fprintf(extern_file, "%s\t%d\n", get_label_name(current), get_label_line(current));
        }

        /* Move on to the next node in the linked list */
        current = get_next_label(current);
    }
}


/*
 * This function takes a pointer to the head of a linked list as input. 
 * It traverses the linked list and checks if any of the nodes are marked as entries. 
 * If it finds an entry node, it returns 1. Otherwise, it returns 0.
 */
int has_entry_label(node** head)
{
    node* current = *head;

    while (current != NULL)
    {
        if(get_is_entry(current))
        {
            return 1;
        }

        current = get_next_label(current);
    }

    return 0;
}


/*
 * This function takes a pointer to the head of a linked list as input. 
 * It traverses the linked list and checks if any of the nodes are marked as extern. 
 * If it finds an extern node, it returns 1. Otherwise, it returns 0.
 */
int has_extern_label(node** head)
{
    node* current = *head;

    while(current != NULL)
    {
        if(get_is_extern(current))
        {
            return 1;
        }

        current = get_next_label(current);
    }

    return 0;
}



void print_machine_code(short int *machine_code_array, int num_elements, FILE *object_file)
{
    int i, j;

    /* Initialize the line number to START (100) */
    static int line_num = START;

    for (i = 0; i < num_elements; i++, line_num++)
    {
        /* Get the next code from the array */
        short int code = machine_code_array[i];

        /* Print the line number */
        fprintf(object_file, "%d\t\t", line_num);

        /* Loop over the bits to print */
        for (j = START_BIT; j <= BITS_TO_PRINT; j++)
        {
            /* Get the value of the current bit */
            int bit = (code >> (BITS_TO_PRINT - j)) & 1;

            /* Print a '/' or '.' character based on the bit value */
            fputc(bit ? '/' : '.', object_file);
        }

        /* Print a newline character to separate lines */
        fprintf(object_file, "\n");
    }
}



