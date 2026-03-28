#include "definitions.h"


/* This function opens a file with the given file name in read mode and returns a pointer to the file. */
FILE* open_source_file(char *file_name);

/* This function checks if the given file is empty or not. If the file is empty, it returns 1, otherwise it returns 0. */
int check_if_file_is_empty(FILE *file, char *file_name);

/* This function creates a new file with the given file name and extension in write mode and returns a pointer to the file. */
FILE* create_new_file(char *file_name, char *extension);

/* This function adds the given extension to the file name and returns a new string containing the modified file name. */
char* add_extension(char *file_name, char* extension);



/* This function replaces macro definitions in the source file with their corresponding content and writes the output to the destination file. */
int replace_macro_definitions(FILE *source_file, FILE *dest_file, char *file_name);




/* This function extracts the macro name from the given string and returns it as a new string. */
char* get_macro_name_from_file(char *str, int line_number);

/* This function reads the macro content from the source file and returns it as an array of strings. */
char** get_macro_content_from_file(FILE *source_file, char *line_number, char *error_status);

/* This function ensures that the given array of strings has enough capacity to store the given number of lines.
   If necessary, it reallocates the memory for the array. */
char** ensure_capacity(char **lines, int num_lines, int *capacity);




/* This function checks if the given macro name is legal or not. It returns 1 if the macro name is legal, otherwise it returns 0. */
int check_if_macro_name_legal(char *macro_name, int line, macro_node **head);

/* This function checks if the given string is a register name. It returns 1 if the string is a register name, otherwise it returns 0. */
int is_register_name(char *str);

/* This function finds the macro node with the given name in the macro list and returns a pointer to it. If the macro node is not found, it returns NULL. */
macro_node* find_macro_by_name(char *name, macro_node **head);

/* This function extracts the directive name from the given string and returns it as a new string. */
char* get_directive_name(char *str);

/* This function extracts the op name from the given string and returns it as a new string. */
char* get_op_name(char *str);




/* This function removes any trailing whitespace characters from the given string. */
void remove_trailing_whitespace(char *str);

/* This function checks if there are any non-white characters after the given string in the given line. It returns 1 if there are, otherwise it returns 0. */
int has_non_white_chars_after_string(char *line, char *string);

/* This function remove extra tabs and blanks and replace them with a single blank. */
void remove_extra_spaces(char* str);




/* This function adds a new macro node to the macro list with the given macro content and name. */
void add_macro_node(macro_node **head, char **macro_content, char *macro_name);




/* This function frees the memory used by the macro list. */
void free_macros_memory(macro_node *head);

/* This function frees the memory used by the array of strings representing the macro content. */
void free_macro_content(char **lines, int num_lines);

/* This function safely allocates the given amount of memory and returns a pointer to the allocated memory. */
void* safe_malloc(size_t size);




/* This function print the content of the macro to the new file (the .am file). */
int print_macro_content_by_name(char *name, macro_node **head, FILE *dest_file);

/* This function creates a new macro with the given name and content, and returns a pointer to it. */
struct macro *create_macro(char *name, char **content);

/* This function returns the name of the macro pointed to by m as a string.*/
char *get_macro_name(struct macro *m);

/* This function returns the content of the macro pointed to by m as an array of strings. */
char **get_macro_content(struct macro *m);

/* This function returns a pointer to the next macro in the list after the one pointed to by m.*/
struct macro *get_macro_next(struct macro *m);

/* This function sets the next pointer of the macro pointed to by m to the specified macro pointer head. */
void set_macro_next(struct macro *m, struct macro **head);


