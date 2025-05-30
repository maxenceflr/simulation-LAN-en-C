FLAGS=-Wall -Wextra -Werror -Wno-unused-parameter -pedantic
all : main

reseau : reseau.c reseau.h
	gcc -c reseau.c ${FLAGS}

initReseau : initReseau.c initReseau.h
	gcc -o initReseau.c ${FLAGS}

graphe : graphe.c graphe.h
	gcc -c graphe.c ${FLAGS}

clean:
	rm *.o
