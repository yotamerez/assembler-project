#include "assembler.h"

/*
 * The main function is the entry point of the program. 
 * It first checks if there are any command-line arguments passed. 
 * If there are no arguments, it prints an error message and exits. 
 * Otherwise, it loops through each file passed as an argument and performs the necessary operations on each file.
 * For each file, the function initializes the head of the label list to NULL,
 * creates a macro layout file, performs the first transition, and then performs the second transition if no errors occurred in the first transition. 
 * Finally, it frees the memory allocated for the label list and closes the current file being processed.
 */

/*
 * For the tester's convenience, 
 * it's important to note that the program will not proceed to the next step if it encounters an error in the previous step.
 * For example, if there is an error in the macro layout, then the first transition will not occur until the error is fixed.
 * Therefore, the first transition will thoroughly check almost every syntax error in the file.
 * On the other hand, the second transition will only check whether a label being used as an operand exists in the label list.
 * Additionally, the preprocessor will only permit macro names that are not operations, directives, or registers and not already used by a macro.
 * Furthermore, the preprocessor will print an error message 
 * if there are any non-whitespace characters after a macro name or "end mcr," but it will ignore it and still proceed to the first transition. 
   And finally the program will not proceed to the first transition if there is a line with more then 80 charcaters.
 */
int main(int argc, char *argv[])
{
    /* No files to be processed */
    if(argc == 1)
    {
        fprintf(stderr, "Error, missing argument. Please enter file name(s) to be processed\n");

    }

    /* Loop through each file passed as argument */
    while(argc-- > 1)
    {
        FILE *am_file;

        /* Initialize head of label list to NULL*/
        node *head = NULL;

        char error_status = 0;

        int IC = START, DC = 0;

        ++argv;

        /* Create macro layout file */
        am_file = macro_layout(*argv);

        /* Check if macro layout was successful */
        if(am_file != NULL)
        {
            /* Perform first transition */
            if(first_transition(am_file, *argv, &head, &IC, &DC) == 1)
                error_status = 1;
        }

        /* The macro layout failed, proceeding to the next file */
        else
            continue;


        /* Perform second transition if no errors occurred in first transition */
        if(!error_status)
            second_transition(am_file, &head, *argv, IC, DC);


        /* Free memory allocated for label list */
        if(head != NULL)
            free_label_list(head);

        /* Close current file being processed */
        fclose(am_file);
    }

    return 0;
}


