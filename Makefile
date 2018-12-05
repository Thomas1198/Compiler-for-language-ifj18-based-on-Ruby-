# IFJ project 2018
# @file Makefile
# @author Tomas Dorda (xdorda00)
#TODO: je potreba matematicka knihovna -lm?
CC=gcc
CFLAGS=-lm -Wall -pedantic -O2
TARGETS = ifj18

.PHONY: all clean run

all: $(TARGETS)

ifj18: main.c scanner.o parser.o code_generator.o dynamic_string.o symtable.o token_list.o token.o error.o expression.o stack.o
	$(CC) $(CFLAGS) $^ -o $@

parser.o: parser.c error.o scanner.o token_list.o symtable.o
	$(CC) $(CFLAGS) $^ -c

scanner.o: scanner.c error.o dynamic_string.o error.o token.o
	$(CC) $(CFLAGS) $^ -c

code_generator.o: code_generator.c dynamic_string.o token.o
	$(CC) $(CFLAGS) $^ -c


dynamic_string.o: dynamic_string.c error.o
	$(CC) $(CFLAGS) $^ -c

symtable.o: symtable.c error.o dynamic_string.o token.o
	$(CC) $(CFLAGS) $^ -c

token_list.o: token_list.c error.o token.o
	$(CC) $(CFLAGS) $^ -c

token.o: token.c dynamic_string.o
	$(CC) $(CFLAGS) $^ -c

error.o: error.c error.h
	$(CC) $(CFLAGS) $^ -c

expression.o: expression.c expression.h
	$(CC) $(CFLAGS) $^ -c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) $^ -c


clean:
	@rm -f *.o *.zip ifj18
	@echo "All .o and .zip files were cleaned out."

run:
	time ./generator_test_1

zip: *.c *.h Makefile rozdeleni
	@zip xdorda00.zip *.c *.h rozdeleni Makefile
	@echo "Project was zipped."
