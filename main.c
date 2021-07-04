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
#include <stdlib.h>
#define str_gen(x) for(i=0;i<x;i++){ \
		for(j=0;j<23;j++){ \
			str[j]= to_ascii(j*i+1);\
		} \
		str[j]='\0';\
		LITERAL(str); \
	}\

int to_ascii(int);

int main(int argc, char **argv)
{	

	INIT_TABLE;
	char str[24];
	int i,j,n;
	n = (int)strtol(argv[1], NULL, 10);
	STRING string = LITERAL("myString"); 	 /*String str = "";*/
	STRING string2 = LITERAL("myStr"); 	 /*String str = "";*/
	STRING string3 = LITERAL("String"); 	 /*String str = "";*/
	STRING string4 = LITERAL("my"); 	 /*String str = "";*/
	STRING string5 = LITERAL("mySt"); 	 /*String str = "";*/
	//STRING string1 = NEW_STRING("myString1");/*String str = new String("");*/
	str_gen(n)
	
	

	//printf("\ntext: %s reference count: %d address: %p number of entries: %d\n",get_text(string),ref_count(string1),string1,number_of_entries(table));

//	SET(string)(string1);					 /*string = string1*/
	
	//STRING string2 = LITERAL("myString");

	//printf("\ntext: %s reference count: %d address: %p number of entries: %d\n",get_text(string2),ref_count(string2),string2,number_of_entries(table)); // new address, different than previous String string address,


	//SET(string2)(string1);	
	//printf("\ntext: %s reference count: %d address: %p number of entries: %d\n",get_text(string1),ref_count(string1),string1,number_of_entries(table));

	DEBUG_TABLE;
	return 1;
}

int to_ascii(int n){
	return (n%0x7E) > 0x5E ? n%0x7E : n%0x7E + 0x20;
}