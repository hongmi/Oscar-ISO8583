CC=gcc -g
SRCDIR=dl_iso8583
INCLUDE=-I${SRCDIR}
LIBRARY=-L${SRCDIR}

all: iso8583 demo benchmark 

clean:
	make -C dl_iso8583 clean
	rm -f benchmark demo  *.o *~
	rm include lib -rf

iso8583: 
	make -C dl_iso8583

benchmark:benchmark.c iso8583
	${CC} ${INCLUDE} -c benchmark.c 
	${CC} -o benchmark benchmark.o ${LIBRARY} -liso8583

demo:demo.c iso8583
	${CC} ${INCLUDE} -c demo.c 
	${CC} -o demo demo.o ${LIBRARY} -liso8583


publish:
	mkdir -p include lib
	cp dl_iso8583/*.h include/
	cp dl_iso8583/libiso8583.a lib/
