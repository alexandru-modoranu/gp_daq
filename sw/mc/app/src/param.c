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
#include "util.h"

/* EEPROM Layout
 * 1b - parameter valid flag
 * 1b - uart baud rate index
 */

#define UART_BAUD(a) ((F_CPU / (16u * (uint32_t)a)) - 1u)

struct param {
	uint8_t isValid;
	uint8_t baudIndex;
};

static struct param lparam;

uint16_t param_get_baud(void)
{
	return (uint16_t)UART_BAUD(9600);
}

void param_init(void)
{

}
