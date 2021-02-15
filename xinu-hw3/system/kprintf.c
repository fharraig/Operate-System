/**
 * @file kprintf.c
 */

/** 
 * COSC 3250 - Project 3
 * kprintf implementation in the embedded xinu OS
 * @author Matthew Covington Alex Alarcon
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO matthew.covington@marquette.edu alex.alarcon@marquette.edu
*/

/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */

static unsigned char ungetArray[UNGETMAX];

/**
 * Synchronously read a character from a UART.  This blocks until a character is
 * available.  The interrupt handler is not used.
 *
 * @return
 *      The character read from the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */

int counter = 0;
syscall kgetc(void)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: First, check the unget buffer for a character.
    //       Otherwise, check UART flags register, and
    //       once the receiver is not empty, get character c.

    
    while (counter <= UNGETMAX){
        if (ungetArray[counter]=='\0'){
            break;
        }
        if (ungetArray[counter + 1] == '\0') {
            counter = 0;
            return (int) ungetArray[counter];
        }
        counter++;
    }

    while (regptr -> fr & PL011_FR_RXFE != 1){
        return (int) regptr -> dr;
    }

    return SYSERR;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
int newcount = 0;
syscall kcheckc(void)
{
    volatile struct pl011_uart_csreg *regptr;
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: Check the unget buffer and the UART for characters.
    
    if (regptr -> dr & PL011_FR_TXFE != 1){
        return 1;
    } else {
        return 0;
    }

    return SYSERR;
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
    // TODO: Check for room in unget buffer, put the character in or discard.

    int size = strlen(ungetArray);
    if (size != 10) {
        if (ungetArray[size] == '\0'){
            ungetArray[size] = c;
            return c;
        }
    }
    return SYSERR;
}

/**
 * Synchronously write a character to a UART.  This blocks until the character
 * has been written to the hardware.  The interrupt handler is not used.
 *
 * @param c
 *      The character to write.
 *
 * @return
 *      The character written to the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */
syscall kputc(unsigned char c)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    // TODO: Check UART flags register.
    //       Once the Transmitter FIFO is not full, send character c.

    while(regptr -> fr & PL011_FR_TXFF != 1) {
        regptr -> dr = (int) c;
    }

    return SYSERR;
}

/**
 * kernel printf: formatted, synchronous output to SERIAL0.
 *
 * @param format
 *      The format string.  Not all standard format specifiers are supported by
 *      this implementation.  See _doprnt() for a description of supported
 *      conversion specifications.
 * @param ...
 *      Arguments matching those in the format string.
 *
 * @return
 *      The number of characters written.
 */
syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
    va_end(ap);
    return retval;
}
