/**
 * @file symtable.c
 * @author
 * @brief
 */

#include "symtable.h"
#include "error.h"

/**
 * @brief calculates hash and returning index to table
 * @param str string which is used to calculate hash
 * @return table index.
 */
static unsigned long hash(Dynamic_string *str) {
    const char *key = dynamic_string_to_string(str);    //TODO vytvorit tuto funkci
    unsigned int h = 0;
    const unsigned char *p;
    for(p = (const unsigned char*) key; *p != '\0'; p++)
        h = 65599*h + *p;

    return h % MAX_TABLE_SIZE;
}

void symtable_create(Symtable *table) {
    if (table != NULL)
        for (int i = 0; i < MAX_TABLE_SIZE; i++)
            (*table)[i] = NULL;
}

void symtable_insert(Symtable *table, struct tToken *token ) {
    if (table == NULL || token == NULL || token->content_string == NULL)
        ErrorPrint(INTERNAL_ERROR,"Prazdny ukazatel tabulky, tokenu nebo klice v symtable.c");

    struct htab_item *new_item = (struct htab_item *) malloc(sizeof(struct htab_item));
    if (new_item == NULL)
        ErrorPrint(INTERNAL_ERROR,"Neuspesna alokace nove polozky tabulky v symtable.c");

    unsigned long index = hash(token->content_string);
    new_item->next = table[index];
    table[index] = new_item;
}

struct tToken *symtable_get(Symtable *table, Dynamic_string *key) {
    if (table == NULL || key == NULL)
        ErrorPrint(INTERNAL_ERROR, "Prazdny ukazatel tabulky nebo klice v symtable.c");

    unsigned long index = hash(key);

    for (Titem *tmp = (*table)[index]; tmp != NULL; tmp = tmp->next)
        if (!dynamic_string_cmp_const_str(tmp->data.content_string, key))
            return &tmp->data;

    return NULL;
}

void symtable_remove(Symtable *table, Dynamic_string *key){
    if (table == NULL || key == NULL)
        ErrorPrint(INTERNAL_ERROR, "Prazdny ukazatel tabulky nebo klice v symtable.c");

    unsigned long index = hash(key);
    Titem *previous = (*table)[index];
    for (Titem *tmp = (*table)[index]; tmp != NULL; tmp = tmp->next){
        if (!dynamic_string_cmp_const_str(tmp->data->content_string, dynamic_string_to_string(key))) {
            previous->next = tmp->next;
            free(tmp);
            break;
        }
        previous = tmp;
    }
}

void symtable_destroy(Symtable *table){
    if (table == NULL)
        ErrorPrint(INTERNAL_ERROR, "Prazdny ukazatel tabulky v symtable.c");

    Titem *next = NULL;

    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        for (Titem *tmp = (*table)[i]; tmp != NULL; tmp = next) {
            next = tmp->next;
            free(tmp);
        }

        (*table)[i] = NULL;
    }
}