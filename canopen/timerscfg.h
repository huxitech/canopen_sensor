/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Edouard TISSERANT and Francis DUPIN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __TIMERSCFG_H__
#define __TIMERSCFG_H__
#include <stdint.h>

#ifndef MIN
	#define MIN(a,b)  ((a)<(b) ? (a) : (b))
#endif
// Whatever your microcontroller, the timer wont work if
// TIMEVAL is not at least on 32 bits
#define TIMEVAL UNS64

extern uint32_t TickTime(void);

// using 32 bits timer
#define TIMEVAL_MAX 0xFFFFFFFF   


#define MS_TO_TIMEVAL(ms) MIN((ms*1000/TickTime()),TIMEVAL_MAX) 
#define US_TO_TIMEVAL(us) MIN((us/TickTime()),TIMEVAL_MAX) 

#define TASK_HANDLE void*

#endif
