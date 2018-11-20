cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 99)

add_executable(ifj main.c error.c error.h scanner.c scanner.h symtable.c symtable.h parser.c parser.h parser.c dynamic_string.c dynamic_string.h token.c token.h token_list.h token_list.c code_generator.h code_generator.c)