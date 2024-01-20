Victor Rivera
1701239 (vrivera7)

DESIGN:
    DESCRIPTION:
        This program is a multithreaded safe queue that can hold size number of elements, where size is the input argument of the function queue_new() used to create the queue. The queue can perform push and pop operations on any data type and uses condition variables to prevent buffer overflow (data corruption/overwite). The function queue_delete() cleans up any memory allocation done by the queue.  
        
    
    THREAD SAFTY/PROTECTION CHOICES:
        This assignment uses a mutex to protect the critical region, and two condition variables - one to signal that an item has been removed from the queue and the other to signal that an item has been added to the queue. Although I had already finished this program using semaphores (making the code about 10 lines shorter), using a mutex made the code a little faster. I also had more experience using mutex and condition variables so I feel more sure about the functionality of this code.

    DATA STRUCTURES:
        queue:
            void **queue - array of pointers (pointer of pointers) used to store the elements of the queue.

            int back - index of the back of the queue (know where to push to)

            int front - index of the front of the queue (know where to pop from)

            int max - static value set to the size of the array (queue).

            int in_q - keeps track of the elements in the queue.

    queue_new:
        This function is used to initialize an allocate memory for the queue. In the function memory is allocated for the queue data structure as well as the queue array. The queue gets allocated size number of elements, where size is the input argument of the function. 
        
        The q member max gets set to size and the members front, back, and in_q are set to 0. Additionally, within this function, the mutex and condition variables are initialized.

    queue_delete:
        This function is used to deallocate/free all memory that was allocated for the queue. In the function, memory allocated to the queue array and queue data structure are freed. Additionally, the mutex and condition variables are destroyed.

    queue_push:
        The push function adds the arguemnt element to the back of the queue and adds one to the index of the back of the queue. This function also adds one to the int keeping track of the number of elements in the queue. 
    
        After acquiring the mutex (lock), I check if the queue is full - that is when the int keeping track of the number of elements in the queue is equal to max (size of the array (queue)). If it is full, then a condition variable realeases the mutex and waits for a signal indicating that the queue is no longer full to try to reaquire the lock. After an item is input into the array the mutex is unlocked and a signal indicating that something was pushed is broadcast (in case there are many threads waiting to pop something from the queue).

    queue_pop:
        The pop function removes an item from the queue and stores it in the elem input argument. It also increases the index of the front of the queue and subtracts one from the int keeping track of the number of elements in the queue. 

        After acquiring the lock, I check if the queue is empty - that is when the int keeping track of the number of elements in the queue is 0. If it is empty, then a condition variable causes the lock to be released and the thread waits until there is something in the queue to try to reaquire the lock. After removing an element from the queue, the lock is released and a signal indicating that somthing was removed from the queue is broadcast (in case there are more than 1 thread waiting to push something onto the queue). 
    


Sources Used:
    Lecture Slides
    Example code in Gitlab resources repository
    My code from previous classes

Files: This project contains the following files:
README.md:
    This file is the README. It contains a description of the files within the program, descriptions of the program itself, and citations to code referenced.
Makefile:
    The Makefile is used to create a ".o" file of the program. To make an ".o" file, use the command "make" or "make all" within the same directory as the Makefile. This will create the ".o" file "queue.o".
    
    The ".o" file can be removed using the command "make clean" in the same directory as the Makefile. 
queue.c:
    This file is the file that contains the code for the program. See "DESIGN" section above for a description of the program and code.
queue.h:
    Header file for queue.c
