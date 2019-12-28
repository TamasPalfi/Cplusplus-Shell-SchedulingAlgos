# Overview

The purpose of this lab is to gain experience working with system
calls, process management, and scheduling policies. There are two
independent exercises in the lab: the first exercises is to implement
a basic shell in C/C++ and the second requires you to
implement/simulate four different scheduling policies given a process
workload.

# Part 1: Simple Shell in C++

The goal of this lab is to implement a simple shell in C++. This
requires you to use system calls directly in your code. In class, we
touched upon how a few system calls, (notably fork and exec) can be
used to implement a command shell. In this exercise, you will
implement tsh (trivial shell), a simple shell.

Like a real shell, `tsh` will take as input the name of a program as
argument. The program can be the name of any executable in the current
directory where your shell program resides. The shell should run the
specified program with the arguments before prompting for a new user
command.

The command "quit" should terminate your shell.

Here is a simple example:

```
$ ./tsh
tsh> ls
foo.txt
tsh> quit
$
```

To get started, clone the starter code from github. Do not change any
file names or function names that are provided to you. Simply
implement the real process logic.

## Shell Notes

* Useful functions and system calls include
  [fork](http://linux.die.net/man/3/fork),
  [exec](http://linux.die.net/man/3/execvp) (specifically the `execvp`
  variant, in conjunction with the `cmdTokens` variable),
  [sleep](http://linux.die.net/man/3/sleep),
  [waitpid](http://linux.die.net/man/3/waitpid).

* If you're trying to use
  [waitpid](https://linux.die.net/man/3/waitpid) and get a warning
  like "warning: implicit declaration of function 'waitpid'", you
  probably need to include an additional system header file. Add the
  line:

    ```c++
    #include <sys/wait.h>
    ```
    
  to the top of your file alongside the other `#include`
  statements.

* Be careful when adding calls to `fork` -- if you write an infinite
  loop with a fork in it, a [fork
  bomb](http://en.wikipedia.org/wiki/Fork_bomb) will result. If in
  doubt, you can add a sleep(1); before each fork during debugging,
  which will slow the rate of process creation.

* `tsh` can execute a program with arguments, but cannot execute
  multiple programs using Bash constructs (e.g., 'sleep 3 && echo
  hello' to sleep for 3 seconds, then print hello). However, you can
  accomplish the same by making a new Bash file (e.g., 'sleephello')
  and calling that from within tsh (e.g., './sleephello'). If you do
  this, make sure the script you are trying to call is executable
  ('chmod +x sleephello').

* Since each OS has its own (different) system call interface, you
  should use linux machines since `fork`, `exec` are Unix/linux system
  calls. You can use your own machine to write the code, but you
  *must* ensure that it runs on linux and in the programming
  environment we provide you as part of this course.

* Do not use cygwin/windows for this lab (`fork`/`exec`) are not
  supported on windows.

# Part 2: Scheduling

The goal of this part of the assignment is to implement four different
scheduling algorithms in a scheduling simulator. In particular, you
are to implement:

* Firt-In, First-Out (FIFO)
* Shortest Job First (SJF)
* Shortest Time to Completion First (STCF)
* Round Robin (RR)

If you need to brush up on these algorithms before starting you should
review the chapter in the book that discusses [CPU
scheduling](http://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf)
and/or review the slides covering this material from class.

You are given some starter code that defines the interface to the
scheduling simulator and some basic data structures to get you
going. You cannot change the interface or the data structures. If you
do the automated tests may fail and you will be heavily penalized. The
files for this part of the lab include:

* `src/scheduling.h`
* `src/scheduling.cpp`
* `src/main_scheduling.cpp`

You need not touch `scheduling.h` or `main_scheduling.cpp`. All the
work that you need to do can be found inside `scheduling.cpp`.

## Scheduling Notes

* The files inside the workloads directory describe a workload of
  processes and their start time and execution duration. The
  definition is two integers per line:

    ```
    0 100
    10 10
    10 10
    ```

  The first integer is the start time and the second integer is how
  long the process needs to run (the duration).

* The **`Process`** struct records all the information you need to know
  about a process before and after it runs through a scheduling
  algorithm. Only the `arrival` time and `duration` are known before a
  process executes. The `first_run` field is updated to indicate when
  a process first begins executing and `completion` is the time a
  process completes executing.

* The **`ArrivalComparator`** and **`DurationComparator`** are two
  classes that define an ordering on processes. The former is used to
  order processes by their arrival time. The later is used to order
  processes by their duration. These classes are used as part of the
  definition of priority queues that are used to manage processes.

* To make it easier to define priority queues we define two new types
  using typedef: **`pqueue_arrival`** and **`pqueue_duration`**. The
  former is used to define priority queues that order processes by
  arrival time and the later is used to define priority queues that
  order processes by duration (how long they have left to execute).

* You are to implement a function to read in a workload file and
  return a priority queue ordered by arrival time:

    ```c++
    pqueue_arrival read_workload(string filename)
    ```

* You are to implement 4 CPU scheduling algorithms:

  ```c++
  list<Process> fifo(pqueue_arrival workload);
  list<Process> sjf(pqueue_arrival workload);
  list<Process> stcf(pqueue_arrival workload);
  list<Process> rr(pqueue_arrival workload);
  ```

  Each CPU scheduling function takes a priority queue of processes
  ordered by their arrival time and returns a list of processes
  ordered by their completion time. In addition, the processes that
  are returned have their `first_run` and `completion` fields filled
  in. You can assume a time slice of 1 time unit for the round robin
  scheduler and all processes are added to the end of the process
  queue upon arrival.

  TIP: you are likely to need both a queue containing the entire
  workload and another queue for processes that have already arrived
  for most of the above algorithms.

* You are to implement two metric producing functions:

    ```c++
    float avg_turnaround(list<Process> processes)
    float avg_response(list<Process> processes)
    ```

  The first returns the average turnaround time given a list of
  processes. The second returns the average response time given a list
  of processes.

* You are free to choose how you implement each of the above
  functions. However, our recommendation is to not use references and
  pointers. Our implementation did not use any references or pointers
  which made it easier to reason about what we were doing. If you need
  to modify anything, it is simply easier to make a copy of it. See
  the `show_workload` and `show_processes` for examples on how we
  copied priority queues. Could you avoid using copies? Yes, and it
  would make the simulator faster - however, we recommend that you go
  for simplicity before choosing to optimize.

# Debugging Help

It is important that you use the `gdb` debugger to debug your code
when you encounter problems. You can easily start the `gdb` debugger
from the command line:

```bash
$ gdb PROGRAM
```

Where `PROGRAM` is the program you compiled. You should look at the
provided `gdb` cheatsheet to see some of the commands you can
execute. If you need additional help you can take a look at [this
tutorial](https://www.cs.cmu.edu/~gilpin/tutorial/).

You will inevitably encounter cases when your code fails a test or
worse, the test program exits with a segmentation violation
(segfault). To debug the code in a test requires you to understand how
the google test framework generates C++ code and how the C++ compiler
generates method signatures. In short, this is what you want to do:

```bash
$ gdb TEST_PROGRAM
(gdb) b TestSuite_TestName_Test::TestBody()
```

The `SuiteName` and `TestName` correspond to how you write a test
using the google test framework. In particular, this is the basic
structure of a test:

```C++
TEST(SuiteName, TestName) {
  // the test body
}
```

You should also know that the `b` (break) command provides tab
completion. So, you can type in the following:

```bash
(gdb) b TestSuite[TAB][TAB]
```

The `[TAB]` is hitting the `tab` key on your keyboard. You can hit it
twice in rapid succession to see all the possible completions.

# C++ Resources

This assignment touches on a variety of C++ language concepts and
libraries that are useful to understand before you get started. The
following links should help you in your work on this assignment. You
should take the time to review these resources before and during the
assignment to help you complete your work:

* [Basic I/O](http://www.cplusplus.com/doc/tutorial/basic_io/)
* [Namespaces](http://www.cplusplus.com/doc/oldtutorial/namespaces/)
* [Classes](http://www.cplusplus.com/doc/tutorial/classes/)
* [Priority Queue](http://gigi.nullneuron.net/comp/cpp-stl-priority-queue.php)
* [List](https://thispointer.com/stdlist-tutorial-and-usage-details/)

Again, spend some time studying up on these C++ basics. The best way
to do this is to write out the examples and compile them to see how
they work. Spending a little time to prepare will go a long way in
successfully implementing this assignment.