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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "routine.h"
#include "conf.h"
#include "util.h"
#include "param.h"
#include "sched.h"

/* Offset to be added to the current sid in case the routine completed with success */
#define SID_RQ_VALID_OFFSET		(0x40u)

/* Id of the response in case a error occured */
#define SID_ERR_RESPONSE		(0xEFu)

/* Error code in case received CRC was wrong */
#define SID_ERR_FRAMING			(0x13u)

/* Error code in case the requested routine is not available */
#define SID_FUNC_NOT_SUPPORTED	(0x12u)

/* Error code in case the routine returned NOK */
#define SID_INCORRECT_PARAM		(ROUTINE_OK)

/* Error code in case received data length doesn't match the requested routine length*/
#define SID_RX_WRONG_LENGTH		(0x14u)

/* Error code in case the routine is busy executing */
#define SID_ROUTINE_BUSY		(ROUTINE_BUSY)

/* Error response length */
#define SID_ERROR_LENGTH		(3u)

/* Structure type for sid data */
struct sid {
	const uint8_t id, lrq, lrs;
	uint8_t (* const cback)(uint8_t *rqData, uint8_t rqLen, uint8_t *rsData, uint8_t rsLen);
};

/* Sid table */
static struct sid sidTable[] = {
	{.id = ROUTINE_ID_READ_INFO, .lrq = 0u, .lrs = 2u, .cback = &routine_read_info}
};

/* Diagnostics buffer */
static uint8_t diagBuf[CONFIG_DIAG_BUF_LEN];

/* Receive data index */
static uint8_t rxInd;

/* Transmit data index */
static uint8_t txInd;

/* Receive timeout */
static uint8_t tOut;

/**
 * Called at init. Used for initalizing the UART
 */
void diag_init(void)
{
	/* Init UART */
	DDRD |= (1u < DDD2);
	UBRR0 = param_get_baud();
	UCSR0C |= ((1u << UCSZ01) | (1u << UCSZ00));
	UCSR0B |= ((1u << RXCIE0) | (1u << TXCIE0) | (1u << RXEN0) | (1u << TXEN0));
}

/**
 * Called each 10ms.
 */
void diag_task(void)
{
	/* Check receive timeout */
	if (tOut > 0u) {
		tOut--;
	} else {
		rxInd = 0u;
	}
}

/**
 * Execution triggered when buffer received expecting values,
 * terminated after one execution
 */
void diag_receive_task(void)
{
	uint8_t i, len, rid, err;
	struct sid *ref = NULL;

	/* Clear all indexes */
	rxInd = 0u;
	txInd = 0u;
	len = diagBuf[0u];

	/* Check for correct message parameters then execute routine,
	 * if parameters are incorrect respond with error code */
	if (diagBuf[len] != util_crc8(&diagBuf[1u], (len - 1u))) {
		err = SID_ERR_FRAMING;
	} else {
		for (i = 0u; i < ARRAY_SIZE(sidTable); i++) {
			if (sidTable[i].id == diagBuf[1u]) {
				ref = &sidTable[i];
				break;
			}
		}
		if (ref != NULL) {
			if (ref->lrq != (len - 2u)) {
				err = SID_RX_WRONG_LENGTH;
			} else {
				err = ref->cback(&diagBuf[2u], ref->lrq, &diagBuf[2u], ref->lrs);
				len = ref->lrs + 2u;
				rid = ref->id + SID_RQ_VALID_OFFSET;
			}
		} else {
			err = SID_FUNC_NOT_SUPPORTED;
		}
	}

	/* If there was no error leave the
	 * message as written by routine */
	if (err != 0u) {
		len = SID_ERROR_LENGTH;
		rid = SID_ERR_RESPONSE;
		diagBuf[2u] = err;
	}

	/* Update buffer values and start transmission */
	diagBuf[0u] = len;
	diagBuf[1u] = rid;
	diagBuf[len] = util_crc8(&diagBuf[1u], (len - 1u));
	UDR0 = diagBuf[txInd++];

	/* Deactivate ourselves */
	sched_set_active(TASK_ID_DIAG_RECEIVE, FALSE);
}

/**
 * Called when a charecter is received on UART
 * @param USART_RX_vect IRQ vector
 */
ISR(USART_RX_vect)
{
	/* Reset timeout counter and push value in buffer */
	tOut = (CONFIG_CHAR_TIMEOUT / 10u);
	if (rxInd < (CONFIG_DIAG_BUF_LEN - 1u)) {
		diagBuf[rxInd++] = UDR0;
	}

	/* Activate processing task in case the buffer length is
	 * correct. Deactivate receiver also */
	if (diagBuf[0u] < rxInd) {
		UCSR0B &= ~(1u << RXEN0);
		sched_set_active(TASK_ID_DIAG_RECEIVE, TRUE);
	}
}

/**
 * Called when the last send request was successfull
 * @param USART_TX_vect IRQ vector
 */
ISR(USART_TX_vect)
{
	/* Fetch new value from buffer and send it ,
	 * reactivate the receiver on complete*/
	if (txInd <= diagBuf[0u]) {
		UDR0 = diagBuf[txInd++];
	} else {
		UCSR0B |= (1u << RXEN0);
	}
}
