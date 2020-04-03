ca_model: CA_routine.c
	gcc -g -O2 --std=c99 -Wno-unknown-pragmas -Wall -o ca_model CA_routine.c
clean:
	rm -f ca_model
