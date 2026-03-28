#include "definitions.h"


FILE* macro_layout(char *file_name);

int second_transition(FILE *am_file, node **head, char *file_name, int IC, int DC);

void free_label_list(node *head);

int first_transition(FILE *am_file, char *file_name, node **head, int *IC, int *DC);

