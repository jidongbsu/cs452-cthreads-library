# Overview

In this assignment, we will write a user level thread library called cadillac-threads, or cthreads. Note this is NOT a kernel project, and you should just develop your code on onyx, not in your virtual machine. Submissions fail to compile or run on onyx, will not be graded.

# Learning Objectives

- Gaining a deep understanding of user-level thread libraries.
- Implementing a round robin scheduler.
- Practicing on managing a queue data structure.
- Creating a large piece of system software in stages.

# Book References

- [Concurrency and Threads](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf)
This chapter explains how multiple threads programs look different from single thread programs.

- [Threads API](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf)
This chapter describes the APIs provided by the pthread library, your cthreads library will provide very similar APIs, with some slight differences.

- [Locks](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf)
The test-and-set example (figure 28.3) described in this chapter is directly related to this assignment and you should use it.

- [CPU Scheduling](https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf)
This chapter has more explanation about the round robin scheduling policy, as well as the concept of time slicing.

# Specification

## user level thread library vs kernel level thread library

In previous projects we used pthreads library, which allows us to run multiple threads concurrently. pthreads library are supported by the Linux kernel, and each pthread is mapped into one kernel thread, and the kernel manages to schedule these threads as if each thread is a seperate process. This suggests such threads are visible to the kernel, and that's why when you run *ps -eLf*, you can see multiple theads of the same process.

```console
(base) [jidongxiao@onyx ~]$ ps -eLf | grep test-mergesort
jidongx+ 3622074 3618225 3622074 97    7 18:14 pts/1    00:00:35 ./test-mergesort 100000000 2 1234
jidongx+ 3622074 3618225 3622087  0    7 18:14 pts/1    00:00:00 ./test-mergesort 100000000 2 1234
jidongx+ 3622074 3618225 3622088  0    7 18:14 pts/1    00:00:00 ./test-mergesort 100000000 2 1234
jidongx+ 3622074 3618225 3622089  0    7 18:14 pts/1    00:00:00 ./test-mergesort 100000000 2 1234
jidongx+ 3622074 3618225 3622090  0    7 18:14 pts/1    00:00:00 ./test-mergesort 100000000 2 1234
jidongx+ 3622074 3618225 3622091  0    7 18:14 pts/1    00:00:00 ./test-mergesort 100000000 2 1234
jidongx+ 3622074 3618225 3622092  0    7 18:14 pts/1    00:00:00 ./test-mergesort 100000000 2 1234
```

Note here there is only one process, which is launched by this command:

```console
(base) [jidongxiao@onyx p1]$ ./test-mergesort 100000000 2 1234
Serial Sorting 100000000 elements took 26.80 seconds.
Parallel(2 levels) Sorting 100000000 elements took 8.50 seconds.
```

In this assignment, we aim to implement a user level thread library which do not require that much support from the kernel, and that means our threads are not visible to the kernel, and therefore they will be collectively treated as one process and the kernel will allocate time slices to this one single process. Inside this process, it is our responsibility to allocate time slices to each thread, and switch between our threads, so that every thread of our process will have a chance to run. Such a model determines that we will not be able to take advantage of multiprocessing. However, user level threads are still expected to be fast, because they require fewer context switches between user mode and kernel mode.

## requirements

You are required to implement the following functions:

### part 1

```c
int cthread_create(cthread_t *thread, void *(*start_routine) (void *), void *arg);
```

This function creates a new thread. The new thread starts execution by invoking *start_routine*(); *arg* is passed as the sole argument of *start_routine*(). Before returning, a successful call to *cthread_create*() stores the ID of the new thread in the address pointed to by thread; the user who uses your library is responsible for allocating memory for the address pointed to by *thread*. It is also the user's responsibility to define the *start_routine*() and pass the correct *arg*.

```c
int cthread_init();
```

This function is not provided for the user, but you should call it in your *cthread_create*() to initialize your library.

```c
void cthread_exit(void *retval);
```

This function exits the current thread. In this assignment, we do not intend to use the argument *retval*.

```c
int cthread_join(cthread_t thread, void **retval);
```

This function let current thread wait for the exit of another thread. In this assignment, we do not intend to use the argument *retval*.

```c
static void schedule(int sig);
```

This function implements the round robin scheduling.

### part 2

```c
int cthread_mutex_init(cthread_mutex_t *mutex);
int cthread_mutex_lock(cthread_mutex_t *mutex);
int cthread_mutex_unlock(cthread_mutex_t *mutex);
```

The user of your library calls these 3 functions to initialize a lock, grab a lock, release a lock, respectively.

# The Starter Code

The starter code look like this.

```console
(base) [jidongxiao@onyx cs452-cthreads-library]$ ls
cthreads.c  cthreads.h  cthreads-test1.c  cthreads-test2.c  cthreads-test3.c  cthreads-test4.c  cthreads-test5.c  Makefile  README.md  README.template
```

# Predefined Data Structures and Global Variables

# Provided Helper Functions

# APIs

I used the following APIs.

## ucontext APIs

```c
int getcontext(ucontext_t *ucp);
int setcontext(const ucontext_t *ucp);
void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);
int swapcontext(ucontext_t *oucp, const ucontext_t *ucp);
```

**getcontext**() saves the current context in the structure pointed by *ucp*. **setcontext**() restores to the previously saved context - the one pointed by *ucp*. **makecontext**() modifies a context (pointed by *ucp*), so that when this context is restored, *func*() will be called. **swapcontext**() saves the current context in the structure pointed to by *oucp*, and then activates the context pointed to by *ucp*.

## timer APIs

```c
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
```

Read the man page of this function to see how to use it. You will need to use this function so as to set a timer, which goes off every 50 milliseconds, which is why the starter code has this line (in cthreads.c):

```c
#define QUANTUM  50000
```

The timer you should use, which is the first argument of *setitimer*(), which is named as *which*, is *ITIMER_PROF*. To use *setitimer*(), you need to initialize a *struct itimerval* structure and pass its address to *setitimer*() as its second argument. To this end, a global variable is defined as the beginning of cthreads.c:

```c
struct itimerval time_quantum;
```

You can initialize this global variable like this:

```c
    /* we set it_value to determine when we want the first timer interrupt. */
    time_quantum.it_value.tv_sec = 0;
    time_quantum.it_value.tv_usec = QUANTUM;
    /* we set it_interval to determine that we want the timer to go off every 50 milliseconds. */
    time_quantum.it_interval = time_quantum.it_value;
```

Once *time_quantum* is initialized, you can pass its address as the second parameter to *setitimer*() like this:

```c
    if ((setitimer(ITIMER_PROF, &time_quantum, NULL)) != 0) {
        printf("oh no, setting a timer can fail?\n");
        return errno;
    }
```

Once *setitimer*() succeeds, a timer interrupt will trigger every 50 milliseconds. The man page of *setitimer*() says "at each expiration, a SIGPROF signal is generated". In this assignment, you should implement an signal handler to handle this signal. A signal handler is a function which will be called when the signal is generated - the OS will generate the signal, and you just need to tell the OS which function is your signal handler, and the OS will call that signal handler to handle this signal. Apparently, this signal handler is your *schedule*() function - you want make a scheduling decision every 50 milliseconds, because that is the foundation of the round robin scheduling.

## signal handling APIs

TO setup the signal handler, the following APIs are useful.

```c
int sigemptyset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```

To facilitate you use these APIs, a global variable is defined:

```c
struct sigaction scheduler;
```

You can initialize this variable like this:

```c
scheduler.sa_handler = schedule;
scheduler.sa_flags = SA_RESTART;
sigemptyset(&scheduler.sa_mask);
sigaddset(&scheduler.sa_mask, SIGPROF);
```

Once it's initialized, you can install the signal handler like this:

```c
    if ((sigaction(SIGPROF, &scheduler, NULL)) != 0) {
        printf("oh no, installing a signal handler can fail?\n");
        return errno;
    }
```

Once *sigaction*() succeeds, every 50 milliseconds, your *schedule*() will be called.

## other APIs

Anytime in any of the functions you implement in this assignment, you are recommended to call *exit*() like this:

```c
exit(EXIT_FAILURE);
```

This is because "man 3 exit" says "The use of *EXIT_SUCCESS* and *EXIT_FAILURE* is slightly more portable (to non-UNIX environments) than the use of 0 and some nonzero value like 1 or -1." There is also one occasion in which you may want to call *exit*() like this:

```
exit(EXIT_SUCCESS);
```

Think about in which function you want to call this.

# Submission

Due: 23:59pm, April 14th, 2022. Late submission will not be accepted/graded.

# Project Layout

All files necessary for compilation and testing need to be submitted, this includes source code files, header files, and Makefile. The structure of the submission folder should be the same as what was given to you.

# Grading Rubric (Undergraduate and Graduate)
Grade: /100

- [ 80 pts] Functional Requirements:
  - [10 pts] thread schedule, join, exit works correctly - tested by cthreads-test1.
  - [10 pts] thread schedule, join, exit works correctly - tested by cthreads-test2.
  - [20 pts] thread lock/unlock works correctly - tested by cthreads-test3.
  - [20 pts] thread lock/unlock works correctly - tested by cthreads-test4.
  - [20 pts] thread lock/unlock works correctly - tested by cthreads-test5.

- [10 pts] Compiler warnings:
  - Each compiler warning will result in a 3 point deduction.
  - You are not allowed to suppress warnings.
  - You won't get these points if you didn't implement any of the above functional requirements.

- [10 pts] Documentation:
  - README.md file: replace this current README.md with a new one using the template. Do not check in this current README.
  - You are required to fill in every section of the README template, missing 1 section will result in a 2-point deduction.
