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

    kprintf("\r \n Testing Menu: \r \na = kungetc and kcheckc (TRUE) \r \ns = kcheckc (FALSE) \r \nd = kgetc() \r \nf = kputc() \r \nb = ??? \r \n");

    int c;
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
                kprintf("True. \r \n");
            } else {
                kprintf("False. \r \n");
            }
            break;
        case 'd':
            w = kgetc();
            kprintf("Char = %c \r \n", w);
            break;
        case 'f': 
            kputc('t');
            break;
        case 'b': 
            kprintf("Hello there. \r \n");
            break;
    default:
        kprintf("Hello Xinu World!\r\n");
    }
 
    kprintf("\r\n===TEST END===\r\n");
    return;
}
