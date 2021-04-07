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
 	* - PID & Process Error Checking (isbadpid) curr/ready/recv
 	* - Deposit Message, raise flag
 	* -  If receving message is blocked, ready process
 	* - Return OK
 	*/

 	ppcb = &proctab[pid];

 	if(isbadpid(pid)){
		 return SYSERR;
	}

	if (ppcb -> state != PRCURR && ppcb -> state != PRREADY && ppcb -> state != PRRECV) {
		return SYSERR;
	}

	lock_acquire(ppcb -> msg_var.core_com_lock);

	if (ppcb -> msg_var.hasMessage == FALSE) { 
		ppcb -> msg_var.msgin = msg;
		ppcb -> msg_var.hasMessage = TRUE;
	} else {
		lock_release(ppcb -> msg_var.core_com_lock);
		return SYSERR;
	}

	lock_release(ppcb -> msg_var.core_com_lock);

	if (ppcb -> state == PRRECV) {
		ready(pid, RESCHED_YES, ppcb -> core_affinity);  
		return SYSERR;
	}
	
	return OK;
}
