# IFJ project 2018
# @file Makefile
# @author Tomas Dorda (xdorda00)

CC=gcc
CFLAGS=-lm -Wall -pedantic -O2	#TODO: je potreba matematicka knihovna -lm?
TARGETS = ifj18

.PHONY: all clean run

all: $(TARGETS)

ifj18: main.c scanner.o parser.o code_generator.o
	$(CC) $(CFLAGS) -o $@ $^

parser.o: parser.c error.o scanner.o token_list.o symtable.o
	$(CC) $(CFLAGS) -c $@ $^

scanner.o: scanner.c error.o dynamic_string.o error.o token.o
	$(CC) $(CFLAGS) -c $@ $^

code_generator.o: code_generator.c dynamic_string.o token.o
	$(CC) $(CFLAGS) -c $@ $^


dynamic_string: dynamic_string.c error.o
	$(CC) $(CFLAGS) -c $@ $^

symtable.o: symtable.c error.o dynamic_string.o token.o
	$(CC) $(CFLAGS) -c $@ $^

token_list.o: token_list.c error.o token.o
	$(CC) $(CFLAGS) -c $@ $^

token.o: token.c dynamic_string.o
	$(CC) $(CFLAGS) -c $@ $^

error.o: error.c
	$(CC) $(CFLAGS) -c $@ $^

clean:
	@rm -f *.o
	@echo "All .o files were cleaned out."

run:
	time ./generator_test_1

zip: *.c *.h Makefile rozdeleni
	@zip xdorda00.zip *.c *.h rozdeleni Makefile
	@echo "Project was zipped."
