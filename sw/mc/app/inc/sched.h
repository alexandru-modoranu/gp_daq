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

/* Task active flag */
#define SCHED_FLAGS_ACTIVE			(0x02u)

/* Task running flag */
#define SCHED_FLAGS_NOT_RUNNING		(0x01u)

/* Exported task indexes */
#define TASK_ID_DIAG				(1u)
#define TASK_ID_DIAG_RECEIVE		(0u)

/* Exported function prototypes */
void sched_init(void);
void sched_set_active(uint8_t ind, uint8_t val);
