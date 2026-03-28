#include "definitions.h"


/* Functions related to encoding instructions and directives: */

/* Encodes an operation instruction with a single parameter and updates the machine code array. */
int encode_operation_with_parameters(char *operand, node **head, short int *operation_machine_code_array, int *i, int j, int line_number);

/* Encodes an operation instruction and updates the machine code array. */
int encode_operation_line(char *line, node **head, short int *operation_machine_code_array, int *i, int j, int line_number);

/* Encodes a directive instruction and updates the machine code array. */
void encode_directive_line(char *line, short int *directive_machine_code_array, int *j);

/* Encodes a string directive and updates the machine code array. */
void encode_string_directive(char *operand, short int *directive_machine_code_array, int *j);

/* Encodes a data directive and updates the machine code array. */
void encode_data_directive(char *operand, short int *directive_machine_code_array, int *j);



/* Functions related to labels and addresses: */

/* Searches for a label with the given name in the label linked list and returns a pointer to the corresponding node if found, or NULL otherwise. */
node* search_label(node **head, char *label_name, int line_number);

/* Encodes the address of a label and updates the label address line array. If the label is external, adds it to the externals linked list. */
void encode_label_address(node *label_node, node **head, short int *label_address_line, int line_for_extern);


/* Functions related to encoding operands and register numbers: */

/* Encodes an operation instruction with one operand and updates the machine code array. */
int encode_operation_with_one_operand(char *operand, char *op_name, node **head, short int *operation_machine_code_array, int *i, int j, int line_number);

/* Encodes an operation instruction with two operands and updates the machine code array. */
int encode_operation_with_two_operands(char *first_operand, char *second_operand, node **head, short int *operation_machine_code_array, int *i, int j, int line_number);

/* Encodes a register number and updates the label address line array. */
void encode_register_number(short int *label_address_line, int register_number);

/* Converts a string representing a number to a char value. */
char get_number_value(char* number_string);


/* Functions related to parsing and identifying instruction and directive names: */

/* Extracts the operation name from a line of assembly code and returns a pointer to the name string. 
   Return NULL if there is no matching operation */
char* get_op_name(char *str);

/* Searches for an operation with the given name in the operations table and 
   returns the op code of the operation. */
int get_op_code_by_name(char *name);

/* Extracts the directive name from a line of assembly code and returns a pointer to the name string. 
   If no directive name is found in the input string, returns NULL */
char* get_directive_name(char *str);



/* Functions related to identifying register names: */

/* Checks if a given string represents a valid register name in the assembly language. */
int is_register_name(char *str);



/* Functions for memory allocation: */

void* safe_calloc(size_t nmemb, size_t size);

void* safe_malloc(size_t size);



/* Functions for outputting machine code to files: */

/* Prints the machine code array to the given file. */
void print_machine_code(short int *machine_code_array, int num_elements, FILE *object_file);

/* Creates a new file with the given name and extension. Returns a pointer to the new file. */
FILE* create_new_file(char *file_name, char *extension);

/* Checks if the given file is empty. */
int check_if_file_is_empty(FILE *file, char *file_name);

/* Opens the source file with the given name. If the file does not exist or cannot be opened, prints an error message and return NULL */
FILE* open_source_file(char *file_name);

/* Creates the output files for the assembly program. The output files include the object file, entry file, and extern file */
int create_output_files(node **head, short int *operation_machine_code_array, short int *directive_machine_code_array, char *file_name, int IC, int DC);



/* Functions for working with labels: */

/* Checks if there are any labels with the "entry" attribute in the linked list of labels. */
int has_entry_label(node** head);

/* Checks if there are any labels with the "extern" attribute in the linked list of labels. */
int has_extern_label(node** head);

/* Prints all labels with the "entry" attribute to a given file pointer */
void print_entry_labels(node **head, FILE *entry_file);

/* Prints all labels with the "extern" attribute to a given file pointer. */
void print_extern_labels(node **head, FILE *extern_file);




/* This function create a label and return a pointer to the label struct */
struct label *create_label(char *label_name, int label_line, char is_extern, char is_entry);

/* This function get the name of a label */
char *get_label_name(struct label *lbl);

/* This function set the line number of a label */
int get_label_line(struct label *lbl);

/* This function set the line number of a label */
void set_label_line(struct label *lbl, int line);

/* This function get the 'is_extern' flag of a label */
char get_is_extern(struct label *lbl);

/* This function get the 'is_entry' flag of a label */
char get_is_entry(struct label *lbl);

/* This function set the 'is_entry' flag of a label */
void set_is_entry(struct label *lbl, char is_entry);

/* This function get a pointer to the next label in a linked list */
struct label *get_next_label(struct label *lbl);

/* This function set a pointer to the next label in a linked list */
void set_next_label(struct label *lbl, struct label *next);

