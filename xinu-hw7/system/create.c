/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

/** 
 * COSC 3250 - Project 6
 * create process in C + priority 
 * @author Matthew Covington Alex Alarcon
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO matthew.covington@marquette.edu alex.alarcon@marquette.edu
*/

#include <arm.h>
#include <xinu.h>

/* Assembly routine for atomic operations */
extern int _atomic_increment(int *);
extern int _atomic_increment_mod(int *, int);

static pid_typ newpid(void);
void userret(void);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, ulong priority, char *name, ulong nargs,...)
{
	ulong *saddr;               /* stack address                */
	ulong pid;                  /* stores new process id        */
	pcb *ppcb;                  /* pointer to proc control blk  */
	ulong i;
	va_list ap;                 /* points to list of var args   */
	ulong pads = 0;             /* padding entries in record.   */
	void INITRET(void);

	if (ssize < MINSTK)
		ssize = MINSTK;
	ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
	/* round up to even boundary    */	
	saddr = (ulong *)getmem(ssize);     /* allocate new stack and pid   */
	pid = newpid();
	/* a little error checking      */
	if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
	{
		return SYSERR;
	}

	_atomic_increment(&numproc);

	ppcb = &proctab[pid];
	/* setup PCB entry for new proc */
	// TODO: Setup PCB entry for new process.
	
	ppcb -> state = PRSUSP; //set to suspended at first

	if (priority >= 0 && priority <= 2) { //error checking
		ppcb -> priority = priority; //initiliaze priority of the current proccess
	} else {
		return SYSERR;
	}
	
	strncpy(ppcb->name, name, PNMLEN); //max size of 16 for names
    ppcb -> stklen = ssize; //size is passed in through create
    ppcb -> stkbase = saddr; //stkbase is the address of the stack (saddr)
	ppcb -> core_affinity = -1; //default core is -1

	saddr = (ulong *) ((ulong)saddr) + ssize + 4; //?????
	
	/* Initialize stack with accounting block. */
	*saddr = STACKMAGIC;
	*--saddr = pid;
	*--saddr = ppcb->stklen;
	*--saddr = (ulong)ppcb->stkbase;

	/* Handle variable number of arguments passed to starting function   */
	if (nargs)
	{
		pads = ((nargs - 1) / 4) * 4;
	}
	/* If more than 4 args, pad record size to multiple of native memory */
	/*  transfer size.  Reserve space for extra args                     */
	for (i = 0; i < pads; i++)
	{
		*--saddr = 0;
	}

	// TODO: Initialize process context.
	// TODO:  Place arguments into activation record.
	//        See K&R 7.3 for example using va_start, va_arg and
	//        va_end macros for variable argument functions

	va_start(ap, nargs);
	int x;
	for (x = 0; x < nargs; x++) {
		if (x >= 4 ) {
			*(++saddr) = va_arg(ap, int);
		} else {
			ppcb->regs[x] = va_arg(ap, int);
		}
	}

	ppcb -> regs[PREG_SP] = saddr;
	ppcb -> regs[PREG_LR] = userret;
	ppcb -> regs[PREG_PC] = funcaddr;

	va_end(ap);

	return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
	pid_typ pid;                /* process id to return     */
	static pid_typ nextpid = 0;

	for (pid = 0; pid < NPROC; pid++)
	{                           /* check all NPROC slots    */
		//        nextpid = (nextpid + 1) % NPROC;
		_atomic_increment_mod(&nextpid, NPROC);
		if (PRFREE == proctab[nextpid].state)
		{
			return nextpid;
		}
	}
	return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
	uint cpuid = getcpuid();
	kill(currpid[cpuid]);
}
