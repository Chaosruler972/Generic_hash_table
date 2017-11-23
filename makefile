main: GenericHashTable.o
	gcc -Wall -Wvla -g GenericHashTable.o -o main
GenericHashTable.o: GenericHashTable.c
	gcc -Wall -Wvla -g GenericHashTable.c -c
clear: 
	rm main GenericHashTable.o
