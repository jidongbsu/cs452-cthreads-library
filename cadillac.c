#include <signal.h> /* for signal handling */
#include <sys/time.h> /* for timing */
#include <stdlib.h> /* for exit */
#include <errno.h> /* for errno */
#include <stdio.h> /* for printf */
#include "cadillac.h"

#define MAX_NUM_THREADS   64  /* we limit our maximum number of threads to be 64 */

/* ready queue. we put threads into this queue so they will be scheduled.
 */

struct Queue {
    int front, rear, size;
    int* array;
};

/* a round-robin scheduler, runs every time the timers goes off.
 */
void schedule(int sig) {
}

/* terminate calling thread.
 */
void cthread_exit(void *retval) {
}

/* make calling thread wait for termination of some other thread.
 * only threads who have kids are supposed to call join.
 */
int cthread_join(cthread_t thread, void **retval) {
	return 0;
}

/* initialize a mutex.
 */
int cthread_mutex_init(cthread_mutex_t *mutex) {
	return 0;
}

/* test and set; matches with the book chapter.
 */

static inline uint xchg(volatile unsigned int *old_ptr, unsigned int new) {
	uint old;
	asm volatile("lock; xchgl %0, %1" :
			"+m" (*old_ptr), "=a" (old) :
			"1" (new) : "cc");
	return old;
}

/* lock a mutex.
 */
int cthread_mutex_lock(cthread_mutex_t *mutex) {

	//while (xchg(&(mutex->lock), 1) == 1)
	//{
	//}
	return 0;
}

/* unlock a mutex.
 */
int cthread_mutex_unlock(cthread_mutex_t *mutex) {
	return 0;
}

/* vim: set ts=4: */
