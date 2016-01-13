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

/* Rate at which the scheduler ticks, in Hz */
#define CONFIG_TICK_RATE	(100u)

/* CRC polynomial used for UART data integrity checks, CRC8 */
#define CONFIG_CRC8_POLY	(0xABu)

/* CRC polynomial used for application validation, CRC32 */
#define CONFIG_CRC32_POLY	(0xEDB88320u)

/* Diagnostics buffer length, (fill one page + address + len + control) */
#define CONFIG_DIAG_BUF_LEN	(136u)

/* Timeout in ms for receive data */
#define CONFIG_CHAR_TIMEOUT	(500u)

/* Default baud rate in case parameters invalid */
#define CONFIG_DEFAULT_BAUD	(19200u)
