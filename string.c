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
#include <string.h>
#include "string.h"
#include "memory.h"

static const size_t _String = sizeof(struct String);
const void * String = &_String;

/* DJB2. See: https://theartincode.stanis.me/008-djb2/ */
unsigned long hash_gen(const char * str);

unsigned long hash_code(const char * str){
    unsigned long hash = 5381;
	int8_t c;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}


/**
 * String name = "foo"; 
 * */
struct String * literal(struct Table *table,const char * str){
    unsigned long str_len;
    assert(str && table);
    str_len = strlen(str);
    assert(is_ascii(str,str_len));
    assert(str_len<max_text_length);
    
    return basic_add(table, str,str_len);    
}

bool is_ascii(const char * str, int len){
    int i =0;
    for(;i<len;i++)
        if(!(str[i]>=0x20 && str[i]<=0x7E))
            return false;
    return true;
}

void * set_text(struct String * string, const char * text, unsigned short text_len){
    assert(!string->text);/*immutable, right?*/
    string->text=text;
    string->length = text_len;
}



struct String * new_string(const char * str);

struct String * intern(struct String * string);

const char * get_text(struct String * string){
    assert(string);
    return string->text;
}

void delete_(struct String * string);

bool equals_str(struct String * a,struct String * b){
    if(a==b)
        return true;
    
    return equals(get_text(a),length(a),
                  get_text(b),length(b));
}

unsigned short length(struct String * string){
    assert(string);
    return string->length;
}

bool equals_char(struct String * a, const char * b){
    assert(a);
    assert(b);
    return equals(get_text(a),length(a),
                  b,strlen(b));
}

bool equals_char_l(struct String * a, const char * b, unsigned short len_b){
    return equals(get_text(a),length(a),
                  b,len_b);
}

bool equals(const char * a, unsigned short len_a,
            const char * b, unsigned short len_b){
    
    if(len_a!=len_b)
        return false;
    int i = len_a;
    while(i>=0){
        if(a[i]!=b[i])
            return false;
        i--;
    }
    return true;
}

struct String * create_string(const char * str, unsigned short str_len, unsigned long hash){
    struct String * string = alloc(String);
    char * text = (char * ) malloc (str_len+1);
    strcpy(text,str);
    set_text(string,text,str_len);
    set_hash(string,hash);
    return string;
}

void copy(struct String *dest, struct String * src){
    assert(dest);
    assert(src);
    /**
     * Must check wheter both strings are interned,
     * if dest is interned, we might need to delete this (if ref_count is 1).
     * if src is interned, we only need to increment ref_count;
     **/
}

int ref_count(struct String * string){
    return string->ref_count;
}

void set_hash(struct String * string,unsigned long hash){
    string->hash = hash;
}

unsigned long get_hash(struct String * string){
    return string->hash;
}

void dec_ref_count(struct String * string){
    assert(string->ref_count-1>=0);
    string->ref_count--;    
}

void inc_ref_count(struct String * string){
    string->ref_count++;
}