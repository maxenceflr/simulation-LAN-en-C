FLAGS=-Wall -Wextra -Werror -Wno-unused-parameter -pedantic
all : reseau, initreseau

reseau : reseau.c reseau.h
	gcc -c reseau.c ${FLAGS}

initreseau : initreseau.c 
	gcc -c initreseau.c ${FLAGS}


