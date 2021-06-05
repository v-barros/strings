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
#include "stringtable.h"
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
struct String * literal(struct Table * table,const char * str){
    assert(str);
    assert(is_ascii(str,strlen(str)));
    assert(strlen(str)<max_text_length);
    struct String * string;

    string = put(string,str);
    return string;
}

bool is_ascii(const char * str, int len){
    int i =0;
    for(;i<len;i++)
        if(!(str[i]>=0x20 && str[i]<=0x7E))
            return false;
    return true;
}

void * setText(struct String * string, const char * text){
    assert(text);
    string->text=text;
    
}

void dec_ref_count(struct String * string){
    assert(string->ref_count-1>=0);
    string->ref_count--;    
}

void inc_ref_count(struct String * string){
    string->ref_count++;
}

struct String * new_string(const char * str);

struct String * intern(struct String * string);

char * text(struct String * string){
    assert(string);
    return string->text;
}

void delete_(struct String * string);

bool equals_str(struct String * a,struct String * b);

unsigned short length(struct String * string){
    assert(string);
    return string->length;
}

bool equals_char(struct String * a, const char * b){
    assert(a);
    assert(b);
    return equals(text(a),length(a),b,strlen(b));
}

bool equals(const char * a, int len_a, const char * b, int len_b){
    if(len_a!=len_b)
        return false;
    int i;
    while(i>0){
        if(a[i]!=b[i])
            return false;
        i++;
    }
    return true;
}

void copy(struct String *dest, struct String * src);

int ref_count(struct String * string);