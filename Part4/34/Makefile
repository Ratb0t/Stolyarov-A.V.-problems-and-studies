run.exe : main.o support_functions.o
	ld main.o support_functions.o -o run.exe
main.o : main.c
	gcc -g -Wall -c main.c
support_functions.o : support_functions.asm
	nasm -f elf64 -g -F dwarf support_functions.asm