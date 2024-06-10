all: main
 
main: main.o
	gcc -static -o main main.o
 
main.o:
	nasm -f elf64 -o main.o main.asm
 
clear:
	rm main main.o