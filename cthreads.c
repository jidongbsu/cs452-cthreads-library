#include <signal.h> /* for signal handling */
#include <sys/time.h> /* for timing */
#include <stdlib.h> /* for exit, malloc, free */
#include <errno.h> /* for errno */
#include <stdio.h> /* for printf */
#include <string.h> /* for memset */
#include <limits.h> /* for INT_MIN */
#include "cthreads.h"

#define MAX_NUM_THREADS 64	/* we limit our maximum number of threads to be 64 */
#define QUANTUM 50000	/* alarm goes off every 50 milliseconds, setitimer may fail (return -1) if this number is too big  */

/* an array for all 64 thread control blocks */
static thread_control_block tcbs[MAX_NUM_THREADS];

/* initialization flag, set this to true when cthread_init() is called already. */
int initialized = FALSE;

/* id of the current running thread.
 * current_tid changes in 4 situations:
 * 1. schedule
 * 2. calling lock finds lock is held
 * 3. child exit - child wants to wake up parent (or let someone else run)
 * 4. parent join - because parent wants to wait
 **/
static cthread_t current_tid;

/* This structure is used to specify when a timer should expire. It contains the following members:
 *
 *	struct timeval it_interval
 *	This is the period between successive timer interrupts. If zero, the alarm will only be sent once.
 *
 *	struct timeval it_value
 *	This is the period between now and the first timer interrupt. If zero, the alarm is disabled.
 */
struct itimerval time_quantum;

/* sigaction for scheduler */
static struct sigaction scheduler;

/* when this flag is one, tells the schedule not to schedule me out */
static char no_schedule = 0;

/* a global variable, we increment this by one every time we create a thread */
cthread_t tid_idx = 0;

/* Part 0: the ready queue, which is a circular queue, kind of... */

/* a structure to represent a queue */
struct Queue {
	/* front and rear are both indexes, indexes of the array. */
    int front, rear, size;
	/* we use this queue to store tids. */
    int tids[MAX_NUM_THREADS];
};

/* is queue full? 
 * functions not exported to outside should be declared as static. */
static int isFull(struct Queue* queue)
{
    return (queue->size == MAX_NUM_THREADS);
}

/* is queue empty? 
 * functions not exported to outside should be declared as static. */
static int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
 
/* add a tid to the queue - to the back of the queue. */
void cthread_enqueue(struct Queue* queue, int tid)
{
    if (isFull(queue)){
		printf("queue is full, you can't add more...\n");
        return;
	}
    queue->rear = (queue->rear + 1) % MAX_NUM_THREADS;
    queue->tids[queue->rear] = tid;
    queue->size = queue->size + 1;
//    printf("tid %d is enqueued\n", tid);
}
 
/* remove a tid from the queue - from the front of the queue. */
int cthread_dequeue(struct Queue* queue)
{
    if (isEmpty(queue)){
		printf("stop! queue is empty, you can't dequeue anything...\n");
        return INT_MIN;
	}
    int tid = queue->tids[queue->front];
    queue->front = (queue->front + 1) % MAX_NUM_THREADS;
    queue->size = queue->size - 1;
    return tid;
}

/* ready queue. we put thread IDs into this queue so they will be scheduled. a fancy way to initialize this struct.
 */

struct Queue ready_queue = {.front = 0, .rear = MAX_NUM_THREADS - 1, .size = 0, .tids = {-1}};

/* Part 1: threads */

/* a round-robin scheduler, runs every time the timers goes off.
 * the first time when this function is called and switches contexts,
 * it places the main thread into the queue, and pick one child thread to run.
 * functions not exported to outside should be declared as static.
 */
static void cthread_schedule(int sig) {
}

/* initialize this library - functions not exported to outside should be declared as static. */
static int cthread_init() {
}

/* starts a new thread in the calling process.
 * the new thread starts execution by invoking start_routine();
 * arg is passed as the sole argument of start_routine().
 * before returning, a successful call to cthread_create() stores
 * the ID of the new thread in the buffer pointed to by thread;
 */
int cthread_create(cthread_t *thread, void *(*start_routine) (void *), void *arg) {

/* terminates the calling thread and returns a value via retval that
 * (if the thread is joinable) is available to another thread
 * in the same process that calls cthread_join().
 * in this assignment, we do not use retval.
 * note: we always assume child calls cthread_exit() to exit.
 * but the main thread never calls this function.
 */
void cthread_exit(void *retval) {
}

/* waits for the thread specified by thread to terminate.
 * if that thread has already terminated, then cthread_join() returns immediately.
 * only threads who have kids are supposed to call join.
 * on success, cthread_join() returns 0; on error, we print an error message and exit.
 * in this assignment, we do not use retval;
 * plus, in our applications, right now, we do not use the return value of cthread_join().
 */
int cthread_join(cthread_t thread, void **retval) {
	return 0;
}

/* Part 2: locks */

/* initialize a mutex. */
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

/* lock a mutex, returns 0 if successful. */
int cthread_mutex_lock(cthread_mutex_t *mutex) {

	//while (xchg(&(mutex->lock), 1) == 1)
	//{
	//}
	return 0;
}

/* unlock a mutex, returns 0 if successful. */
int cthread_mutex_unlock(cthread_mutex_t *mutex) {
	return 0;
}

/* vim: set ts=4: */
