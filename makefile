main: main.o stringtable.o string.o memory.o
	gcc -o main main.o stringtable.o string.o memory.o
main.o: main.c  stringtable.h string.h
	gcc -c main.c
string.o: string.c string.h stringtable.h memory.h
	gcc -c string.c
stringtable.o: stringtable.c string.h stringtable.h memory.h
	gcc -c stringtable.c
memory.o: memory.c memory.h
	gcc -c memory.c