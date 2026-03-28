#include "definitions.h"


/* This function analyse each line in the .am file */
int line_analysis(char *line, int line_number, int label_line, node **head, int *DC, char *file_name);


/* Functions for checking line syntax: */

/* This function checks whether commas in the given string are placed in a legal syntax. */
int is_comma_syntax_legal(char *str, int line_counter, char *file_name);

/* This function checks whether there are consecutive commas in the given string. */
int check_consecutive_commas(char *str);

/* This function checks whether the given string ends with a comma. */
int ends_with_comma(char *str);

/* This function checks whether the given string starts with a dot. */
int starts_with_dot(const char *str);

/* This function checks whether the given string starts with an alphabetic character. */
int starts_with_alpha(const char *str);

/* This function checks whether the given string contains non-alphabetic characters. */
int contains_non_alpha_chars(char *str);

/* This function checks whether the given string ends with a colon. */
int end_with_colon(char *str, int line_number, char print, char *file_name);



/* Functions for checking label syntax: */

/* This function checks if a label name is valid according to the assembly language rules, and returns 1 if it is, and 0 otherwise.*/
int is_label_name_valid(char *str, int line_number, char print, char *file_name);

/* This function checks if a line starts with a label, and if so, adds it to the linked list of labels.*/
int line_start_with_label(char *line, int line_number, node **head, int *DC, char *file_name);

/* This function removes the colon character from the end of a string. */
char* remove_colon(char *str);



/* Functions for checking directive syntax: */

/* This function returns the name of a directive from a given string. */
char* get_directive_name(char *str);

/* This function checks if a given string represents a valid parameter for a directive. */
int is_parameters_legal_numbers(char *str, int line_number, char *file_name);

/* This function checks the syntax of a directive's operand and performs relevant actions. */
int directive_operand(char *directive_name, char *operand, int line_number, char after_label, node **head, char *file_name);


/* Functions for checking operation syntax: */

/* This function returns the name of an operation from a given string. */
char* get_op_name(char *str);

/* This function checks if a given string represents a valid operation and returns its name.*/
char* check_op(char *str, int line_number, char print, char *file_name);

/* This function checks the syntax of an operation with no operands and performs relevant actions. */
int operations_with_no_operands(char *op_name, int line_number, char *file_name);

/* This function checks the syntax of an operation with one operand and performs relevant actions. */
int operations_with_one_operand(char *operand, char *op_name, int line_number, char *file_name);

/* This function checks the syntax of an operation with two operands and performs relevant actions. */
int operations_with_two_operands(char *first_operand, char *second_operand, char *op_name, int line_number, char *file_name);

/* This function checks the syntax of an operation with parameters and performs relevant actions. */
int operations_with_parameters(char *operand, int line_number, char *file_name);

/* This function checks if a number is legal. */
int is_operand_legal_number(const char* str);


/* Utility functions: */

/* This function removes leading spaces from the given string */
void remove_leading_spaces(char *str);

/* This function removes comma from the given string */
void remove_comma(char *str);

/* This function checks if the given string is a valid register name */
int is_register_name(char *str);

/* This function checks if parentheses are balanced in the given string */
int check_parentheses(char *str);

/* This function safely allocates memory and returns a pointer to the allocated memory */
void* safe_malloc(size_t size);

/* This function removes trailing whitespace from the given string */
void remove_trailing_whitespace(char *str);

/* This function checks the syntax of an operation line, it returns the number of lines required to encode the line or -1 if there is an error*/
int op_line(char *line, char *op_name, int line_number, char *file_name);

/* This function checks the syntax of a directive line, it returns the number of lines required to encode the line or -1 if there is an error*/
int directive_line(char *line, char *directive_name, int line_number, node **head, char *file_name);

/* This function adds a label to the symbol table, along with its line number and flags indicating if it's an extern or entry label. */
int add_label_to_list(char *label_name, int label_line, node **head, char is_extern, char is_entry, int line_number, char *file_name);

/* This function checks for any errors for the entry line */
int entry_line(node *current, int line_number, char *file_name);

/* This function checks for any errors for the extern line */
int extern_line(node *current, int line_number, char *file_name);

/* This function checks if a string is legal */
int is_string_legal(char *str, int line_number, char *file_name);

/* This function create a label and return a pointer to the label struct */
struct label *create_label(char *label_name, int label_line, char is_extern, char is_entry) ;

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

/* This function get the opcode value of an instruction given its name */
int get_op_code_by_name(char *name);


