all:
	# gcc -Wall -g -std=c99 .D _POSIX_C_SOURCE=200809L .Werror soc.c -o soc -lpthread
	gcc -Wall -g soc.c -o soc -lpthread

clean:
	rm soc