9cc: 9cc.c
all:
	gcc -o 9cc 9cc.c
	./test.sh
	rm -f 9cc *.o *- tmp*
test:
	./test.sh
clean:
	rm -f 9cc *.o *- tmp*