# **Operating System Simulation**

* A Simulation of how an Operating System runs, created in C++

## This Operating System contains the following:

* Real-Time (RT) Processes
* Common Processes
* Input/Output (I/O) Processes
* Memory Storage

### CPU Scheduling Scheme:
* 2 levels of a ready-queue
  - 1 for RT processes
  - 1 for common processes

* Each process uses a unique ID
* Each level uses a round-robin system
* Common proccesses are only run if there are no RT processes waiting
* RT processes take priority if they arrive while common processes are running, common processes are sent to the beginning of the common process ready-queue.

### Input/Output Processes:
* Uses a First Come First Serve (FCFS) queue

### Memory Storage:
* Memory management is simulated using a first-fit approach

## The program begins by asking the user 2 questions:
1. How much RAM memory is there on the simulated computer (in bytes)? (At least 1 byte and maximum of 4 Billion bytes)
2. How many hard disks does the simulated computer have? (At least 1)

## Simulation starts afterwards and these are the usable commands:

### Note: Commands are case sensitive

* A # - used to create a new common process, where # is the specified bytes of memory required to run the process
*        A 100 - means a new common process is created that requires 100 bytes of memory

* AR # - used to create a new real-time process, where # is the specified bytes of memory required to run the process
*        AR 100 - means a new common process is created that requires 100 bytes of memory

* Q - the current process that is using the CPU ends 
*     the process goes to the end of the queue in its appropriate position

* t - the current process that is using the CPU terminates (the process' memory is freed and ID is deleted)

* d # - the process that is currently using the CPU is requesting to use hard disk #
*      d 1 - the current running process is requesting to use hard disk 1 and leaves the CPU ready-queue
*      the CPU ready-queue is then updated to be empty or is being used by the next process in the ready-queue

* D # - the hard disk # has finished running for one process
*       D 1 - the process that was requesting disk 1 is finished and leaves the I/O queue
*       the process re-enters the CPU ready-queue in its appropriate spot

* S r - displays which process is using the CPU and which processes are waiting in both levels of the ready-queue

* S i - displays which processes are currently using which hard disks and their queues for each (I/O queue)

* S m - displays the memory state for the simulated operating system

## Commands to run on a terminal

To compile:
* make all

To delete executables and object file type:
* make clean
