#include "definitions.h"


/* Function for calloc() that checks if the memory allocation was successful. 
 * If the allocation fails, prints an error message and exits the program. 
 */
void* safe_malloc(size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL)
    {
        fprintf(stderr, "Error: out of memory\n");

        exit(EXIT_FAILURE);
    }

    return ptr;
}


/* Function for malloc() that checks if the memory allocation was successful.
 * If the allocation fails, prints an error message and exits the program.
 */
void* safe_calloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb, size);

    if (ptr == NULL)
    {
        fprintf(stderr, "Error: out of memory\n");

        exit(EXIT_FAILURE);
    }

    return ptr;
}

struct macro 
{
    char *name_of_macro; /* The name of the macro */

    char **macro_content; /* An array of strings representing the content of the macro */

    struct macro *next; /* Pointer to the next macro in the list */
};


/* Create a new macro with the given name and content */
struct macro *create_macro(char *name, char **content) 
{
    struct macro *m = safe_malloc(sizeof(struct macro));

    m -> name_of_macro = name;

    m -> macro_content = content;

    m -> next = NULL;

    return m;
}


/* Get the name of a macro */
char *get_macro_name(struct macro *m) 
{
    return m -> name_of_macro;
}


/* Get the content of a macro */
char **get_macro_content(struct macro *m)
{
    return m -> macro_content;
}


/* Get the next macro in the list */
struct macro *get_macro_next(struct macro *m) 
{
    return m -> next;
}

/* Set the next of a macro */
void set_macro_next(struct macro *m, struct macro **head)
{
    m -> next = *head;
}  

void free_macros_memory(macro_node *head)
{   

    macro_node *temp;

    /* While there are still nodes in the list */
    while(head != NULL)
    {
        int i = 0;

        /* Store the current node to be deleted in a temporary variable */
        temp = head;

        /* Move the head to the next node in the list */
        head = head -> next;

        /* Free the dynamically allocated memory for the macro name */
        free(temp -> name_of_macro);


        /* Free the dynamically allocated memory for the macro content */
        while(temp -> macro_content[i] != NULL)
        {
            free(temp -> macro_content[i]);

            i++;
        }

        /* Free the dynamically allocated memory for the macro content array */
        free(temp -> macro_content);

        /* Free the dynamically allocated memory for the node itself */
        free(temp);
    }

}

/* A struct representing a label in the source file */
struct label 
{
    char *label_name; /* The name of the label */

    int label_line; /* The line of the label */

    char is_extern; /* A flag for extern label */

    char is_entry; /* A flag for entry label */

    struct label *next; /* Pointer to the next label in the list */
   
} label;


/* This function creates new label node */
struct label *create_label(char *label_name, int label_line, char is_extern, char is_entry) 
{
    /* Allocate memory for the new label */
    struct label *new_label = safe_malloc(sizeof(struct label));

    /* Set the values of the new label */
    new_label->label_name = label_name;

    new_label -> label_line = label_line;

    new_label -> is_extern = is_extern;

    new_label -> is_entry = is_entry;

    new_label -> next = NULL;

    return new_label;
}


/* Getter function for the label name */
char *get_label_name(struct label *lbl)
{
    return lbl -> label_name;
}

/* Getter function for the label line */
int get_label_line(struct label *lbl)
{
    return lbl -> label_line;
}

/* Setter function for the label line */
void set_label_line(struct label *lbl, int line)
{
    lbl -> label_line = line;
}

/* Getter function for the is_extern flag */
char get_is_extern(struct label *lbl)
{
    return lbl -> is_extern;
}

/* Getter function for the is_entry flag */
char get_is_entry(struct label *lbl)
{
    return lbl -> is_entry;
}

/* Setter function for the is_entry flag */
void set_is_entry(struct label *lbl, char is_entry)
{
    lbl -> is_entry = is_entry;
}

/* Getter function for the next label pointer */
struct label *get_next_label(struct label *lbl)
{
    return lbl -> next;
}

/* Setter function for the next label pointer */
void set_next_label(struct label *lbl, struct label *next)
{
    lbl -> next = next;
}

void free_label_list(node *head)
{
    node *current = head;

    /* Loop through each node in the linked list */
    while (current != NULL)
    {
        /* Create a pointer to the next node in the linked list*/
        node *next = current -> next;

        /* Free the memory allocated for the label name string in the current node */
        free(current -> label_name);

        /* Free the memory allocated for the current node */
        free(current);

        /* Update the current node pointer to point to the next node in the linked list */
        current = next;
    }
}

struct op_code
{   
    char *op_name; /* The name of the operation */

    int op_number; /* The operation opcode */
};

static op_code op_codes[] = 
{
    {MOV, MOV_OP_CODE},
    {CMP, CMP_OP_CODE},
    {ADD, ADD_OP_CODE},
    {SUB, SUB_OP_CODE},
    {NOT, NOT_OP_CODE},
    {CLR, CLR_OP_CODE},
    {LEA, LEA_OP_CODE},
    {INC, INC_OP_CODE},
    {DEC, DEC_OP_CODE},
    {JMP, JMP_OP_CODE},
    {BNE, BNE_OP_CODE},
    {RED, RED_OP_CODE},
    {PRN, PRN_OP_CODE},
    {JSR, JSR_OP_CODE},
    {RTS, RTS_OP_CODE},
    {STOP, STOP_OP_CODE}
};

int get_op_code_by_name(char *name)
{
    int i;

    for (i= 0; i < sizeof(op_codes)/sizeof(op_codes[0]); i++)
    {
        if (strcmp(op_codes[i].op_name, name) == 0)
        {
            return op_codes[i].op_number;
        }
    }

    return -1;
}

/*
 * This function takes a string as input and checks if it matches with a known operation name.
 * If the string has a colon (:) character, it is assumed to be a label and the colon is removed.
 * It then compares the remaining string with an array of possible operation names.
 * If a match is found, the corresponding operation name is returned.
 * If no match is found, NULL is returned.
 */
char* get_op_name(char *str)
{
    int i;

    /* Check if the string has a colon (:) character */
    char *colon_ptr = strchr(str, ':');

    /* If there is a colon, replace it with a null terminator (it means the string is a label) */
    if (colon_ptr != NULL)
    {
        *colon_ptr = '\0';
    }

    /* Loop through the array of possible operation names */
    for(i = 0; i < NUM_OF_OPS; i++)
    {
        /* If the current operation name matches the string, return it */
        if(strcmp(str, op_codes[i].op_name) == 0)
        {
            /* If there was a colon in the original string, put it back */
            if(colon_ptr != NULL)
            {
                *colon_ptr = ':';
            }

            return op_codes[i].op_name;
        }
    }


    /* If there was a colon in the original string, put it back */
    if(colon_ptr != NULL)
    {
        *colon_ptr = ':';
    }

    /* If no operation name was found, return NULL */
    return NULL;
}


