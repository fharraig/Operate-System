/**
 * @file free.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup libxc
 *
 * Attempt to free a block of memory based on malloc() accounting information
 * stored in preceding two words.
 *
 * @param ptr
 *      A pointer to the memory block to free.
 */
syscall free(void *ptr)
{
    struct memblock *block;

    ulong length;

	/* TODO:
     *      1) set block to point to memblock to be free'd (ptr)
     *      2) find accounting information of the memblock
     *      3) call freemem on the block with its length
     */

    block = (struct memblock *)ptr; //cast ptr to a memblock
    length = block -> length; //find length of the memblock to be freed

    if (length == 0 || block -> next != block) //basic error checking to make sure malloc worked properly
        return SYSERR;
    
    freemem(block, length);

    return OK;
}
