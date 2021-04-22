/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*        Matthew Covington                                                         */
/* and                                                             */
/*        Alex Alarcon                                                      */
/*                                                                 */

/** 
 * COSC 3250 - Project 10
 * sbFreeBlock implementation
 * @author Matthew Covington Alex Alarcon
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO matthew.covington@marquette.edu alex.alarcon@marquette.edu
*/


#include <kernel.h>
#include <device.h>
#include <memory.h>
#include <disk.h>
#include <file.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
devcall sbFreeBlock(struct superblock *psuper, int block)
{
    // TODO: Add the block back into the filesystem's list of
    //  free blocks.  Use the superblock's locks to guarantee
    //  mutually exclusive access to the free list, and write
    //  the changed free list segment(s) back to disk.

    struct freeblock *freeblk, *free2;
    struct dirblock *swizzle;
    struct dentry *phw;
    int result, i;
    int diskfd;

    if (NULL == psuper){
        return SYSERR;
    }

    if (block < 0 || block >= DISKBLOCKTOTAL){
        return SYSERR;
    }

    phw = psuper->sb_disk;

    if (NULL == phw) {
        return SYSERR;
    }

    diskfd = phw - devtab;
    freeblk = psuper->sb_freelst;
    
    return SYSERR;
}
