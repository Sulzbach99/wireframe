CC =			gcc
CFLAGS =		-Wall -g
LDLIBS =		-lSDL2 -lSDL2_gfx -lm

all:			wireframe

wireframe:		wireframe.o Malloc.o queue.o obj.o graphics.o

Malloc.o:		Malloc.c Malloc.h
queue.o:		queue.c queue.h Malloc.h
obj.o:			obj.c obj.h queue.h debug.h
graphics.o:		graphics.c graphics.h obj.h
wireframe.o:	wireframe.c graphics.h

clean:
				-rm *.o

purge:			clean
				-rm wireframe