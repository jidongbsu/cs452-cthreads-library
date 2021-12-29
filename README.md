# Overview

In this assignment, we will write a user level threads library called cadillac-threads, or cthreads.

# Specification

## user level threads library vs kernel level threads library

In previous projects we used pthreads library, which allows us to run multiple threads concurrently. pthreads library are supported by the Linux kernel, and each pthread is mapped into one kernel thread, and the kernel manages to schedule these threads as if each thread is a seperate process. This suggests such threads are visible to the kernel, and that's why when you run ps -eLf, you can see multiple theads of the same process.

```console
(base) [jidongxiao@onyx ~]$ ps -eLf | grep mytests
jidongx+ 3622074 3618225 3622074 97    7 18:14 pts/1    00:00:35 ./mytests 100000000 2 1
jidongx+ 3622074 3618225 3622087  0    7 18:14 pts/1    00:00:00 ./mytests 100000000 2 1
jidongx+ 3622074 3618225 3622088  0    7 18:14 pts/1    00:00:00 ./mytests 100000000 2 1
jidongx+ 3622074 3618225 3622089  0    7 18:14 pts/1    00:00:00 ./mytests 100000000 2 1
jidongx+ 3622074 3618225 3622090  0    7 18:14 pts/1    00:00:00 ./mytests 100000000 2 1
jidongx+ 3622074 3618225 3622091  0    7 18:14 pts/1    00:00:00 ./mytests 100000000 2 1
jidongx+ 3622074 3618225 3622092  0    7 18:14 pts/1    00:00:00 ./mytests 100000000 2 1
```

Note here there is only one process, which is launched by this command:

```console
(base) [jidongxiao@onyx p1]$ ./mytests 100000000 2 1
Serial Sorting 100000000 elements took 33.66 seconds.
Parallel(2 levels) Sorting 100000000 elements took 9.78 seconds.
```

In this assignment, we aim to implement a user level threads library which do not require that much support from the kernel, and that means our threads are not visible to the kernel, and therefore they will be collectively treated as one process and the kernel will allocate time slices to this one single process. Inside this process, it is our responsibility to allocate time slices to each thread, and switch between our threads, so that every thread of our process will have a chance to run. Such a model determines that we will not be able to take advantage of multiprocessing. However, user level threads are still expected to be fast, because they require fewer context switches between user mode and kernel mode.

## The Starter Code

To be added.

# References

To be added.

# Submission

Due: 23:59pm, Feburary 24th, 2022. Late submission will not be accepted/graded.

# Grading Rubric (Undergraduate and Graduate)
Grade: /100

- [ 90 pts] Functional Requirements:
  - [60 pts] thread init, create, join, exit works correctly - tested by bad-bank-balance, and parallel mergesort (when testing parallel mergesort, you can limit your array size to be no more than 1,000,000, an array of larger size might cause some stack overflow or heap overflow issue).
  - [30 pts] thread lock/unlock works correctly - tested by the safe-bank-balance.

- [10 pts] Documentation:
  - README.md file (replace this current README.md with a new one using the README template. You do not need to check in this current README file.)
