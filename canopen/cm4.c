// Includes for the Canfestival driver
#include "canfestival.h"
#include "timer.h"
#include "data.h"
#include "cm4.h"
#include "include/bsp_port.h"
#include "include/canopen.h"

TIMEVAL last_counter_val = 0;
TIMEVAL elapsed_time = 0;

static CO_Data *co_data = NULL;

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
	last_counter_val = 65535-value;
	bsp_setTimer(65535-value);
	bsp_TimerEnable(1);
	//printf("setTimer %lu, elapsed %lu\r\n", value, elapsed_time);
}

//Return the elapsed time to tell the Stack how much time is spent since last call.
TIMEVAL getElapsedTime(void)
{
  	uint32_t timer = bsp_getTimer();        // Copy the value of the running timer
	if(timer < last_counter_val)
		timer += 65535;
	TIMEVAL elapsed = timer - last_counter_val + elapsed_time;
	//printf("elapsed %lu - %lu %lu %lu\r\n", elapsed, timer, last_counter_val, elapsed_time);
	return elapsed;
}

/**
 * @brief the timer dispatch
 * @return void
 */
void  api_TimerDispatch(void)
{
	last_counter_val = 0;
	elapsed_time = 0;
	TimeDispatch();
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


void  api_canDispatch(uint32_t cob_id,uint8_t*data,uint8_t len,uint8_t rtr)
{
	int i;
	Message rxm = {0};
	rxm.cob_id = cob_id;
  	if(rtr!=0)
		rxm.rtr = 1;
	rxm.len = len;
	for(i=0 ; i< len ; i++)
  		 rxm.data[i] = data[i];
	canDispatch(co_data, &rxm);
}

