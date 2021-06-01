/*
 * string.c
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 */
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "string.h"
#include "stringtable.h"
#include "memory.h"

static const size_t _String = sizeof(struct String);
const void * String = &_String;

/* DJB2. See: https://theartincode.stanis.me/008-djb2/ */
unsigned long hash_gen(const char * str);

/**
 * Bucket verification
 * */
uint32_t hash_validate(unsigned long full_hash);


unsigned long hash_gen(const char * str){
    unsigned long hash = 5381;
	int8_t c;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

uint32_t hash_validate(unsigned long full_hash){
    uint32_t h = full_hash % TABLE_SIZE;
	assert(!(h >= 0 && h < TABLE_SIZE));
	return h;
}
/**
 * String name = "foo"; 
 * */
struct String * literal(struct Table * table,const char * str){
    assert(str);
    struct String * new_string = (struct String*) alloc(String);
    unsigned long full_hash = hash_gen(str);
    uint32_t index = hash_validate(full_hash);
    
    struct String * string = *(table->table+index);
    string = lookup_and_get_previous(string,str,full_hash);
    return string;
}
struct String * new_String_Object(const char * str);

struct String * intern(struct String * string);

char * text(struct String * string);

void delete_(struct String * string);

bool equals(struct String * a,struct String * b);


/**
 * Used to assign a String to another instance of String, ex:
 * 
 * String name = "foo"
 * String nickName = "bar"
 * name = nickName;
 * 		^ instead of "=", it would be: copy(name,nickName);
 * If src is a interned String, increase the ref_count;
 * */
void copy(struct String *dest, struct String * src);

void dec_ref_count(struct String * string);

void inc_ref_count(struct String * string);

int ref_count(struct String * string);
