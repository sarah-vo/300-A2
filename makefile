all:
	# gcc -Wall -g -std=c99 .D _POSIX_C_SOURCE=200809L .Werror soc.c -o soc -lpthread
	gcc -g main.c soc.c output.c receive.c list.o -o s-talk -lpthread

input:
	gcc -g input.c list.o -o input -lpthread

clean:
	rm soc