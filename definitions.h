#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>


/* These are macros that define opcode names as string literals. */
#define MOV "mov"
#define CMP "cmp"
#define ADD "add"
#define SUB "sub"
#define NOT "not"
#define CLR "clr"
#define LEA "lea"
#define INC "inc"
#define DEC "dec"
#define JMP "jmp"
#define BNE "bne"
#define RED "red"
#define PRN "prn"
#define JSR "jsr"
#define RTS "rts"
#define STOP "stop"

/* These are macros that define the operation code for the various supported operations in the assembly language */
#define MOV_OP_CODE 0
#define CMP_OP_CODE 1
#define ADD_OP_CODE 2
#define SUB_OP_CODE 3
#define NOT_OP_CODE 4
#define CLR_OP_CODE 5
#define LEA_OP_CODE 6
#define INC_OP_CODE 7
#define DEC_OP_CODE 8
#define JMP_OP_CODE 9
#define BNE_OP_CODE 10
#define RED_OP_CODE 11
#define PRN_OP_CODE 12
#define JSR_OP_CODE 13
#define RTS_OP_CODE 14
#define STOP_OP_CODE 15


/* A typedef for convenience */
typedef struct op_code op_code;




#define DATA ".data"
#define STRING ".string"
#define EXTERN ".extern"
#define ENTRY ".entry"



/* Constants used by the preprocessor */
#define LENGTH_OF_LINE 81  /* The maximum length of a line in the source file, including the newline character */
#define LENGTH_OF_REGISTER_NAME 2 /* The number of characters in a register name (e.g. "r0") */
#define MAX_LENGTH_OF_MACRO_NAME 30 /* The maximum length of a macro name */
#define ADD_EXTRA_LINES 4 /* The number of extra lines to add to the input buffer when resizing it */



/* A typedef for convenience */
typedef struct macro macro_node; 






#define MAX_LABEL_NAME 30 /* The maximum number of characters that can be used to name a label */
#define MAX_LINE_LENGTH 81 /* The maximum length of a line in the .as file, including the newline character + 1*/
#define NUM_OF_OPS 16 /* The number of opcodes available */
#define MAX_14BIT_INT  16383
#define MIN_14BIT_INT  (-16384)




/* A typedef for convenience */
typedef struct label node; 



/* Codes for the different types of operands */
#define NUMBER_CODE 0 /* Operand is a numeric value */
#define LABEL_CODE 1 /* Operand is a label */
#define JUMP_CODE 2 /* Operand is a label to jump to */
#define REGISTER_CODE 3 /* Operand is a register */

/* Shift values for encoding and decoding instructions */
#define DEST_OP_SHIFT 2 /* Shift for destination operand type */
#define SRC_OP_SHIFT 4 /* Shift for source operand type */
#define OPCODE_SHIFT 6 /* Shift for opcode */
#define PARAM2_SHIFT 10 /* Shift for parameter 2 */
#define PARAM1_SHIFT 12 /* Shift for parameter 1 */

#define DEST_REG_SHIFT 2 /* Shift for destination register */
#define SRC_REG_SHIFT 8 /* Shift for source register */

#define SKIP_ERA_BITS 2 /* Number of bits to skip in ERA encoding */

/* Values for defining the state of a label */
#define DEFINED_IN_FILE 2 /* Label is defined in current file */
#define EXTERN_LABEL 1 /* Label is defined in an external file */

#define SHIFT_LEFT(x, y) ((x) << (y)) /* Macro for shifting bits to the left */

#define BITS_TO_PRINT 14 /* Number of bits to print for debugging */
#define START_BIT 1 /* Starting bit for printing bits */

#define DECI 10 /* Base 10 for integer conversion */


#define START 100 /* Starting memory address for program */
#define MAX_LABEL_NAME 30 /* Maximum length of a label name */
#define MAX_CHAR_VALUE 127 /* Maximum value of a character */
#define MAX_ENCODED_LINES 155 /* Maximum number of encoded lines */

/* Encoding types for different numbers of operands */
#define ENCODING_LINES_NO_OPERAND 0 /* No operands */
#define ENCODING_LINES_ONE_OPERAND 1 /* One operand */
#define ENCODING_LINES_TWO_OPERANDS 2 /* Two operands */
#define ENCODING_LINES_WITH_PARAMETERS 3 /* Operands with additional parameters */



