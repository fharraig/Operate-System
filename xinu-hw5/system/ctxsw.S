/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */


/** 
 * COSC 3250 - Project 5
 * context swtich in ARM
 * @author Matthew Covington Alex Alarcon
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO matthew.covington@marquette.edu alex.alarcon@marquette.edu
*/

#include <arm.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldregs, &newregs)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
ctxsw:
	.func ctxsw

	// TODO: Save callee-save ("non-volatile") registers.
	// TODO: Restore callee-save ("non-volatile") registers.
	// TODO: Jump to next function. 
	
	mov ip, r0
	
	str r1, [r0, #PREG_R0 * 4]
	str r1, [r0, #PREG_R1 * 4]
	str r1, [r0, #PREG_R2 * 4]
	str r1, [r0, #PREG_R3 * 4]
	str r1, [r0, #PREG_R4 * 4]
	str r1, [r0, #PREG_R5 * 4]
	str r1, [r0, #PREG_R6 * 4]
	str r1, [r0, #PREG_R7 * 4]
	str r1, [r0, #PREG_R8 * 4]
	str r1, [r0, #PREG_R9 * 4]
	str r1, [r0, #PREG_R10 * 4]
	str r1, [r0, #PREG_R11 * 4]
	str r1, [r0, #PREG_IP * 4]
	str r1, [r0, #PREG_SP * 4]
	str r1, [r0, #PREG_LR * 4]
	str r1, [r0, #PREG_PC * 4]

	ldr r0, [ip, #PREG_R0 * 4]
	ldr r1, [ip, #PREG_R1 * 4]
	ldr r2, [ip, #PREG_R2 * 4]
	ldr r3, [ip, #PREG_R3 * 4]
	ldr r4, [ip, #PREG_R4 * 4]
	ldr r5, [ip, #PREG_R5 * 4]
	ldr r6, [ip, #PREG_R6 * 4]
	ldr r7, [ip, #PREG_R7 * 4]
	ldr r8, [ip, #PREG_R8 * 4]
	ldr r9, [ip, #PREG_R9 * 4]
	ldr r10, [ip, #PREG_R10 * 4]
	ldr r11, [ip, #PREG_R11 * 4]
	ldr ip, [ip, #PREG_IP * 4]
	ldr sp, [ip, #PREG_SP * 4]
	ldr lr, [ip, #PREG_LR * 4]
	ldr pc, [ip, #PREG_PC * 4]

	.end ctxsw
