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


int to_ascii(int);

void string_from_file(char file_name[25],char *str, int line);

/* ./main <file containing strings to insert on the table> <number of strings to read from file and create> */
int main(int argc, char **argv)
{	
	assert(argc==3);
	
	char *str = malloc(sizeof(char)*33);
	int i;
	unsigned int strings_to_read;
	strings_to_read = (int)strtol(argv[2], NULL, 10);
	init_table;
	
	

	for(i=0;i<strings_to_read;i++){
		string_from_file(argv[1],str,i);
		literal(str);
	}
	dump_table;
	
	return 1;
}

int to_ascii(int n){
	return (n%0x7E) > 0x5E ? n%0x7E : n%0x7E + 0x20;
}

void string_from_file(char file_name[25],char *str,int line)
{
	char _str[31];
	FILE *fp;
	int i=-1,ln=0;

	fp = fopen(file_name, "r"); // read mode

	if (fp == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}
	while (fscanf(fp,"%[^\n]\n",_str) !=EOF && (++i<line));
	assert(i==line);
	strcpy (str,_str);
	fclose(fp);
	
}