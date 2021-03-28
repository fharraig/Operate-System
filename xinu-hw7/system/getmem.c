/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Allocate heap memory.
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(ulong nbytes)
{
    register memblk *prev, *curr, *leftover;
    irqmask im;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);

    im = disable();

	/* TODO:
     *      - Use cpuid to use correct freelist
     *           ex: freelist[cpuid]
     *      - Acquire memory lock (memlock)
     *      - Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with simple compaction)
     *      - Release memory lock
     *      - return memory address if successful
     */

    int cpuid = getcpuid(); 
    int left;
    int free;

    lock_acquire(freelist[cpuid].memlock);

    curr = freelist[cpuid].head; //head points to the first block in the freelist
    prev = NULL;
    
    while (curr != NULL) {
        if (curr -> length < nbytes) { //not enough space, keeps looping to find spot where there is enough spcae
            prev = curr;
            curr = curr -> next;
        } else if (curr -> length >= nbytes) { //sets accounting info of leftover memblock to act as new curr, returns old curr to be changed in malloc
            left = curr -> length - nbytes; //decrements length of the current memblock
            free = freelist[cpuid].length - nbytes; //decrements overall length of the freelist

            leftover = curr + (nbytes/8); //set the address of the leftover memory block, divided by 8 to convert bytes to bits
            leftover -> next = curr -> next; //takes the place of curr, so has the same next
            leftover -> length = left; //leftover memory after nbytes are taken away from it

            if (left <= 0) //if there isnt any space left in the current block, moves onto the next block for the next loop 
                leftover = curr -> next;

            freelist[cpuid].length = free; //leftover overall memory after nbytes are taken out

            if (prev == NULL) //if the memory block is the first in the list, sets the freelist.head to the new memblock, keeping prev as null for future runs
                freelist[cpuid].head = leftover;
            else  //otherwise, puts the new memblock after the previous one
                prev -> next = leftover; 

            lock_release(freelist[cpuid].memlock);
            return curr;   //curr doesnt have to be modified in this function because it gets the necessary accounting info from malloc
        } else {
            kprintf("how did you even get here \r\n");
            return (void *)SYSERR;
        }
    }
    
    lock_release(freelist[cpuid].memlock);

    restore(im);
    return (void *)SYSERR;
}
