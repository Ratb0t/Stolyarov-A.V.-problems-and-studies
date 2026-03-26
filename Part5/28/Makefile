run.exe : main.c
	gcc -g -Wall -DA main.c -o run.exe

A : main.c
	gcc -g -Wall -D$@ main.c -o run.exe

B : main.c
	gcc -g -Wall -D$@ main.c -o run.exe

C : main.c
	gcc -g -Wall -D$@ main.c -o run.exe

D : main.c
	gcc -g -Wall -D$@ main.c -o run.exe

test_a :
	@./run.exe cat main.c > out.testdata

test_b :
	@grep '^ ' main.c > grp.testdata
	@./run.exe cat main.c > out.testdata
	@diff grp.testdata out.testdata

test_c :
	@@./run.exe ls -la > out.testdata

test_d :
	@./run.exe cat main.c > out.testdata