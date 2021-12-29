CC=gcc
CFLAGS=-Wall -MMD -g -O0 #-fPIC
LIBS=-L../lib -lcthreads-p1 -L../lib -lcthreads-p2

all: bad-bank-balance safe-bank-balance philosopher

bad-bank-balance: bad-bank-balance.o
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

safe-bank-balance: safe-bank-balance.o
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

philosopher: philosopher.o
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

clean:
	rm -rf *.o a.out *.so *.a *.d
	rm -f bad-bank-balance safe-bank-balance philosopher
