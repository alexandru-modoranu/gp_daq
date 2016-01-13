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
#include "conf.h"
#include "util.h"

/**
 * Computes CRC8 for the given field according to CONFIG_CRC8_POLY
 * @param data Pointer to the data for computation
 * @param len Length of the data on which the CRC should be computed
 * @return Computed CRC value
 */
uint8_t util_crc8(uint8_t *data, uint8_t len)
{
	uint8_t byte, sum, i;
	uint8_t crc = 0u;

	/* Do crc stuff */
	while (len--) {
		byte = *data++;
		for(i = 8u; i > 0u; i--) {
			sum = (crc ^ byte) & 1u;
			crc >>= 1u;
			if (sum != 0u) {
				crc ^= CONFIG_CRC8_POLY;
			}
			byte >>= 1u;
		}
	}
	return crc;
}

void util_init(void)
{
	DDRC = 0xFF;
}

void util_task(void)
{
	PORTB ^= (1 << 5);
}
