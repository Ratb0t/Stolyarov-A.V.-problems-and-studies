run.exe : main.c
	gcc -g -Wall -DA main.c -o run.exe

A : main.c
	gcc -g -Wall -D$@ main.c -o run.exe

B : main.c
	gcc -g -Wall -D$@ main.c -o run.exe

C : main.c
	gcc -g -Wall -D$@ main.c -o run.exe


run_a : A
	./run.exe cat main.c ";;" cat > out.testdata

run_b : B
	./run.exe cat in.testdata ";;" cat > out.testdata

run_c : C
	./run.exe ls -la ";;" cat > out.testdata