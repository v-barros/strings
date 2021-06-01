/*
 * hashtable.c
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 * 
 * Simplified implementation of hashtable, based on pydict: https://github.com/python/cpython/blob/main/Objects/dictobject.c
 * and hostpot openjdk symbol table: http://hg.openjdk.java.net/jdk8/jdk8/hotspot/file/87ee5ee27509/src/share/vm/classfile/symbolTable.hpp
 * 
 */
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "string.h"
#include "stringtable.h"
#include "memory.h"

static const size_t _Table = sizeof(struct Table);
const void *Table = &_Table;


struct Table * create_table()
{
    struct Table * table = alloc(Table);
    table->rehash_multiple = 60;
    table->table_size=TABLE_SIZE;
    return table;
}

struct String *lookup_and_get_previous(struct String *string, const char *str, unsigned long full_hash)
{
    assert(string);
    struct String *previous = string;
    do
    {
        if (!(string->hash ^ full_hash))
        {
            /*
			*	It is kind of redundant, but considering there might be hundreds of nodes 
			*	in a single position on table and comparison using integers is faster than using strings, 
			*   with this aproach we can compare only a few strings even on worst case scenario.
			*/
            if (!(strcmp(string->text, str)))
                return previous;
        }
        previous = string;
        string = string->next;
    } while (string);
    return previous;
}