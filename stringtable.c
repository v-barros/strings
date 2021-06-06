/*
 *MIT License
 *
 *Copyright (c) 2021 v-barros
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 *
 * hashtable.c
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 * 
 * 
 */
#include "stringtable.h"
#include "string.h"

static const size_t _Table = sizeof(struct Table);
const void *Table = &_Table;

struct Table *new_table()
{
    struct Table *table = alloc(Table);
    table->rehash_multiple = REHASH_MULTIPLE;
    table->rehash_count = REHASH_COUNT;
    table->table_size = TABLE_SIZE;
    return table;
}
/**
 * Bucket verification
 * */
int hash_validate(unsigned long full_hash);

int hash_validate(unsigned long full_hash)
{
    int h = full_hash % TABLE_SIZE;
    assert((h >= 0 && h < TABLE_SIZE));
    return h;
}

/* DJB2. See: https://theartincode.stanis.me/008-djb2/ */
unsigned long hash_gen(const char *str);

unsigned long hash_gen(const char *str)
{
    unsigned long hash = 5381;
    int8_t c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

struct String *basic_add(struct Table *table, const char *str, unsigned short str_len)
{
    unsigned long full_hash = hash_gen(str);
    int index = hash_validate(full_hash);
    struct String *new_string;

    if (is_empty_bucket(table, index))
    {
        new_string = create_string(str, str_len, full_hash);
        inc_ref_count(new_string);
        *(table->table + index) = new_string;
        inc_num_of_entries(table);
        set_interned(new_string);
        return new_string;
    }
    return lookup(table, index, str, str_len, full_hash);
}

struct String *lookup(struct Table *table, int index, const char *name, unsigned short name_len, unsigned long full_hash)
{
    struct String *string = *(table->table + index);
    assert(string);
    struct String * previous = string;
    do
    {
        if (equals_char_l(string, name, name_len))
        {
            inc_ref_count(string);
            return string;
        }
        previous = string;
        string = string->next;
    }while (string);
    /*reached end of bucket*/

    struct String *new_string = create_string(name, name_len, full_hash);
    inc_ref_count(new_string);
    set_interned(new_string);
    
    previous->next = new_string;
    inc_num_of_entries(table);
    return new_string;
}

bool is_empty_bucket(struct Table * table, int index){
    return(!(*(table->table+index)));
}

int table_size(struct Table * table){
    return table->table_size;
}

void debug_table(struct Table * table ){
    int i =0;
    printf("\n");
	while(i<table->table_size){
		printf("0x%p ",*(table->table+i++));
	}
    printf("\n");
}

void inc_num_of_entries(struct Table * table){
    table->number_of_entries++;
}

void dec_num_of_entries(struct Table * table){
    table->number_of_entries--;
}

int number_of_entries(struct Table * table){
    return table->number_of_entries;
}

void set_interned(struct String * string ){
    string->is_interned = true;
}

bool is_interned(struct String * string){
    return string->is_interned;
}

bool needs_rehashing(int count);

void rehash_table();


struct String * lookup_and_get_previous(struct String * string, const char * str, unsigned long full_hash);
