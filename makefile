main: main.o stringtable.o string.o memory.o hashing.o
	gcc -o main main.o stringtable.o string.o memory.o hashing.o
main.o: main.c  stringtable.h string.h hashing.h
	gcc -c main.c
string.o: string.c string.h stringtable.h memory.h hashing.h
	gcc -c string.c
stringtable.o: stringtable.c string.h stringtable.h memory.h hashing.h
	gcc -c stringtable.c
memory.o: memory.c memory.h
	gcc -c memory.c
hashing.o: hashing.c hashing.h
	gcc -c hashing.c