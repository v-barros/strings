/*
 *
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
 *string.h
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros 
 */
#ifndef STRING_H_
#define STRING_H_
#define LITERAL(x) literal(table,x)
#define INIT_TABLE struct Table *table = new_table()
#define NEW_STRING(x) new_string(x)
#define STRING struct String *
#define SET(x) drop(table,x);x=TO
#define TO(y) copy(y)
#define INTERN(x) intern(table,x)
#define DEBUG_TABLE debug_table(table)

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "stringtable.h"
#include "memory.h"

struct String{
	const char * text;
	unsigned long hash; 
	struct String * next;
	int ref_count;
	unsigned short length; 
};

/**
 * Struct to represent a String.
 * 
 * For a programmer using Java_lang_string, there are 2 main ops for Strings:
 * 
 * 1) String creation, wich can be done,in Java, by 2 different ways:
 * 
 *	String Object: when using the constructor -> new String(), usually,
 *  the methods returning a String are actually creating a new String
 *  Object, like substring(), concat(),valueOf(), etc; 
 *	String Literal: using double commas -> String str = ""; 
 *  
 *  In general, we can define this as bellow.
 *  String object = Strings calculated in run time
 *  String literal = Strings defined in source code.
 * 
 *  Only String Literals are automatically interned, i.e., the JVM 
 *  performs a Lookup on the String table and if the string we're 
 *  trying to create has already been interned, then the local variable 
 *  will have a reference to this pre-existing string.
 *  Example:
 * 		String a = "a";
 *  	String b = "a";
 * 	Both variables are pointing to the same object, so there is just one
 *  instance of "a".
 * 
 *  String Objects don't get interned automatically, this is, when we 
 *  use the constructor, the JVM creates a new string even if
 *  there is already an equal string on string table.
 *  Example:
 * 		String a = "a";
 * 		String b = new String("a");
 * 	In this case, there are two different instances of String, where
 *  a has a reference to an interned string on string table
 *  b holds a reference to an string located anywhere inside the heap.
 * 
 * 
 * */

static unsigned short max_text_length = (1 << 16)-1;/* 65535 */

extern const void * String;

/**
 * equivalent to String var = "string";
 */
struct String * literal(struct Table *table,const char * str);

struct String * new_string(const char * str);

struct String * intern(struct Table * table,struct String * string);

const char * get_text(struct String * string);

void delete_shared(struct Table * table, struct String * string);

void delete_not_shared(struct String * string);

void delete_str(struct String * string);

bool equals_str(struct String * a,struct String * b);

bool equals_char_l(struct String * a, const char * b, unsigned short len_b);

bool equals(const char * a, unsigned short len_a,
 			const char * b, unsigned short len_b);

struct String * create_string(const char * str, unsigned short str_len, unsigned long hash);

/**
 * True if every char is a printable ascii char. 
 * ie between 0x20 and 0x7E 
 * */
bool is_ascii(const char * str, int len);

/**
 * Used for internal operations only, this function makes string->text point to text variable.
 * This is not an equivalent for:
 * String a = "foo";
 * String b = "bar";
 * a =  b;
 *   ^
 * */
void set_text(struct String * string, const char * text,unsigned short text_len);

/**
 * Used to assign a String to another instance of String, ex:
 * 
 * String name = "foo"
 * String nickName = "bar"
 * name = nickName;
 * 		^ instead of y=x, this would be SET(y)(x)
 * */
struct String * copy(struct String * src);

void drop(struct Table * table,struct String * str);

void dec_ref_count(struct String * string);

void inc_ref_count(struct String * string);

int ref_count(struct String * string);

unsigned short length(struct String * string);

void set_hash(struct String * string, unsigned long hash);

unsigned long get_hash(struct String * string);

// For new table alternate hashing
unsigned long new_hash(struct String * string, unsigned long seed);

/* Pick hash algorithm */
unsigned long hash_string(const char * str, int len);

#endif /*STRING_H_*/