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
    lock_acquire(freelist[cpuid].memlock);

    curr = freelist[cpuid].head;
    
    while (curr -> next != NULL) {
        if (curr -> length < nbytes){
            *prev = *curr;
            *curr = *curr -> next;
        } else if (curr -> length == nbytes) {
            lock_release(freelist[cpuid].memlock);
            return (void *)curr;
        } else if (curr -> length > nbytes) { 
            left = curr -> length - nbytes;
            leftover -> length = left;
            *leftover -> next = *curr -> next;
            *curr -> next = *curr;
            *prev -> next = *leftover;
            lock_release(freelist[cpuid].memlock);
            return (void *)curr;
        } else {
            kprintf("how did you get here \r\n");
        }
    }
    

    restore(im);
    return (void *)SYSERR;
}
