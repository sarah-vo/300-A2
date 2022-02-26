all:
	# gcc -Wall -g -std=c99 .D _POSIX_C_SOURCE=200809L .Werror soc.c -o soc -lpthread
	gcc -g soc.c input.c output.c receive.c send.c -o soc -lpthread

clean:
	rm soc