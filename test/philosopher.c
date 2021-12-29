#include <stdio.h>
#include <stdlib.h>
#include "../cadillac.h"

#define N 5

cthread_mutex_t forks[N];

unsigned long fibonacci(unsigned long n)
{
   if (n <= 0)
      return 0;
   else if (n == 1)
      return 1;
   else
      return fibonacci(n-1) + fibonacci(n-2);
}

/* the two helper functions from the book chapter. */

int left(int p)  { return p; }
int right(int p) { return (p + 1) % N; }

/* get and put forks function from the bokk chapter */

void get_forks(int p) {
	if(p == N) {
		cthread_mutex_lock(&forks[right(p)]);
		cthread_mutex_lock(&forks[left(p)]);
	} else {
		cthread_mutex_lock(&forks[left(p)]);
		cthread_mutex_lock(&forks[right(p)]);
	}
}

void put_forks(int p) {
	cthread_mutex_unlock(&forks[left(p)]);
	cthread_mutex_unlock(&forks[right(p)]);
}

void* philosopher(void *arg) {
    long p;

    p = (long) arg;
    p = p+1;

	/* unlike the book chapter which uses a infinite while(1) loop, we do some computation and exit */
	fibonacci(36);
	get_forks(p);
	fibonacci(36);
	put_forks(p);

    printf("thread %ld exiting\n", p);
    cthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[]) {
    cthread_t tids[N];
    long i;
    
    for (i = 0; i < N; ++ i) {
        cthread_mutex_init(&forks[i]);
    }

    for (i = 0; i < N; ++ i) {
        cthread_create(&tids[i], NULL, philosopher, (void *) i);
    }

    for (i = 0; i < N; ++ i) {
        cthread_join(tids[i], NULL);
    }

    printf("main: exiting\n");

    return 0;
}

/* vim: set ts=4: */
