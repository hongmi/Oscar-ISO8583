CC=gcc -g
INCLUDE=-Idl_iso8583
LIBRARY=-Ldl_iso8583

all: iso8583 demo benchmark

clean:
	make -C dl_iso8583 clean
	rm -f benchmark demo  *.o *~

iso8583:
	make -C dl_iso8583

benchmark:
	${CC} ${INCLUDE} -c benchmark.c 
	${CC} -o benchmark benchmark.o ${LIBRARY} -liso8583

demo:
	${CC} ${INCLUDE} -c demo.c 
	${CC} -o demo demo.o ${LIBRARY} -liso8583
