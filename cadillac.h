#ifndef CADILLAC_H
#define CADILLAC_H

#include <ucontext.h> /* for user thread context */

#define TRUE 1
#define FALSE 0

#define STACK_SIZE  64*1024 /* we choose 64k as our stack size, for each thread */
#define MAX_NUM_THREADS   64  /* we limit our maximum number of threads to be 64 */
#define DEBUG 1

typedef int cthread_t;
/* current_thread_id changes in 4 situations:
 * 1. schedule
 * 2. calling lock finds lock is held
 * 3. child exit - child wants to wake up parent (or let someone else run)
 * 4. parent join - because parent wants to wait */
extern cthread_t current_thread_id;             // id of the current running thread

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

extern cthread_t ready[MAX_NUM_THREADS];
extern int ready_head, ready_tail;

/* read a fresh thread id.
 */
cthread_t read_fresh_tid();

/* put back a thread id to be fresh.
 */
void write_fresh_tid(cthread_t tid);

/* see if the fresh id queue is empty.
 */
int is_fresh_queue_empty();

/* read a ready thread id.
 */
cthread_t read_ready_tid();

/* write back a ready thread id.
 */
void write_ready_tid(cthread_t tid);

/* see if the ready queue is empty.
 */
int is_ready_empty();

/* get ready queue size.
 */
int ready_size();

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
