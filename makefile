CC =			gcc
CFLAGS =		-Wall -g
LDLIBS =		-lSDL2 -lSDL2_gfx -lm

all:			wireframe

wireframe:		wireframe.o Malloc.o list.o obj.o graphics.o

Malloc.o:		Malloc.c Malloc.h
list.o:			list.c list.h Malloc.h
obj.o:			obj.c obj.h list.h
graphics.o:		graphics.c graphics.h obj.h
wireframe.o:	wireframe.c graphics.h

clean:
				-rm *.o

purge:			clean
				-rm wireframe