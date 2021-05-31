/*
 * string.h
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 */
#ifndef STRING_H_
#define STRIG_H_
#include <stdint.h>
#include <stdbool.h>

struct String{
	uint32_t ref_count;
	unsigned long hash;
	const char * str;
	struct String * next;
};

struct String * literal(const char * str);

struct String * new_String(const char * str);

struct String * intern(struct String * string);

struct String * intern(const char * str);

void delete_(struct String * string);

bool equals(struct String * a,struct String * b);

#endif /*STRING_H_*/