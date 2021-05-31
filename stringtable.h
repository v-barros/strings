/*
 * stringtable.h
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 */

#ifndef STRINGTABLE_H_
#define STRINGTABLE_H_
#define TABLE_SIZE 10
#include <stdint.h>
#include <stdbool.h>
#include "string.h"



struct Table{
	uint32_t tableSize;
	uint32_t numberOfEntries;
	const struct String * table[TABLE_SIZE];
};

/**
 *  Alloc a new table struct with default size defined by TABLE_SIZE macro and return its pointer
 * */
struct Table * createTable();

/**
 * Lookup for a key on the given table
 * If the key is present, returns its value, else, returns an empty string ""
 * */
const char * getValue(struct Table * table,const char * key);

/**
 * Given a key and a value, try to insert a new structure on the table
 * If the key already exists, places the new value on the present structure 
 * If it doesnt, creates a new structure to insert key and value
 * Returns the (updated) value passed to the key
 * */
const char * put(struct Table * table, const void * item);

/**
 * Remove a key on the table
 * return 1 if removed sucessfuly, else, return 0 
 * */
bool removeByKey(struct Table * table,const char * key);

/**
 * Return the number of entries on the table(number of active keys)
 * */
int numberOfEntries(struct Table * table);

/**
 * Return the table size(number of buckets)
 * */
int tableSize(struct Table * table);

/**
 *  Prints address of buckets from 0 to tableSize, but it doesnt loop through Buckets
 * */
void debugTable(struct Table * table );

void rehash(struct Table *  table);

#endif /* STRINGTABLE_H_ */
