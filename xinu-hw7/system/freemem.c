/**
 * @file freemem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with memget().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to memget().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, ulong nbytes)
{
    register struct memblock *block, *next, *prev;
    irqmask im;
    //ulong top;

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < (ulong)memheap)
        || ((ulong)memptr > (ulong)platform.maxaddr))
    {
        return SYSERR;
    }

    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

    im = disable();

	/* TODO:
     *      - Determine correct freelist to return to
     *        based on block address
     *      - Acquire memory lock (memlock)
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent 
     */


    //check if memory blocks are next to each other, combine if they are
    //check top of prev and bottom or curr || top of curr and bottom of next

    int cpuid;
    int i;

    //find the cpuid to be used 
    for (i = 0; i < 4; i++) {
        if ((ulong)(memptr) >= freelist[i].base && (ulong)(memptr) < freelist[i].base + freelist[i].bound) {
            cpuid = i;
        } 
        break;
    }

    lock_acquire(freelist[cpuid].memlock);
      
    next = freelist[cpuid].head;
    prev = NULL;

    //scan through the freelist and find the place where the block goes
    while (next != NULL && next < block) {
        prev = next;
        next = next -> next;
    }   

    //put block in-between prev and next
    block -> length = nbytes;
    block -> next = next;

    //checks to see if the block was placed in the first spot, if yes, sets it equal to head for future use
     if (prev == NULL)
        freelist[cpuid].head = block;
    else //otherwise, sets it after previous 
        prev -> next = block;

    //check for coalescence with prev block, combine the two if yes, wouldnt even have to check if the prev is still null (i.e. block is in head (first) spot)
    if (prev != NULL) {
        if ((prev + (prev -> length / 8)) >= block) { //if the address of the prev is overlapping the address of the block, combine the two
            prev -> length += block -> length;
            prev -> next = next; //takes out block, just keeps previous but with added length 
        }
    }

    //check for coalescence with next block, combine the two if yes, doesnt even have to check if next is null (i.e. block is in last spot)
    if (next != NULL){
        if ((block + (block -> length / 8)) >= next) { //if the address of the block is overlapping the address of the next, combine the two
            block -> next = next -> next;
            next -> length += block -> length; //takes out block, just keeps next, but with added length
        }
    }

    freelist[cpuid].length += nbytes; //increments the overall length of the freelist by nbytes

    lock_release(freelist[cpuid].memlock);
	restore(im);
    return OK;
}
