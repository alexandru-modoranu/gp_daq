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

/* Exported routine IDs */
#define ROUTINE_ID_READ_INFO		(0x01u)

/* Routine return codes */
#define ROUTINE_OK					(0x00u)
#define ROUTINE_NOK 				(0x11u)
#define ROUTINE_BUSY				(0x22u)

/* Exported function prototypes */
uint8_t routine_read_info(uint8_t *rqData, uint8_t rqLen, uint8_t *rsData, uint8_t rsLen);
