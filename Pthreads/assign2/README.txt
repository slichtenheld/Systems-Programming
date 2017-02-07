Samuel Lichtenheld

I used mutexes and conditions to solve this producer consumer problem. 

Since there was only one producer thread and one consumer thread I used 
the "poison pill" methodology to shut down the program. This consists of
placing a terminate message in the buffer that the consumer will then 
read, allowing for both threads to be safely shut down. Once these threads
both exit, the main thread does some clean up and then exits as well.

Rather than having a giant main.c file, I split everything into separate files.
Hopefully this makes it easier to understand my work as well. 

I tested using the sample input provided. 