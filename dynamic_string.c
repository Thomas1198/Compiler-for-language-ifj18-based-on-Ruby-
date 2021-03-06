/**
 * @file dynamic_string.c
 * @author Jan Osolsobe (xosols00)
 * @brief Dynamic string interface
 */

#include "dynamic_string.h"

void dynamic_string_clear(Dynamic_string *s) {
    s->length = 0;
    s->str[s->length] = '\0';
}

Dynamic_string *dynamic_string_init() {
    Dynamic_string *s;
    s = (Dynamic_string *) malloc(sizeof(Dynamic_string));
    if (s == NULL)
        ErrorPrint(INTERNAL_ERROR, "Internal fault in dynamic_string.c");

    s->str = (char *) malloc(DYNAMIC_STRING_LENGTH);
    if (s->str == NULL)
        ErrorPrint(INTERNAL_ERROR, "Internal fault in dynamic_string.c");

    dynamic_string_clear(s);
    s->alloc_size = DYNAMIC_STRING_LENGTH;

    return s;
}

void dynamic_string_free(Dynamic_string *s) {
    if (s != NULL) {
        dynamic_string_clear(s);
        free(s->str);
    }

}

void dynamic_string_add_char(Dynamic_string *s, char c) {
    if ((s->length + 1) >= s->alloc_size) {
        unsigned int new_size = s->length + DYNAMIC_STRING_LENGTH;
        if ((s->str = (char *) realloc(s->str, new_size)))
            s->alloc_size = new_size;
        else
            ErrorPrint(INTERNAL_ERROR, "Internal fault in dynamic_string.c");
    }
    s->str[s->length++] = c;
    s->str[s->length] = '\0';
}

void dynamic_string_add_const_str(Dynamic_string *s, const char *const_string) {
    unsigned int const_str_len = (unsigned int) strlen(const_string);
    if ((s->length + const_str_len + 1) >= s->alloc_size) {
        unsigned int new_size = s->length + const_str_len + 1;
        if ((s->str = (char *) realloc(s->str, new_size)))
            s->alloc_size = new_size;
        else
            ErrorPrint(INTERNAL_ERROR, "Internal fault in dynamic_string.c");
    }
    s->length = s->length + const_str_len;
    strcat(s->str, const_string);
    s->str[s->length] = '\0';
}

int dynamic_string_cmp_const_str(Dynamic_string *dynamic_string, const char *const_string) {
    int result = strcmp(dynamic_string->str, const_string);
    return result;
}

void dynamic_string_copy(Dynamic_string *src, Dynamic_string *dest) {
    unsigned int new_len = src->length;
    if (new_len >= dest->alloc_size) {
        if ((dest->str = (char *) realloc(dest->str, new_len)))
            dest->alloc_size = new_len + 1;
        else
            ErrorPrint(INTERNAL_ERROR, "Internal fault in dynamic_string.c");
    }
    strcpy(dest->str, src->str);
    dest->length = new_len;
}