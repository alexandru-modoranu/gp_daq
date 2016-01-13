/*******************************************************************************
gp_daq. A general purpose Data Aquisition System
Copyright (C) 2015  Ionut Catalin Pavel

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*******************************************************************************/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include "util.h"
#include "conf.h"
#include "diag.h"
#include "sched.h"

/* Macro for saving registers on the stack */
#define SAVE_CONTEXT()								\
	asm volatile (	"push	r0				\n\t"	\
					"in		r0, __SREG__	\n\t"	\
					"cli					\n\t"	\
					"push	r0				\n\t"	\
					"push	r1				\n\t"	\
					"clr	r1				\n\t"	\
					"push	r2				\n\t"	\
					"push	r3				\n\t"	\
					"push	r4				\n\t"	\
					"push	r5				\n\t"	\
					"push	r6				\n\t"	\
					"push	r7				\n\t"	\
					"push	r8				\n\t"	\
					"push	r9				\n\t"	\
					"push	r10				\n\t"	\
					"push	r11				\n\t"	\
					"push	r12				\n\t"	\
					"push	r13				\n\t"	\
					"push	r14				\n\t"	\
					"push	r15				\n\t"	\
					"push	r16				\n\t"	\
					"push	r17				\n\t"	\
					"push	r18				\n\t"	\
					"push	r19				\n\t"	\
					"push	r20				\n\t"	\
					"push	r21				\n\t"	\
					"push	r22				\n\t"	\
					"push	r23				\n\t"	\
					"push	r24				\n\t"	\
					"push	r25				\n\t"	\
					"push	r26				\n\t"	\
					"push	r27				\n\t"	\
					"push	r28				\n\t"	\
					"push	r29				\n\t"	\
					"push	r30				\n\t"	\
					"push	r31				\n\t"	\
				);

/* Macro for restoring register content from the stack */
#define RESTORE_CONTEXT()							\
	asm volatile (	"pop	r31				\n\t"	\
					"pop	r30				\n\t"	\
					"pop	r29				\n\t"	\
					"pop	r28				\n\t"	\
					"pop	r27				\n\t"	\
					"pop	r26				\n\t"	\
					"pop	r25				\n\t"	\
					"pop	r24				\n\t"	\
					"pop	r23				\n\t"	\
					"pop	r22				\n\t"	\
					"pop	r21				\n\t"	\
					"pop	r20				\n\t"	\
					"pop	r19				\n\t"	\
					"pop	r18				\n\t"	\
					"pop	r17				\n\t"	\
					"pop	r16				\n\t"	\
					"pop	r15				\n\t"	\
					"pop	r14				\n\t"	\
					"pop	r13				\n\t"	\
					"pop	r12				\n\t"	\
					"pop	r11				\n\t"	\
					"pop	r10				\n\t"	\
					"pop	r9				\n\t"	\
					"pop	r8				\n\t"	\
					"pop	r7				\n\t"	\
					"pop	r6				\n\t"	\
					"pop	r5				\n\t"	\
					"pop	r4				\n\t"	\
					"pop	r3				\n\t"	\
					"pop	r2				\n\t"	\
					"pop	r1				\n\t"	\
					"pop	r0				\n\t"	\
					"out	__SREG__, r0	\n\t"	\
					"pop	r0				\n\t"	\
				);


/* Number of miliseconds in a second */
#define SCHED_BASE_MILISECOND	(1000u)

/* Idle task priority */
#define SCHED_IDLE_PRIORITY		(255u)

/* Structure type for task table */
struct task {
	uint16_t elapsed;
	const uint16_t period;
	void (* const task)(void);
	uint8_t flags;
};

/* Task table data */
static struct task taskTable[] = {
	{.period = 10u, .task = &diag_receive_task, .flags = (SCHED_FLAGS_NOT_RUNNING)},
	{.period = 500u, .task = &util_task, .flags = (SCHED_FLAGS_ACTIVE | SCHED_FLAGS_NOT_RUNNING)},
	{.period = 10u, .task = &diag_task, .flags = (SCHED_FLAGS_ACTIVE | SCHED_FLAGS_NOT_RUNNING)}
};

/* Used for tracking current running task */
static uint8_t current = 0u;

/* Priority task table */
static uint8_t running[ARRAY_SIZE(taskTable) + 1u] = {255};

/**
 * Activates or deactivates execution of a task
 * @param ind Task index
 * @param val (TRUE | FALSE)
 */
void sched_set_active(uint8_t ind, uint8_t val)
{
	if(val != FALSE) {
		taskTable[ind].flags |= SCHED_FLAGS_ACTIVE;
	} else {
		taskTable[ind].flags &= ~(SCHED_FLAGS_ACTIVE);
	}
}

/**
 * Initialize TIMER1 to give a tick according to CONFIG_TICK_RATE parameter
 */
void sched_init(void)
{
	/* Initialize timer to give a tick */
#if (CONFIG_TICK_RATE <= (F_CPU / (1024u * 255u)))
	|| (CONFIG_TICK_RATE > (F_CPU / 1024u))
#error "Could not resolve tick frequency."
#else
	OCR2A = (uint8_t)(F_CPU / (1024u * CONFIG_TICK_RATE));
	TCNT2 = 0u;
	TCCR2A |= (1u << WGM21);
	TIMSK2 |= (1u << OCIE2A);
	TCCR2B |= ((1u << CS22) | (1u << CS21) | (1u << CS20));
#endif
}

/**
 * Called each time a tick from the timer is triggered
 * @param TIMER1_COMPA_vect IRQ vector
 */
ISR(TIMER2_COMPA_vect, ISR_NAKED)
{
	uint8_t i;
	struct task *tsk;

	/* Save register context in case of nesting */
	SAVE_CONTEXT();

	/* For all tasks in list */
	for (i = 0u; i < ARRAY_SIZE(taskTable); i++) {
		tsk = &taskTable[i];

		/* Check for running preconditions */
		if ((tsk->flags == (SCHED_FLAGS_ACTIVE | SCHED_FLAGS_NOT_RUNNING))
		&& (tsk->elapsed >= tsk->period)
		&& (running[current] > i)) {

			/* Clear counter, add prio in queue and mark as running */
			asm volatile("cli");
			tsk->elapsed = 0u;
			tsk->flags &= ~(SCHED_FLAGS_NOT_RUNNING);
			++current;
			running[current] = i;
			asm volatile("sei");

			/* Execute callback */
			tsk->task();

			/* Mark as not running and remove from queue */
			asm volatile("cli");
			tsk->flags |= SCHED_FLAGS_NOT_RUNNING;
			running[current] = SCHED_IDLE_PRIORITY;
			current--;
			asm volatile("sei");
		}

		/* Increment tick according to tick frequency */
		tsk->elapsed += (SCHED_BASE_MILISECOND / CONFIG_TICK_RATE);
	}

	/* Restore register context and return from IRQ */
	RESTORE_CONTEXT();
	asm volatile("reti");
}


