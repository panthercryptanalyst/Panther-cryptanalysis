all: exec

exec: attack.o gf16.o 
	gcc -o exec attack.o gf16.o

main.o: attack.c attack.h
	gcc -c attack.c

gf16.o: gf16.c gf16.h
	gcc -c gf16.c

clean:
	rm *.o

run:	exec
	./exec