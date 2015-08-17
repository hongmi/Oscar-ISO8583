CC=gcc -g
INCLUDE=-Iinclude
LIBRARY=-Llib

all: iso8583 publish demo benchmark 

clean:
	make -C dl_iso8583 clean
	rm -f benchmark demo  *.o *~
	rm include lib -rf

iso8583:
	make -C dl_iso8583

benchmark:
	${CC} ${INCLUDE} -c benchmark.c 
	${CC} -o benchmark benchmark.o ${LIBRARY} -liso8583

demo:
	${CC} ${INCLUDE} -c demo.c 
	${CC} -o demo demo.o ${LIBRARY} -liso8583


publish:
	mkdir -p include lib
	cp dl_iso8583/*.h include/
	cp dl_iso8583/libiso8583.a lib/
