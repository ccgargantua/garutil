all:
	gcc -Igarutil/headeronly -o tests test/tests.c

clean:
	rm tests