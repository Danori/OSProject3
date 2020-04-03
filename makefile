all: rwmain

rwmain:
	gcc -o rwmain main.c readerwriter.c -pthread
