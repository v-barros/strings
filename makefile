main: main.o stringtable.o string.o memory.o
	gcc -o main main.o stringtable.o string.o memory.o
main.o: main.c  stringtable.h string.h
	gcc -c main.c
stringtable.o: stringtable.c stringtable.h
	gcc -c stringtable.c
string.o: string.c string.h
	gcc -c string.c
memory.o: memory.c memory.h
	gcc -c memory.c