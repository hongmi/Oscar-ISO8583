CC=gcc -g
INCLUDE=-I.

src=$(wildcard *.c)
dir=$(notdir $(src))
obj=$(patsubst %.c,%.o,$(dir))

.c.o:
	${CC} ${INCLUDE} -c $<

libiso8583.a : ${obj}
	ar -rv $@ *.o

all : libiso8583.a

clean:
	rm -f *.o *.a *~
