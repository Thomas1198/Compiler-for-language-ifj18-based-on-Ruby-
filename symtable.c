/**
 * @file symtable.c
 * @author
 * @brief
 */

#include "symtable.h"

/**
 * @brief calculates hash and returning index to table
 * @param str string which is used to calculate hash
 * @return table index.
 */
static unsigned long hash(const char *str)
{
    unsigned int h = 0;
    const unsigned char *p;
    for(p = (const unsigned char*) str; *p != '\0'; p++)
        h = 65599*h + *p;

    return h % MAX_TABLE_SIZE;
}

void symtable_create(Symtable *table) {
    if (table != NULL)
        for (int i = 0; i < MAX_TABLE_SIZE; i++)
            (*table)[i] = NULL;
}

struct tToken *symtable_insert(Symtable *table, const char *key){

}

struct tToken *symtable_get(Symtable *table, const char *key){

}

bool symtable_remove(Symtable *table, const char *key){

}

void symtable_destroy(Symtable *table){
    if (table == NULL)
        return;

    Titem *tmp_next = NULL;

    for (int i = 0; i < MAX_TABLE_SIZE; i++)
    {
        for (Titem *tmp = (*table)[i]; tmp != NULL; tmp = tmp_next)
        {
            tmp_next = tmp->next;
            free(tmp->key);

            if (tmp->data.params != NULL)
            {
                dynamicstring_free(tmp->data.params);
                free(tmp->data.params);
            }

            free(tmp);
        }

        (*table)[i] = NULL;
    }
}