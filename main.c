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
 * main.c
 *
 *  Created on: 2021-05-29
 *      Author: @v-barros
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringtable.h"
#include "string.h"

//struct String * literal(struct Table * table,const char * str);


int main(void){
	struct Table * table = new_table();
	struct String * string = literal(table,"myString");
	
	//printf("\ntext: %s reference count: %d address: 0x%p \n",get_text(string),ref_count(string),string);
	
	struct String * string2 = literal(table,"myString");
	printf("\ntext: %s reference count: %d address: 0x%p number of entries: %d\n",get_text(string2),ref_count(string2),string2,number_of_entries(table));
	struct String * string3 = literal(table,"aaa");
	struct String * string4 = literal(table,"str");
	struct String * string5 = literal(table,"strin");

	
	printf("\ntext: %s reference count: %d address: 0x%p number of entries: %d\n",get_text(string3),ref_count(string3),string3,number_of_entries(table));
	
	debug_table(table);
	return 1;

}