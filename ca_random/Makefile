CFLAGS=-g -O2 -Wall --std=c99
TARGETS=rand_ind rand_ord_serial rand_ord_pthreads

all: $(TARGETS)

rand_ind: ca_rand_ind.c
	gcc $(CFLAGS) -o $@ $<

rand_ord_serial: ca_rand_order_serial.c
	gcc $(CFLAGS) -o $@ $<

rand_ord_pthreads: ca_rand_order_pthreads.c
	gcc $(CFLAGS) -o $@ $< -lpthread

clean:
	rm -f $(TARGETS)
