/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:	
 *
 * TA-BOT:MAILTO 
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

/* Function prototypes */
void print_sem(semaphore sem);
uchar getc(void);
void putc(uchar);


/* Test process to use for testing semaphores. */
process testSemWait(semaphore sem)
{
    wait(sem);
    printf("process %d after wait()\r\n", currpid[getcpuid()]);
    return 0;
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    uchar c;
    semaphore testsem;

    char w;

    enable();

    printf("===TEST BEGIN===\r\n");
    
	// TODO: Test your operating system!
    c = getc();
    switch (c)
    {

        case 'a':
        case 'A':
            putc('H');
            putc('E');
            putc('L');
            putc('L');
            putc('O');
            break;
        case 's':
        case 'S':
           while ((w = getc()) != 'z') {
               printf("not z \r\n");
           }
            break;
        case 'd':
        case 'D':
            printf("Enter character . . .  \r\n");
            w = getc();
            putc(w);
            break;
        case 'f':
        case 'F':
            while (w != 'q'){
                w = getc();
                putc(w);
            }
            break;

    default:
        break;
    }

    while (numproc > 5)  // Four null procs plus this test proc.
    {
        resched();
    }

    printf("\r\n===TEST END===\r\n");
    printf("\r\n\r\nAll user processes have completed.\r\n\r\n");

    while (1);
    return;
}

void print_sem(semaphore sem)
{
    struct sement *semptr;
    semptr = &semtab[sem];

    printf("\r\n%d->state: %s\r\n", sem,
           (semptr->state == 0x01) ? "SFREE" : "SUSED");
    printf("%d->count: %d\r\n", sem, semptr->count);
    printf("%d->queue: %s\r\n\r\n", sem,
           (isempty(semptr->queue)) ? "EMPTY" : "NONEMPTY");
}
