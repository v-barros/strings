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
    table->rehash_multiple = REHASH_MULTIPLE;
    table->rehash_count = REHASH_COUNT;
    table->table_size = TABLE_SIZE;
    return table;
}

unsigned long seed();

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

struct String *add_from_char_array(struct Table *table, const char *str, unsigned short str_len)
{
    unsigned long full_hash = hash_gen(str);
    int index = hash_validate(full_hash);
    struct String *new_string;

    if (is_empty_bucket(table, index))
    {
        new_string = create_string(str, str_len, full_hash);
        return put_at_empty_bucket(table, index, new_string);
    }
    return lookup(table, index, str, str_len, full_hash);
}

struct String *add_from_string_obj(struct Table *table, struct String *string)
{
    unsigned long full_hash = get_hash(string);
    int index = hash_validate(full_hash);
    struct String *new_string;

    if (is_empty_bucket(table, index))
    {
        new_string = create_string(get_text(string), length(string), full_hash);
        return put_at_empty_bucket(table, index, new_string);
    }
    return lookup(table, index, get_text(string), length(string), full_hash);
}

struct String *lookup(struct Table *table, int index, const char *name, unsigned short name_len, unsigned long full_hash)
{
    struct String *string = *(table->table + index);
    struct String *previous = string;
    int count;
    do
    {
        if (equals_char_l(string, name, name_len))
        {
            inc_ref_count(string);
            return string;
        }
        previous = string;
        string = string->next;
        count++;
    } while (string);
    /*reached end of bucket*/

    struct String *new_string = create_string(name, name_len, full_hash);
    inc_ref_count(new_string);
    set_interned(new_string);
    set_next(previous,new_string);
    inc_num_of_entries(table);

    if(needs_rehashing(table))
        rehash_table(table);
    if(count>=table->rehash_count&&!needs_rehashing(table))
        table->needs_rehashing = check_rehash_table(table,count);
    return new_string;
}

struct String *put_at_empty_bucket(struct Table *table, int index, struct String *string)
{
    inc_ref_count(string);
    *(table->table + index) = string;
    inc_num_of_entries(table);
    set_interned(string);
    return string;
}

void delete_entry(struct Table * table, struct String* stringToDelete)
{   
    struct String * stringCheck;

    int index = hash_validate(get_hash(stringToDelete));

	/**
	 * 	2 possibilities here, ->
	 *  1: str is at the first node of the bucket(table->table[i]);
	 *  2: str is anywhere but on the first node(stringCheck->next);
	 * */ 

    if(is_at(table,index,stringToDelete))
    {
        *(table->table+index) = stringToDelete->next; 
    }
    else
    {
        stringCheck = find_previous(table,index,get_text(stringToDelete),length(stringToDelete));
        set_next(stringCheck,get_next(stringToDelete));
    }
    dec_num_of_entries(table);
    delete_str(stringToDelete);
}

void set_next(struct String * string,struct String * next)
{
    string->next=next;
}

struct String * get_next(struct String * string)
{
    return string->next;
}

bool is_at(struct Table * table, int index, struct String * string)
{
    return *(table->table+index)==string;
}

struct String * find_previous(struct Table *table, int index, const char *name, unsigned short name_len)
{
    struct String *string = *(table->table + index);
    struct String *previous = string;
    
    do
    {
        if (equals_char_l(string, name, name_len))
        {
            return previous;
        }
        previous = string;
        string = string->next;
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
    printf("\n");
    while (i < table->table_size)
    {
        printf("0x%p ", *(table->table + i++));
    }
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

void set_interned(struct String *string)
{
    string->is_shared = true;
}

bool is_shared(struct String *string)
{
    return string->is_shared;
}

bool needs_rehashing(struct Table * table){
    return table->needs_rehashing;
}

bool check_rehash_table(struct Table * table, int count){
  assert(table_size(table) != 0);
  if (count > ((number_of_entries(table)/table_size(table))*table->rehash_multiple))
  {
    // Set a flag for the next safepoint, which should be at some guaranteed
    // safepoint interval.
    return true;
  }
  return false;
}

void rehash_table(struct Table * table)
{

  struct Table * newtable = new_table();

  move_to(table,newtable);

  // Delete the table and buckets (entries are reused in new table).
  //delete _the_table;
  // Don't check if we need rehashing until the table gets unbalanced again.
  // Then rehash with a new global seed.
  //_needs_rehashing = false;
  //_the_table = new_table;
}

void move_to(struct Table * table, struct Table * newtable)
{
// Initialize the global seed for hashing.
  _seed = compute_seed();
  assert(seed() != 0);
  int i;
  int saved_entry_count = number_of_entries(table);
  struct String * p, *next;
  // Iterate through the table and create a new entry for the new table
  for (i = 0; i < table_size(new_table); ++i) {
    for (p = bucket(table,i); p != NULL; ) {
      next = get_next(p);
      
      // Use alternate hashing algorithm on the symbol in the first table
      unsigned int hashValue = new_hash(p,seed());
      // Get a new index relative to the new table (can also change size)
      int index = hash_validate(hashValue);
     
      set_hash(p,hashValue);
      // Keep the shared bit in the Hashtable entry to indicate that this entry
      // can't be deleted.   The shared bit is the LSB in the _next field so
      // walking the hashtable past these entries requires
      // BasicHashtableEntry::make_ptr() call.
      bool keep_shared = is_shared(p);
      unlink_entry(table,p);
      add_from_string_obj(newtable,p);
      if (keep_shared) {
        set_shared(p); //don't need this if lookup is setting the entry to interned
      }
      p = next;
    }
  }
  // give the new table the free list as well
  //new_table->copy_freelist(this);
  //assert(new_table->number_of_entries() == saved_entry_count, "lost entry on dictionary copy?");

  // Destroy memory used by the buckets in the hashtable.  The memory
  // for the elements has been used in a new table and is not
  // destroyed.  The memory reuse will benefit resizing the SystemDictionary
  // to avoid a memory allocation spike at safepoint.
  //BasicHashtable<F>::free_buckets();

}

struct String * bucket(struct Table * table, int index)
{
    return *(table->table+index);
}
bool use_alt_hashing(){
    return _seed!=0;
}

void unlink_entry(struct Table * table, struct String * string)
{
    string->is_shared=0;
    dec_num_of_entries(table);
}