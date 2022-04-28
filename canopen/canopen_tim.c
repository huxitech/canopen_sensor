// Includes for the Canfestival driver
#include "canfestival.h"
#include "timer.h"
#include "data.h"
#include "include/bsp_port.h"
#include "include/canopen.h"

TIMEVAL last_counter_val = 0;
TIMEVAL elapsed_time = 0;
CO_Data *co_data = NULL;

extern uint32_t TickTime(void);

// Initializes the timer, turn on the interrupt and put the interrupt time to zero
void initTimer(void)
{
	bsp_TimerOpen();
}

//Set the timer for the next alarm.
void setTimer(TIMEVAL value)
{
  uint32_t timer = bsp_getTimer();        // Copy the value of the running timer
	elapsed_time += timer - last_counter_val;
	last_counter_val = TickTime()-value;
	bsp_setTimer(TickTime()-value);
	bsp_TimerEnable(1);
	//printf("setTimer %lu, elapsed %lu\r\n", value, elapsed_time);
}

//Return the elapsed time to tell the Stack how much time is spent since last call.
TIMEVAL getElapsedTime(void)
{
  	uint32_t timer = bsp_getTimer();        // Copy the value of the running timer
	if(timer < last_counter_val)
		timer += TickTime();
	TIMEVAL elapsed = timer - last_counter_val + elapsed_time;
	//printf("elapsed %lu - %lu %lu %lu\r\n", elapsed, timer, last_counter_val, elapsed_time);
	return elapsed;
}



//Initialize the CAN hardware 
unsigned char canInit(CO_Data * d, uint32_t bitrate)
{
	/* save the canfestival handle */  
	co_data = d;

	return bsp_CanOpen(bitrate);
}

// The driver send a CAN message passed from the CANopen stack
char canSend(CAN_PORT notused, Message *m)
{
	return bsp_CanSend(m->cob_id,(uint8_t*)m->data,m->len);
}
