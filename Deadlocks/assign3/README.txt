Samuel Lichtenheld
2/17/17

Structure of program:

  Thread perspective:
    - main thread
        - opens and reads given input file.
        - parses input file 
            - if account, adds it to list of accounts
            - if transfer, assigns it to worker thread
    - worker threads
        - takes transfer information and finds
          appropriate accounts in account list
        - locks data with semaphores before changing it
            - avoids deadlocks by locking smaller account first 

  Data structure perspective:
    - Accounts are added to a linked list
    - each worker thread has a handle to unique fifo
    - the main thread assigns work by placing transfers
      in different fifos. 

Testing: 
    I tested this program by writing an inputGenerator program.
    This program took in the number of accounts wanted, number
    of transactions, and a seed for the randomly generated 
    transactions. I ran the program with this generated code on 
    a dual-core laptop.

Results: 
	100 accounts 
	1000000 transactions

	Avg of 10 runs w/ 1 thread  : 2.5402551 
	Avg of 10 runs w/ 2 threads : 2.1615067
	Avg of 10 runs w/ 4 threads : 4.3415602

	Avg of 10 runs w/ 2 threads and NO LOCKING : 2.1489465

	Speedup from 2 threads compared with 1 thread : 1.1752242545
	Slowdown generated from synchronization : 1.0058448174

Past 2 threads, there was actual slowdown rather than speedup. 
This makes sense as I am running a dual-core laptop and 
hyper-threading should yield no benefit as each thread is using
the same functional units. While the speedup from the second 
thread was only 17.5%, this could be explained by the overhead of
reading in from a file** as well as the overhead of having a linked-
list data structure storing the handles to each separate account.
When the synchronization code was commented out, there was only a 
0.5% speedup. This means that the overhead is not with the 
syncronization logic of the program. 

** The moment data is read in from a file, it is assigned to a 
worker thread, theoretically making most of the time of the 
program from reading from the file. This could also be seen by 
varying the depth of the fifos that each worker thread used. 
There was no speedup past a depth of 5.

Using a profiler would be required for more indepth analysis.