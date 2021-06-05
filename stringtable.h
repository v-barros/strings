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
#define TABLE_SIZE 3
#define REHASH_MULTIPLE 60
#define REHASH_COUNT 100
#include <stdint.h>
#include <stdbool.h>
#include "string.h"

struct Table{
	int table_size;
	int number_of_entries;
	int rehash_multiple;
	int rehash_count;
	const struct String * table[TABLE_SIZE];
};
extern const void * Table;

/**
 *  Alloc a new table struct with default size defined by TABLE_SIZE macro and return its pointer
 * */
struct Table * new_table();

/**
 * Given a String, try to insert a new structure on the table
 * If the String already exists on the table, increases the ref_count and returns 
 * a pointer to the interned String.
 * If it doesnt, insert the string on the table and returns a pointer to the the interned String.
 * */
struct String * basic_add(struct Table * table, const char * str,unsigned short str_len );
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


/**
 * Must receive the first String of a bucket in table->table.
 * Checks wheter a String exists or not on a bucket, if it does, then returns its pointer,
 * else, NULL.
 * */
struct String *lookup(struct Table *table, int index, const char *name, unsigned short name_len, unsigned long full_hash);
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
 *  This is somewhat an arbitrary heuristic but if one bucket gets to
 *  rehash_count which is currently 100, there's probably something wrong.
 * */
bool needs_rehashing(int count);

/**
 * Create a new table and using alternate hash code, populate the new table
 * with the existing strings.   Set flag to use the alternate hash code afterwards.
 * */
void rehash_table();

bool is_interned(struct String * string);

bool is_empty_bucket(struct Table * table, int index);

void inc_num_of_entries(struct Table * table);
#endif /* STRINGTABLE_H_ */
