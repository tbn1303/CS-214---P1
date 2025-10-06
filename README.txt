NAME: Thai Bao Nguyen
RUID: 249008650

Design:
---
mymalloc.c
	
	Each chunk has a header of 8-byte containing size and inuse to check if chunk has been allocated or freed.
	
	Anytime we initialize heap, we clean up the heap and check for leak atexit().

	In my coalesce_chunk(), I include a pointer from my free as our current chunk. Then I traverse the chunk base on this pointer forward and backward to
		check for any free chunk and coalesce them.

	In mymalloc(), I initialize heap and split the heap whenever I allocate a chunk. 
		Also, I place a message if the program cannot allocate the requested bytes from user.

	In myfree(), I have two error handling: one for invalid pointer and one for double free.
		If the program pass the check, I will mark chunk inuse and coalesce chunks (if any).

Test plan:
---
	First, I allocate 12 objects into heap and fill each of them with distinct pattern (e.g. i * 2, even number).
	Then, I check for overlapping. If they overlap, their value will not consistent in each object (e.g. obj[0] will have different value in each byte).
	Next, I check for coalesce chunk in my program. As observe, there were no error message print out, meaning that my program has coalesced chunks succesfully.
	Finally, before checking for double free error, I freed all object in heap and call another free in freed object. We have an error printed out meaning that
		my error handling was successed.

Stress testing:
---
	There are two optional tasks that I make:
		The first one is I fill the heap with 64 object each with 56-byte + 8-byte header. Then I deallocate half of the object.
		The second one is I fill the heap again. Then I deallocate the first 10 objects and allocate that 10 object again but with smaller size.
	I free all object after each task.

	After 50 tries, the results are:
		Task 1 avg time: 0.000004 secs
		Task 2 avg time: 0.000043 secs
		Task 3 avg time: 0.000005 secs
		Task 4 avg time: 0.000020 secs
		Task 5 avg time: 0.000014 secs
	The first task and the third one is the fastest one while the second task is the slowest one.
		Therefore, allocate and deallocate immediately and object is efficient and faster than allocate all object and free them later.
	Last but not least, randomly choosing between allocate or deallocate is also fast but quite complicated. However, it is as fast and efficient as the first task.
