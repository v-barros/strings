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
#include "string.h"
#include "stringtable.h"
#include <stdio.h>


int main(void){
	struct Table * table = new_table();

	struct String * string = literal(table,"myString");
	struct String * string1 = new_string("myString");
	
	printf("%d %d\n", (int) ((void *)string ==(void *) string1), equals_str(string, string1));

	string1 = intern(table,string1);
	
	printf("%d %d\n", (int) ((void *)string ==(void *) string1), equals_str(string, string1));

	//printf("\ntext: %s reference count: %d address: 0x%p number of entries: %d\n",get_text(string3),ref_count(string3),string3,number_of_entries(table));

	return 1;
}