#ifndef CADILLAC_H
#define CADILLAC_H

#include <ucontext.h> /* for user thread context */

#define TRUE 1
#define FALSE 0

#define STACK_SIZE  64*1024 /* we choose 64k as our stack size, for each thread */
#define MAX_NUM_THREADS   64  /* we limit our maximum number of threads to be 64 */
#define DEBUG 1

typedef int cthread_t;
/* current_tid changes in 4 situations:
 * 1. schedule
 * 2. calling lock finds lock is held
 * 3. child exit - child wants to wake up parent (or let someone else run)
 * 4. parent join - because parent wants to wait */
extern cthread_t current_tid;             // id of the current running thread

/* thread control block.
 */

typedef struct {
    ucontext_t uc;              // user thread context
    cthread_t parent;           // parent thread id
    char valid;                 // set this to 1 indicating an id is in use
    char exited;                // set this to 1 when the thread is exiting
    unsigned int waiting;   // who are you (as a parent) waiting for
    char stack[STACK_SIZE];     // thread's own stack 
} thread_control_block;

extern thread_control_block tcbs[MAX_NUM_THREADS];    // all the thread control blocks

extern int no_schedule;                    // when this flag is one, tells the schedule not to schedule me out

/* helper variables and functions */

/* ready queue. we put threads into this queue so they will be scheduled.
 */

typedef struct {
    int front, rear, size;
    int* array;
} Queue;

extern Queue *ready_queue;

/* returns size of the queue */
int queue_size(Queue *queue);

/* returns 1 if the queue is full */
int queue_full(Queue *queue);

/* returns 0 if the queue is empty */
int queue_empty(Queue *queue);

// add a tid to the queue.
void enqueue_tid( Queue *queue, int tid);

// remove an tid from queue.
int dequeue_tid( Queue *queue);

/* starts a new thread in the calling process. 
 * the new thread starts execution by invoking start_routine(); 
 * arg is passed as the sole argument of start_routine().
 * before returning, a successful call to cthread_create() stores 
 * the ID of the new thread in the buffer pointed to by thread;
 */
int cthread_create(cthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

/* terminates the calling thread and returns a value via retval that
 * (if the thread is joinable) is available to another thread 
 * in the same process that calls cthread_join().
 */
void cthread_exit(void *retval);

/* waits for the thread specified by thread to terminate. 
 * if that thread has already terminated, then cthread_join() returns immediately. 
 */
int cthread_join(cthread_t thread, void **retval);

/* mutex data structure.
 */

typedef struct {
    unsigned int lock;                    // lock flag
} cthread_mutex_t;

/* initialize a mutex.
 */
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

#endif /* CADILLAC_H */
