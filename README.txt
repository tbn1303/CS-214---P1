NAME: Thai Bao Nguyen  
RUID: 249008650  

NAME: Tommy Pham  
RUID: 225005291  

Design:
---
mymalloc.c

Each chunk has an 8-byte header containing its size and an “inuse” flag to check if the chunk has been allocated or freed.  

Whenever we initialize the heap, we clean it up and register a leak check using atexit().  

In coalesce_chunk(), we use a pointer from myfree() as the current chunk. Then we traverse the chunks forward and backward based on this pointer to check for any free chunks and coalesce them.  

In mymalloc(), we initialize the heap and split it whenever we allocate a chunk. We also print an error message if the program cannot allocate the requested number of bytes.  

In myfree(), we have two types of error handling: one for invalid pointers and one for double frees. If the program passes the checks, we mark the chunk as not in use and coalesce adjacent free chunks (if any).

Test plan:
---
First, we allocate 12 objects in the heap and fill each of them with a distinct pattern (for example, i * 2 or even numbers).  
Then, we check for overlapping. If they overlap, their values will not be consistent across objects (for example, obj[0] will have different values in each byte).  
Next, we check for chunk coalescing in our program. As observed, there were no error messages printed out, meaning that our program successfully coalesced the chunks.  
Finally, before checking for double free errors, we freed all objects in the heap and called free again on a freed object. An error message was printed, meaning that our error handling worked correctly.

Stress testing:
---
There are two optional tasks that we created:  

The first one fills the heap with 64 objects, each with 56 bytes of data plus an 8-byte header. Then we deallocate half of the objects.  
The second one fills the heap again, deallocates the first 10 objects, and reallocates those 10 objects but with smaller sizes.  
We free all objects after each task.  

After 50 trials, the results were:  
	Task 1 average time: 0.000004 seconds  
	Task 2 average time: 0.000043 seconds  
	Task 3 average time: 0.000005 seconds  
	Task 4 average time: 0.000020 seconds  
	Task 5 average time: 0.000014 seconds  

The first and third tasks were the fastest, while the second task was the slowest.  
Therefore, allocating and immediately deallocating an object is more efficient and faster than allocating all objects and freeing them later.  
Lastly, randomly choosing between allocation and deallocation is also fast but slightly more complex. However, it is still about as efficient as the first task.
