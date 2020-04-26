all:	CA_routine.c CA_routinePT.c
	gcc -g -O2 --std=c99 -Wno-unknown-pragmas -Wall -o ca_model CA_routine.c
	gcc -g -O2 --std=gnu99 -Wno-unknown-pragmas -Wall -o ca_routinePT CA_routinePT.c -lpthread

clean:
	rm -f ca_model
	rm -f ca_routinePT
