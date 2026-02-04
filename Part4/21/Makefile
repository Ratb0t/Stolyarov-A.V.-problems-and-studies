
CFLASGS = -DDEBUGg

all : run.exe

run.exe : main.c
	gcc -g -Wall $(CFLASGS) main.c -o run.exe

clean:
	-rm *.exe

rebuild : clean all