CC=gcc
AR = ar
CFLAGS=-Wall -MMD -g -O0 -fPIC -Wpointer-arith -std=gnu89 -MP

LIBOBJS = cthreads.a

OBJS =  cthreads-test1 cthreads-test2 cthreads-test3\
	cthreads-test4 cthreads-test5\

build = \
	@if [ -z "$V" ]; then \
		echo '  [$1]    $@'; \
		$2; \
	else \
		echo '$2'; \
		$2; \
	fi

% : %.c $(LIBOBJS)
	$(call build,LINK,$(CC) $(CFLAGS) $< $(LIBOBJS) -o $@)

%.o : %.c
	$(call build,CC,$(CC) $(CFLAGS) -c $< -o $@)

%.a : %.o
	$(call build,AR,$(AR) rcs $@ $^)

all: $(LIBOBJS) $(OBJS)

clean:
	rm -rf *.o a.out *.so *.a *.d
	rm -f *.o *.d $(OBJS) $(LIBOBJS)
