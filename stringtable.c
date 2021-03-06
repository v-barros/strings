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
 * hashtable.c
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros 
 */
#include "stringtable.h"
#include "string.h"
#include "hashing.h"


static unsigned long _seed;
static const size_t _Table = sizeof(struct Table);
const void *Table = &_Table;

struct Table *new_table()
{
    struct Table *table = alloc(Table);
    size_t tableBytes = TABLE_SIZE * (sizeof(struct sstring *));
    table->table = calloc(1,tableBytes);
    assert(table->table);
    table->rehash_multiple = REHASH_MULTIPLE;
    table->rehash_count = REHASH_COUNT;
    table->table_size = TABLE_SIZE;
    return table;
}

unsigned long seed(){
    return _seed;
}

/**
 * Bucket verification
 * */
int hash_validate(unsigned long full_hash);

int hash_validate(unsigned long full_hash)
{
    int h = full_hash % TABLE_SIZE;
    assert((h >= 0 && h < TABLE_SIZE));
    return h;
}

struct sstring * make_ptr(struct sstring * next);

struct sstring * make_ptr(struct sstring * next){
    return (struct sstring*) ((uintptr_t) next & -2);
}

struct sstring *add_from_char_array(struct Table *table, const char *str, unsigned short str_len)
{
    unsigned long full_hash = hash_string(str,str_len);
    int index = hash_validate(full_hash);
    struct sstring *new_string;
    
    if (is_empty_bucket(table, index))
    {
        new_string = create_string(str, str_len, full_hash);
        basic_add(table,new_string,index);
        set_shared(new_string);
        return new_string; 
    }
    return lookup(table, index, str, str_len, full_hash);
}

struct sstring *add_from_string_obj(struct Table *table, struct sstring *string)
{
    unsigned long full_hash = use_alt_hashing ? new_hash(string,seed()) :  get_hash(string);
    int index = hash_validate(full_hash);
    struct sstring *new_string;

    if(is_empty_bucket(table, index))
    {
        new_string = create_string(get_text(string), length(string), full_hash);
        basic_add(table,new_string,index);
        set_shared(new_string);
        return new_string; 
    }
    return lookup(table, index, get_text(string), length(string), full_hash);
}

struct sstring *lookup(struct Table *table, int index, const char *name, unsigned short name_len, unsigned long full_hash)
{
    struct sstring *string = *(table->table + index);
    int count=0;
    do
    {
        if (equals_char_l(string, name, name_len))
        {
            inc_ref_count(string);
            return string;
        }
        string = get_next(string);
        count++;
    } while (string);
    /*reached end of bucket*/

    struct sstring *new_string = create_string(name, name_len, full_hash);
    set_shared(new_string);
    basic_add(table,new_string,index);

    if(needs_rehashing(table)){
       //printf("needs_rehashing\n\n");
        rehash_table(table);
        
    }
    
    if(count>=table->rehash_count&&!needs_rehashing(table)){
       // printf("count >= rehash_count count %d at index %d num of entries %d\n\n",count,index,number_of_entries(table));
        table->needs_rehashing = check_rehash_table(table,count);
        //printf("needs rehashing? %d\n",needs_rehashing(table));
    }
        
    return new_string;
}

void delete_entry(struct Table * table, struct sstring* stringToDelete)
{   
    struct sstring * stringCheck;

    int index = hash_validate(get_hash(stringToDelete));

	/**
	 * 	2 possibilities here, ->
	 *  1: str is at the first node of the bucket(table->table[i]);
	 *  2: str is anywhere but on the first node(stringCheck->next);
	 * */ 

    if(is_at(table,index,stringToDelete))
    {
        *(table->table+index) = get_next(stringToDelete);
    }
    else
    {
        stringCheck = find_previous(table,index,get_text(stringToDelete),length(stringToDelete));
        set_next(stringCheck,get_next(stringToDelete));
    }
    dec_num_of_entries(table);
    delete_str(stringToDelete);
}

void set_next(struct sstring * string,struct sstring * next)
{
    string->next=next;
}

struct sstring * get_next(struct sstring * string)
{
    return make_ptr(string->next);
}

bool is_at(struct Table * table, int index, struct sstring * string)
{
    return *(table->table+index)==string;
}

struct sstring * find_previous(struct Table *table, int index, const char *name, unsigned short name_len)
{
    struct sstring *string = *(table->table + index);
    struct sstring *previous = string;
    
    do
    {
        if (equals_char_l(string, name, name_len))
        {
            return previous;
        }
        previous = string;
        string = get_next(string);
    } while (string);
    /*reached end of bucket*/

    return previous;
}

bool is_empty_bucket(struct Table *table, int index)
{
    return (!(*(table->table + index)));
}

int table_size(struct Table *table)
{
    return table->table_size;
}

void debug_table(struct Table *table)
{
    int i = 0;
    struct sstring * string;

    printf("\n\t\tDEBUG TABLE\nneeds rehashing: %d\nnumber of entries: %d\nrehash count: %d\nrehash multiple: %d\ntable size: %d\n\n",
    table->needs_rehashing,table->number_of_entries,table->rehash_count,table->rehash_multiple,table->table_size);
    
    
    for (i = 0; i < table_size(table); ++i) {
        printf("%05d: {",i);
        for (string = bucket(table,i); string != NULL; ) {
            printf("\"%s\" , ",get_text(string));
            string=get_next(string);
        }
        printf("%p}\n",string);
    }/*
    while (i < table->table_size)
    {
        printf("0x%p ", *(table->table + i++));
    }*/
    printf("\n");
}

void inc_num_of_entries(struct Table *table)
{
    table->number_of_entries++;
}

void dec_num_of_entries(struct Table *table)
{
    table->number_of_entries--;
}

int number_of_entries(struct Table *table)
{
    return table->number_of_entries;
}

void set_shared(struct sstring *string)
{
    string->next = (struct sstring *) ((uintptr_t) string->next | 1);
}

bool is_shared(struct sstring *string)
{
    return ((uintptr_t)string->next & 1) !=0;
}

bool needs_rehashing(struct Table * table){
    return table->needs_rehashing;
}

bool check_rehash_table(struct Table * table, int count){
  assert(table_size(table) != 0);
  if (count > ((number_of_entries(table)/table_size(table))*table->rehash_multiple))
  {
    return true;
  }
  return false;
}

void rehash_table(struct Table *table)
{
    struct Table *newtable = new_table();
    printf("\t\tbefore rehashing\n");
    debug_table(table);  
    move_to(table, newtable);
    
    delete(table->table);
    table->table = newtable->table;

    table->needs_rehashing = false;
    table->number_of_entries = number_of_entries(newtable);

    delete(newtable);
    printf("\t\tafter rehashing\n");
    debug_table(table);
}

void move_to(struct Table * table, struct Table * newtable)
{
// Initialize the global seed for hashing.
  _seed = compute_seed();
  assert(seed() != 0);
  int i;
  int saved_entry_count = number_of_entries(table);
  struct sstring * p, *next;
  // Iterate through the table and create a new entry for the new table

  printf("saved entry count: %d\n", saved_entry_count);
  for (i = 0; i < table_size(newtable); ++i) {
    for (p = bucket(table,i); p != NULL; ) {
        next = get_next(p);
      
        // Use alternate hashing algorithm on the symbol in the first table
        unsigned long hashValue = new_hash(p,seed());
        
        // Get a new index relative to the new table (can also change size)
        int index = hash_validate(hashValue);
        
        set_hash(p,hashValue);
        // Keep the shared bit in the Hashtable entry to indicate that this entry
        // can't be deleted.   The shared bit is the LSB in the _next field so
        // walking the hashtable past these entries requires
        // BasicHashtableEntry::make_ptr() call.
        bool keep_shared = is_shared(p);
        unlink_entry(table,p);
        basic_add(newtable,p,index);
        if (keep_shared){
            set_shared(p);
        }

        p = next;
    }
  }
   assert(number_of_entries(newtable) == saved_entry_count);
}

struct sstring * bucket(struct Table * table, int index)
{
    return *(table->table+index);
}

bool use_alt_hashing()
{
    return _seed!=0;
}

void unlink_entry(struct Table * table, struct sstring * string)
{
    string->next=NULL;
    dec_num_of_entries(table);
}

void basic_add(struct Table * table, struct sstring * string, int index)
{
    set_next(string,*(table->table+index));
    *(table->table+index) = string;
    inc_num_of_entries(table);
}