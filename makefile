FLAGS=-Wall -Wextra -Werror -Wno-unused-parameter -pedantic
all : main

reseau : reseau.c reseau.h
	gcc -c reseau.c ${FLAGS}

initReseau : initReseau.c filesystem.o
	gcc -o initReseau.c filesystem.o ${FLAGS}

clean:
	rm *.o
