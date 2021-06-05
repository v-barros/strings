/*
 * hashtable.c
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 * 
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


struct Table * new_table()
{
    struct Table * table = alloc(Table);
    table->rehash_multiple = REHASH_MULTIPLE;
    table->rehash_count = REHASH_COUNT;
    table->table_size = TABLE_SIZE;
    return table;
}

/**
 * Bucket verification
 * */
int hash_validate(unsigned long full_hash);


int hash_validate(unsigned long full_hash){
    int h = full_hash % TABLE_SIZE;
	assert(!(h >= 0 && h < TABLE_SIZE));
	return h;
}


/* DJB2. See: https://theartincode.stanis.me/008-djb2/ */
unsigned long hash_gen(const char * str);

unsigned long hash_gen(const char * str){
    unsigned long hash = 5381;
	int8_t c;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

/**
 * Given a char array, try to insert a new structure on the table
 * If the String already exists on the table, increases the ref_count and returns 
 * a pointer to the interned String.
 * If it doesnt, insert the string on the table and returns a pointer to the the interned String.
 * */
struct String * put(struct Table * table, const char * str ){
    unsigned long full_hash = hash_gen(str);
    int index = hash_validate(full_hash);
    struct String *str_p = table->table+index;
    int16_t str_len = strlen(str);
    
    str_p = lookup(table,index,str,str_len,full_hash);

    if(str_p!=NULL){
        return str_p;
    }else{// we need to insert str in the end of this bucket
        return str_p;
    }
    return str_p;
    

}

/**
 * Return the number of entries on the table(number of active keys)
 * */
int number_of_entries(struct Table * table);

/**
 * Must receive the first String of a bucket in table->table.
 * If node is NULL, return NULL.
 * Checks wheter str exists or not on the bucket, if it does, then returns the previous String on the bucket,
 * else, returns the last node found on the bucket.
 * */
struct String * lookup_and_get_previous(struct String * string, const char * str, unsigned long full_hash);

struct String * add_entry(struct String * previous, struct String * );

/**
 * Must receive the first String of a bucket in table->table.
 * Checks wheter a String exists or not on a bucket, if it does, then returns the its pointer,
 * else, NULL.
 * */
struct String *lookup(struct Table *table, int index, const char *name, int name_len, unsigned long full_hash)
{
    struct String *string = table->table + index;
    while (string->next)
    {
        if (equals_char(string, name))
        {
            inc_ref_count(string);
            return string;
        }
        string = string->next;
    };
    return NULL;
}


/**
 * Return the table size(number of buckets)
 * */
int table_size(struct Table * table);

/**
 *  Prints address of buckets from 0 to tableSize, but it doesnt loop through Buckets
 * */
void debug_table(struct Table * table );

/**
 *  Check to see if the hashtable is unbalanced. If this bucket is <rehash_multiple> times greater than the
 *  expected average bucket length, it's an unbalanced hashtable.
 * */
bool needs_rehashing(int count);

/**
 * Create a new table and using alternate hash code, populate the new table
 * with the existing strings.   Set flag to use the alternate hash code afterwards.
 * */
void rehash_table();

void inc_num_of_entries(struct Table * table){
    table->number_of_entries++;
}

void dec_num_of_entries(struct Table * table){
    table->number_of_entries--;
}