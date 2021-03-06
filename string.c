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
 * string.c
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 */

#include "string.h"
#include "hashing.h"

static const size_t String_size = sizeof(struct sstring);
const void * _String = &String_size;

struct sstring * _literal(struct Table *table,const char * str)
{
    unsigned long str_len;
    assert(str && table);
    str_len = strlen(str);
    assert(is_ascii(str,str_len));
    assert(str_len<max_text_length);
    
    return add_from_char_array(table, str,str_len);    
}

struct sstring * _new_string(const char * str)
{
    unsigned long str_len;
    assert(str);
    str_len = strlen(str);
    assert(is_ascii(str,str_len));
    assert(str_len<max_text_length);

    struct sstring *new_string = create_string(str, str_len, hash_string(str,str_len));
        
    return new_string;
}

struct sstring *_intern(struct Table *table, struct sstring *string)
{
    assert(string && table);
    if (!is_shared(string))
    {
        string = add_from_string_obj(table, string);
    }
    return string;
}

bool is_ascii(const char *str, int len)
{
    int i = 0;
    for (; i < len; i++)
        if (!(str[i] >= 0x20 && str[i] <= 0x7E))
            return false;
    return true;
}

void set_text(struct sstring *string, const char *text, unsigned short text_len)
{
    assert(!string->text); /*immutable, right?*/
    string->text = text;
    string->length = text_len;
}

const char *get_text(struct sstring *string)
{
    assert(string);
    return string->text;
}

unsigned short length(struct sstring *string)
{
    assert(string);
    return string->length;
}

bool equals_str(struct sstring *a, struct sstring *b)
{
    if (a == b)
        return true;

    return equals(get_text(a), length(a),
                  get_text(b), length(b));
}

bool equals_char(struct sstring *a, const char *b)
{
    assert(a);
    assert(b);
    return equals(get_text(a), length(a),
                  b, strlen(b));
}

bool equals_char_l(struct sstring *a, const char *b, unsigned short len_b)
{
    return equals(get_text(a), length(a),
                  b, len_b);
}

bool equals(const char *a, unsigned short len_a,
            const char *b, unsigned short len_b)
{

    if (len_a != len_b)
        return false;
    int i = len_a;
    while (i >= 0)
    {
        if (a[i] != b[i])
            return false;
        i--;
    }
    return true;
}

struct sstring *create_string(const char *str, unsigned short str_len, unsigned long hash)
{
    struct sstring *string = alloc(_String);
    char *text = (char *)malloc(str_len + 1);
    strcpy(text, str);
    set_text(string, text, str_len);
    set_hash(string, hash);
    inc_ref_count(string);
    return string;
}

struct sstring * copy(struct sstring *src)
{
    assert(src);
    inc_ref_count(src);
    return src;
}

int ref_count(struct sstring *string)
{
    return string->ref_count;
}

void set_hash(struct sstring *string, unsigned long hash)
{
    string->hash = hash;
}

unsigned long get_hash(struct sstring *string)
{
    return string->hash;
}

void dec_ref_count(struct sstring *string)
{
    assert(string->ref_count - 1 >= 0);
    string->ref_count--;
}

void inc_ref_count(struct sstring *string)
{
    string->ref_count++;
}

void drop(struct Table * table,struct sstring * string)
{
    assert(string);
    if(!is_shared(string))
        delete_not_shared(string);
    else
        delete_shared(table,string);
}

void delete_shared(struct Table * table, struct sstring * string)
{
    if(ref_count(string)>1)
        dec_ref_count(string);
    else
        delete_entry(table,string);
}

void delete_not_shared(struct sstring *string)
{
    if (ref_count(string) > 1)
        dec_ref_count(string);
    else    
        delete_str(string);   
}

void delete_str(struct sstring * string)
{
    delete ((void *)string->text);
    delete (string);
}

unsigned long new_hash(struct sstring * string, unsigned long seed)
{
    return alt_hash(seed,get_text(string),length(string));
}

unsigned long hash_string(const char * str, int len)
{
    return use_alt_hashing() ? alt_hash(seed(),str,len) : hash_gen(str);
}