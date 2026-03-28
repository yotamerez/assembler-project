assembler: assembler.o pre_processor.o pre_processor_utility.o first_transition.o first_transition_utility.o second_transition.o second_transition_utility.o syntax_utility.o file_utility.o structs.o
	gcc -g -ansi -pedantic -Wall assembler.o pre_processor.o pre_processor_utility.o  first_transition.o first_transition_utility.o second_transition.o second_transition_utility.o syntax_utility.o file_utility.o structs.o -o assembler
assembler.o: assembler.c assembler.h
	gcc -g -c -ansi -pedantic -Wall assembler.c -o assembler.o
pre_processor.o: pre_processor.c pre_processor.h
	gcc -g -c -ansi -pedantic -Wall pre_processor.c -o pre_processor.o
pre_processor_utility.o: pre_processor_utility.c pre_processor.h
	gcc -g -c -ansi -pedantic -Wall pre_processor_utility.c -o pre_processor_utility.o
first_transition.o: first_transition.c first_transition.h
	gcc -g -c -ansi -pedantic -Wall first_transition.c -o first_transition.o
first_transition_utility.o: first_transition_utility.c first_transition.h
	gcc -g -c -ansi -pedantic -Wall first_transition_utility.c -o first_transition_utility.o

first_transition_utility.c:
second_transition.o: second_transition.c second_transition.h
	gcc -g -c -ansi -pedantic -Wall second_transition.c -o second_transition.o
second_transition_utility.o: second_transition_utility.c second_transition.h
	gcc -g -c -ansi -pedantic -Wall second_transition_utility.c -o second_transition_utility.o
syntax_utility.o: syntax_utility.c definitions.h
	gcc -g -c -ansi -pedantic -Wall syntax_utility.c -o syntax_utility.o

syntax_utility.c:
file_utility.o: file_utility.c definitions.h
	gcc -g -c -ansi -pedantic -Wall file_utility.c -o file_utility.o
structs.o: structs.c definitions.h
	gcc -g -c -ansi -pedantic -Wall structs.c -o structs.o
