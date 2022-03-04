all:
	# gcc -Wall -g -std=c99 .D _POSIX_C_SOURCE=200809L .Werror soc.c -o soc -lpthread
	gcc -Werror -g main.c soc.c input.c send.c receive.c output.c list.o -o s-talk -lpthread

inputDEBUG:
	gcc -g input.c list.o -o input -lpthread

sendDEBUG:
	gcc -g input.c send.c soc.c list.o -o sendDEBUG -lpthread

clean:
	rm s-talk