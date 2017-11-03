CC =			gcc
CFLAGS =		-Wall -g

all:			wireframe

wireframe:		wireframe.o Malloc.o list.o obj.o

Malloc.o:		Malloc.c Malloc.h
list.o:			list.c list.h Malloc.h
obj.o:			obj.c obj.h list.h
wireframe.o:	wireframe.c obj.h

clean:
				-rm *.o

purge:			clean
				-rm wireframe