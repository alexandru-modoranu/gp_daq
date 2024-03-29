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
#include "util.h"
#include "sched.h"
#include "diag.h"
#include "param.h"

/**
 * Relocate Vectors and init stuff then go loop forever
 */
void main(void)
{
	//MCUCR = (1u << IVCE);
	//MCUCR = (1u << IVSEL);

	DDRB = 0xFF;

	param_init();
	diag_init();
	sched_init();
	sei();

	while(TRUE);
}
