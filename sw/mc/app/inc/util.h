/*******************************************************************************
my_oven. A system for homemade reflow ovens.
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

/* Platform defines */
#define TRUE				(1u)
#define FALSE				(0u)

#ifndef NULL
#define NULL				(void *)0
#endif

/* Utility macros */
#define ARRAY_SIZE(arr)		(sizeof(arr) / sizeof(arr[0u]))

/* Exported functions */
uint8_t util_crc8(uint8_t *data, uint8_t len);
void util_task(void);
void util_init(void);
