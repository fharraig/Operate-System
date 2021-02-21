/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by:
 *
 * and
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

// struct lockent locktab[NLOCK];  /* spinlock table */

devcall putc(int dev, char c)
{
    return 0;
}


/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{

    kprintf("===TEST BEGIN===\r\n");

    kprintf("\r \nTesting Menu: \r \na = kungetc() and kcheckc() \r \ns = kcheckc() and kputc() \r \nd = kgetc() \r \nf = kputc() \r \ng = kgetc(), kungetc(), kcheckc() \r \n");

    int c;
    int q;
    int w;

    c = kgetc();

    switch (c)
    {
        // TODO: Test your operating system!
        case 'a': 
            kungetc('a');
            if (kcheckc()){ //supposed to return true
                kprintf("True. \r \n");
            } else {
                kprintf("False. \r \n");
            }
            break;
        case 's':
            if (kcheckc()){
                kputc('q');
            } else {
                kprintf("False \r \n");
            }
            break;
        case 'd':
            kprintf("Enter character ...");
            w = kgetc(); //takes in input char and prints it
            kprintf("Char = %c \r \n", w);
            break;
        case 'f': 
            kputc('t'); //prints out a t
            break;
        case 'g':
            kprintf("Enter character : \r \n");
            q = kgetc();
            kungetc(q);
            if (kcheckc()){
                kprintf("True : %c \r \n", q);
            } else {
                kprintf("False. \r \n");
            }
            break;
        case 'q':
            kprintf("Core: %i \r \n", );
    default:
        kprintf("Hello Xinu World!\r\n");
    }
 
    kprintf("\r\n===TEST END===\r\n");
    return;
}
