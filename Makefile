ca_model: CA_routine.c
	gcc -std=c99 -o ca_model CA_routine.c
clean:
	rm -f ca_model
