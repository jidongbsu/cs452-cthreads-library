CC=gcc
CFLAGS=-Wall -MMD -g -O0 #-fPIC

#LIB1OBJS= cadillac-p1.o 
LIB2OBJS= cadillac.o 

all: libs

libs: libcthreads-p2.a #libcthreads-p2.a #libcthreads-p1.so libcthreads-p2.so
	make install

#libcthreads-p1.so: cadillac-p1.o
#	$(LD) -shared -o $@  cadillac-p1.o

#libcthreads-p1.a: $(LIB1OBJS)
#	$(AR)  rcv $@ $(LIB1OBJS)
#	ranlib $@

#libcthreads-p2.so: $(LIBOBJS)
#	$(LD) -shared -o $@  $(LIBOBJS)

libcthreads-p2.a: $(LIB2OBJS)
	$(AR)  rcv $@ $(LIB2OBJS)
	ranlib $@

install:
#	install --mode=444 -C libcthreads-p1.a ./lib
	install --mode=444 -C libcthreads-p2.a ./lib

clean:
	rm -rf *.o a.out *.so *.a *.d
	rm -f ./lib/libcthreads-p2.a
