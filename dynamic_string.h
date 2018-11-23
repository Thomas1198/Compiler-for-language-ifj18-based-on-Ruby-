/**
 * @file dynamic_string.h
 * @author Jan Osolsobe (xosols00)
 * @brief Dynamic string interface
 */

#ifndef _DYNAMIC_STRING_H
#define _DYNAMIC_STRING_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"

#define DYNAMIC_STRING_LENGTH 8

/**
 * @struct Dynamic string
 */
typedef struct
{
    char *str;  //  string
    unsigned int length;    // length of string
    unsigned int alloc_size;    //  number of chars allocated for string
} Dynamic_string;

/**
 * @brief Inicialization dynamic string
 * @param s Pointer to dynamic string
 * @return true if its ok
 */
void  dynamic_string_init(Dynamic_string *s);

/**
 * @brief Frees alocated memory
 * @param s Pointer to dynamic string
 */
void dynamic_string_free(Dynamic_string *s);

/**
 * @brief Clean dynamic string
 * @param s Pointer to dynamic string
 */
void dynamic_string_clear(Dynamic_string *s);

/**
 * @brief Add char to the end of string
 * @param s Pointer to dynamic string
 * @param c char to add
 * @return true if its ok
 */
void dynamic_string_add_char(Dynamic_string *s, char c);

/**
 * @brief Add string to the end of string
 * @param s Pointer to dynamic string
 * @param const_string String to add
 * @return true if its ok
 */
void dynamic_string_add_const_str(Dynamic_string *s, const char *const_string);

/**
 * @brief Compare dynamic string with constante string
 * @param dynamic_string dynamic string
 * @param const_string constant string
 * @return 1 if s1 is greater than s2, 0 if itd equal, -1 if its less
 */
int dynamic_string_cmp_const_str(Dynamic_string *dynamic_string, const char *const_string);

/**
 * @brief Copy string to destination
 * @param src string
 * @param dest destination to copy
 * @return True if its ok
 */
void dynamic_string_copy(Dynamic_string *src, Dynamic_string *dest);

#endif // _DYNAMIC_STRING_H