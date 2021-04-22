/* fileDelete.c - fileDelete */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <kernel.h>
#include <memory.h>
#include <file.h>

/*------------------------------------------------------------------------
 * fileDelete - Delete a file.
 *------------------------------------------------------------------------
 */
devcall fileDelete(int fd)
{
    // TODO: Unlink this file from the master directory index,
    //  and return its space to the free disk block list.
    //  Use the superblock's locks to guarantee mutually exclusive
    //  access to the directory index.

    syscall result;

    if ((NULL == supertab) || (NULL == filetab) || (isbadpfd(fd)) || (FILE_FREE == filetab[fd].fn_state)) {
        return SYSERR;
    }

    if ((fd < 0) || (fd >= DISKBLOCKTOTAL)){
        return SYSERR;
    }

     if (wait(supertab -> sb_dirlock) != OK){
        return SYSERR;
    }

    result = sbFreeBlock(supertab, supertab -> sb_dirlst -> db_fnodes[fd].fn_blocknum); 

    if (result != OK){
        return SYSERR;
    }

    supertab -> sb_dirlst -> db_fnodes[fd].fn_state = FILE_FREE; 
    supertab -> sb_blocktotal--;

    seek(DISK0, supertab -> sb_dirlst -> db_fnodes[fd].fn_blocknum);

    result = write(DISK0, supertab -> sb_dirlst, sizeof(struct dirblock));

    if (result != OK) {
        return SYSERR;
    }

    signal(supertab -> sb_dirlock);
    
    return OK;
    
}
