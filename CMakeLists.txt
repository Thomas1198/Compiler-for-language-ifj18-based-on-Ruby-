cmake_minimum_required(VERSION 3.8.2)

set(CMAKE_CXX_STANDARD 99)

FILE(GLOB SOURCES "*.c")
FILE(GLOB HEADERS "*.h")

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES} ${HEADERS} expression.c expression.h)
