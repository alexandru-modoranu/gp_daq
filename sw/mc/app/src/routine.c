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

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/boot.h>
#include "routine.h"
#include "util.h"

/* Major version for the bootloader */
#define APP_MAJOR_VERSION	(1u)

/* Minor version of the bootloader */
#define APP_MINOR_VERSION 	(1u)

/* A one in this field means that the returned info is for the bootloader */
#define FBL_ACTIVE			(1u)

/**
 * Reads the software version
 * @param rqData Incoming data pointer
 * @param rqLen Incoming data length
 * @param rsData Response data pointer
 * @param rsLen Response data length
 * @return (ROUTINE_OK/ROUTINE_NOK/ROUTINE_PENDING)
 */
uint8_t routine_read_info(uint8_t *rqData, uint8_t rqLen, uint8_t *rsData, uint8_t rsLen)
{
	(void)rqData;
	(void)rqLen;
	(void)rsLen;

	rsData[0u] = APP_MAJOR_VERSION;
	rsData[1u] = APP_MINOR_VERSION;

	return ROUTINE_OK;
}
