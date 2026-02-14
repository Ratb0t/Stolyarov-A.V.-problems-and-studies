run.exe : main.o support_func.o
	ld main.o support_func.o -o run.exe
main.o : main.c
	gcc -g -Wall -c main.c
support_func.o : support_func.asm
	nasm -f elf64 -g -F dwarf support_func.asm