/*
 * string.h
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 */
#ifndef STRING_H_
#define STRING_H_
#include <stdint.h>
#include <stdbool.h>
#include "stringtable.h"

struct String{
	uint32_t ref_count;
	unsigned long hash;
	const char * text;
	struct String * next;
};

extern const void * String;

struct String * literal(struct Table * table,const char * str);

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

#endif /*STRING_H_*/