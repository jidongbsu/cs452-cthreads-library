#ifndef CTHREAD_H
#define CTHREAD_H

#include <ucontext.h> /* for user thread context */
#include <sched.h> /* for struct sched_param - which unfortunately is useless in this program... */

#define TRUE 1
#define FALSE 0

#define STACK_SIZE  64*1024 /* we choose 64k as our stack size, for each thread */

typedef int cthread_t;

/* starts a new thread in the calling process. 
 * the new thread starts execution by invoking start_routine(); 
 * arg is passed as the sole argument of start_routine().
 * before returning, a successful call to cthread_create() stores 
 * the ID of the new thread in the buffer pointed to by thread;
 */
int cthread_create(cthread_t *thread, void *(*start_routine) (void *), void *arg);

/* terminates the calling thread and returns a value via retval that
 * (if the thread is joinable) is available to another thread 
 * in the same process that calls cthread_join().
 */
void cthread_exit(void *retval);

/* waits for the thread specified by thread to terminate. 
 * if that thread has already terminated, then cthread_join() returns immediately. 
 */
int cthread_join(cthread_t thread, void **retval);

/* thread control block. */

typedef struct {
	int exited;	// is this thread exited
	int waiting;	// who are you (as a parent) waiting for, at any given moment, you will wait for at most one thread. your waiting would be -1 if you're not waiting for anyone.
	cthread_t parent;	// parent thread id
	ucontext_t uc;	// user thread context
	char stack[STACK_SIZE];	// each thread has its own private stack
} thread_control_block;

/* mutex struct. */

typedef struct {
	unsigned int lock;
} cthread_mutex_t;

/* initialize a mutex. */
int cthread_mutex_init(cthread_mutex_t *mutex);

/* lock a mutex.
 * if the mutex is already locked by another thread, the calling thread
 * shall yield the CPU for now and retry again later to see if the mutex becomes available.
 */
int cthread_mutex_lock(cthread_mutex_t *mutex);

/* unlock a mutex.
 * release the mutex object referenced by mutex.
 */
int cthread_mutex_unlock(cthread_mutex_t *mutex);

#endif /* CTHREAD_H */

/* vim: set ts=4: */
