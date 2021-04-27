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
    i = 0;

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

    //if freeblk null
    //if freeblk full
    //if freeblk ok

    wait(psuper -> sb_freelock);
    freeblk = psuper->sb_freelst;

    if (freeblk == NULL) { //create new freelist if freelst = null
        freeblk = malloc(sizeof(struct freeblock));
        freeblk -> fr_blocknum = block;
        freeblk -> fr_count = 0;
        freeblk -> fr_next = NULL;

        seek(diskfd, block);
        if (write(diskfd, psuper, sizeof(struct freeblock)) == SYSERR){
            signal(psuper -> sb_freelock);
            return SYSERR;
        }

        //honestly i have no idea what this does but its in getblock so i added it in lol
        free2 = freeblk;
        swizzle = psuper -> sb_dirlst;
        psuper -> sb_dirlst = (struct dirblock *)swizzle -> db_blocknum;
        psuper -> sb_freelst = (struct freeblock *) block;
        
        seek (diskfd, psuper -> sb_blocknum);
        if (write(diskfd, psuper, sizeof(struct superblock)) == SYSERR){
            signal(psuper -> sb_freelock);
            return SYSERR;
        }   
        psuper -> sb_freelst = free2;
        psuper -> sb_dirlst = swizzle;

        signal(psuper -> sb_freelock);
        return OK;
    }

    //otherwise, loop through until a reasonable spot is found
    while (freeblk -> fr_next != NULL) {
        freeblk = freeblk -> fr_next;
    }

    if (freeblk -> fr_count >= DISKBLOCKTOTAL) {//if freelist is full, make a new freelist
        free2 = malloc(sizeof(struct freeblock));
        free2 -> fr_blocknum = block;
        free2 -> fr_count = 0;
        free2 -> fr_next = NULL;

        seek(diskfd, block);
        if (write(diskfd, free2, sizeof(struct freeblock)) == SYSERR){
            signal(psuper -> sb_freelock);
            return SYSERR;
        }

    } else { //otherwise, toss it in at the end
        freeblk -> fr_free[freeblk -> fr_count] = block;
        freeblk -> fr_count++;
    }

    //update this freeblock record on disk, stolen from getblock
    free2 = freeblk->fr_next;
    if (NULL == freeblk->fr_next) {
        freeblk->fr_next = 0;
    } else {
        freeblk->fr_next = (struct freeblock *)freeblk->fr_next->fr_blocknum;
    }
    seek(diskfd, freeblk->fr_blocknum);
    if (SYSERR == write(diskfd, freeblk, sizeof(struct freeblock))) {
        signal(psuper -> sb_freelock);
        return SYSERR;
    }
    freeblk->fr_next = free2;

    signal(psuper -> sb_freelock);
    return OK;
}
