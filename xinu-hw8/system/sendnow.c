/**
 * @file sendnow.c
 * @provides sendnow.
 *
 */
/* Embedded Xinu, Copyright (C) 2020.   All rights reserved. */

#include <xinu.h>

/**
 * Send a message to anoher thread
 * @param pid proc id of recipient
 * @param msg contents of message
 * @return OK on sucess, SYSERR on failure
 */

syscall sendnow(int pid, message msg)
{
	register pcb *ppcb;

	/* TODO:
 	* - Acquire Lock and release when appropriate
 	* - PID & Process Error Checking (isbadpid)
 	* - Deposit Message, raise flag
 	* -  If receving message is blocked, ready process
 	* - Return OK
 	*/

 	ppcb = &proctab[currpid[pid]];
	struct pmessage *newmsg = ppcb -> msg_var;

 	if(isbadpid(pid)){
		 return SYSERR;
	}

	lock_acquire(newmsg -> core_com_lock);

	if (newmsg -> hasMessage == TRUE) {
		lock_release(newmsg -> core_com_lock);
		return SYSERR;
	} else {
		newmsg -> msgin = msg;
		newmsg -> hasMessage = TRUE;
	}

	lock_release(newmsg -> core_com_lock);

	if (ppcb -> state == PRRECV){
		ppcb -> state = PRREADY;
		ready(pid, RESCHED_YES, ppcb -> core_affinity);
	}

	return OK;
}
