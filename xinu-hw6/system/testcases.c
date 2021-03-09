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

void printpid(int times)
{
    int i = 0;
    uint cpuid = getcpuid();

    enable();
    for (i = 0; i < times; i++)
    {
        kprintf("This is process %d\r\n", currpid[cpuid]);
        udelay(1);
    }
}

void infinite(){
    int x = 0;
    while (x < 10000){
        kprintf("Hello \r \n");
        x++;
    }
}

void infinite1(){
    int y = 0;
    while (y < 2000){
        kprintf("Goodbye \r \n");
        y++;
    }
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    uchar c;

    kprintf("===TEST BEGIN===\r\n");
    kprintf("0) Test priority scheduling\r\n");
    kprintf("\'A\') Aging / Starvation testcase\r\n");
    kprintf("\'P\') Preemption testcase\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
    case '0':   
        // Run 3 processes with varying priorities
        ready(create
              ((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-A", 1,
               5), RESCHED_NO, 0);
        ready(create
              ((void *)printpid, INITSTK, PRIORITY_MED, "PRINTER-B", 1,
               5), RESCHED_NO, 0);
        ready(create
              ((void *)printpid, INITSTK, PRIORITY_LOW, "PRINTER-C", 1,
               5), RESCHED_YES, 0);

        break;

    case 'a':
    case 'A':
#if AGING
        // AGING TESTCASE
        kprintf("AGING is enabled.\r\n");

        // TODO: Create a testcase that demonstrates aging.

        //expected output: everything switching around and running normally
        ready(create((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-Z", 1, 5), RESCHED_NO, 0);      //4
        ready(create((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-A", 1, 5), RESCHED_NO, 0);      //5
        ready(create((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-B", 1, 5), RESCHED_NO, 0);      //6
        ready(create((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-C", 1, 5), RESCHED_NO, 0);      //7
        ready(create((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-D", 1, 5), RESCHED_NO, 0);      //8 
        ready(create((void *)printpid, INITSTK, PRIORITY_LOW, "PRINTER-E", 1, 5), RESCHED_YES, 0);      //9

#else
        // STARVING TESTCASE
        kprintf("\r\nAGING is not currently enabled.\r\n");

        // TODO: Create a testcase that demonstrates starvation

        //expected output: high priority items dominating the queue and not letting the low run
        ready(create((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-Z", 1, 5), RESCHED_NO, 0);      //4
        ready(create((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-A", 1, 5), RESCHED_NO, 0);      //5
        ready(create((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-B", 1, 5), RESCHED_NO, 0);      //6
        ready(create((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-C", 1, 5), RESCHED_NO, 0);      //7
        ready(create((void *)printpid, INITSTK, PRIORITY_HIGH, "PRINTER-D", 1, 5), RESCHED_NO, 0);      //8 
        ready(create((void *)printpid, INITSTK, PRIORITY_LOW, "PRINTER-E", 1, 5), RESCHED_YES, 0);      //9

#endif
        break;

    case 'p':
    case 'P':
#if PREEMPT
        // PREEMPTION TESTCASE
        kprintf("\r\nPreemption is enabled.\r\n");

        // TODO: Create a testcase that demonstrates preemption

        //expected output: infinite loop runs but is eventually stopped and the other process is allowed to run.
        ready(create((void *)infinite, INITSTK, PRIORITY_HIGH, "Test1", 1, 5), RESCHED_NO, 0);
        ready(create((void *)infinite1, INITSTK, PRIORITY_HIGH, "Test2", 1, 5), RESCHED_YES, 0);

#else
        kprintf("\r\nPreemption is not currently enabled...\r\n");
#endif
        break;

    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
