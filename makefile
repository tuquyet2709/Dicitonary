TARGET = dictionary
CC = gcc
OBJS = demotd.o
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0` libbt.a


${TARGET}: ${OBJS} 
	${CC} ${CFLAGS} -o ${TARGET} ${OBJS} ${LIBS} 
demotd :
	cc - c demotd.c

clean:
	rm -f *.o ${TARGET}

