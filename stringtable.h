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
 *stringtable.h
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 */

#ifndef STRINGTABLE_H_
#define STRINGTABLE_H_
#define TABLE_SIZE 50
#define REHASH_MULTIPLE 2
#define REHASH_COUNT 4
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "memory.h"


struct Table{
	int table_size;
	int number_of_entries;
	int rehash_multiple;
	int rehash_count;
	struct sstring ** table;
	bool needs_rehashing;
};
extern const void * Table;

/**
 *  Alloc a new table struct with default size defined by TABLE_SIZE macro and return its pointer
 * */
struct Table * new_table();

/**
 *  In this case, this it not the same as in hotspot, in hotspot, 
 *  the entries are kept in a free list, so they can be reused in order
 *  to reduce object creation.
 * 	Here, we receive a shared string, remove it from the table and then the memory is freed
 * */
void delete_entry(struct Table * table, struct sstring* string);

/**
 * Given a char array and it's length, try to insert a new structure on the table
 * If the String already exists on the table, increases the ref_count and returns 
 * a pointer to the interned String.
 * If it doesnt, insert the string on the table and returns a pointer to the the interned String.
 * */
struct sstring * add_from_char_array(struct Table * table, const char * str,unsigned short str_len );

/**
 * Given a String, try to insert a new structure on the table
 * If the String already exists on the table, increases the ref_count and returns 
 * a pointer to the interned String.
 * If it doesnt, insert the string on the table and returns a pointer to the the interned String.
 * */
struct sstring * add_from_string_obj(struct Table * table, struct sstring * string);
/**
 * Return the number of entries on the table(number of active keys)
 * */
int number_of_entries(struct Table * table);

/**
 * Must receive the first String of a bucket in table->table.
 * Checks wheter a String exists or not on a bucket, if it does, then returns its pointer,
 * else, NULL.
 * */
struct sstring *lookup(struct Table *table, int index, const char *name, unsigned short name_len, unsigned long full_hash);

/**
 * Return the table size(number of buckets)
 * */
int table_size(struct Table * table);

/**
 * Must receive the index of a bucket and the content of a String to be searched.
 * 
 * Checks wheter str exists or not on the bucket, if it does, then returns the previous String on the bucket,
 * else, returns the last node found on the bucket.
 * */
struct sstring * find_previous(struct Table *table, int index, const char *name, unsigned short name_len);

/**
 *  Prints address of buckets from 0 to tableSize, but it doesnt loop through Buckets
 * 
 * 0 - nil
 * 1 - mystr - nil
 * 2 - strrr - abc - chair - nil
 * 3 - name - myname - nil
 * 4 - aaaa - bbbb - cccc - nil
 * 5 - nil
 * 
 * */
void debug_table(struct Table * table );

/**
 * return table->needs_rehashing
 * */
bool needs_rehashing(struct Table * table);

/**
 *  Check to see if the hashtable is unbalanced. If this bucket is <rehash_multiple> times greater than the
 *  expected average bucket length, it's an unbalanced hashtable.
 *  This is somewhat an arbitrary heuristic but if one bucket gets to
 *  rehash_count which is currently 100, there's probably something wrong.
 * */
bool check_rehash_table(struct Table * table, int count);

/**
 * Create a new table and using alternate hash code, populate the new table
 * with the existing strings.   Set flag to use the alternate hash code afterwards.
 * */
void rehash_table(struct Table * table);

void set_shared(struct sstring * string );

bool is_shared(struct sstring * string);

bool is_empty_bucket(struct Table * table, int index);

void inc_num_of_entries(struct Table * table);

void dec_num_of_entries(struct Table * table);

void set_next(struct sstring * string,struct sstring * next);

struct sstring * get_next(struct sstring * string);

/**
 * Check if the string is on the first position of the bucket 'index'.
 * */
bool is_at(struct Table * table, int index, struct sstring * string);

/**
 * Create a new table and using alternate hash code, populate the new table
 * with the existing elements.   This can be used to change the hash code
 * and could in the future change the size of the table.
 * */
void move_to(struct Table *table, struct Table* newtable);

struct sstring * bucket(struct Table * table, int index);

bool use_alt_hashing();

void unlink_entry(struct Table * table, struct sstring * string);

void basic_add(struct Table * table, struct sstring * string, int index);

unsigned long seed();

#endif /* STRINGTABLE_H_ */
