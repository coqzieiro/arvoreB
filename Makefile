all:
	gcc -g -Wall -o programaTrab *.c

run: all
	./programaTrab

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./programaTrab

clean:
	rm *.exe