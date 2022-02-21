// Includes for the Canfestival driver
#include "canfestival.h"
#include "timer.h"
#include "data.h"
#include "cm4.h"
#include "include/bsp_port.h"
#include "include/canopen.h"

TIMEVAL last_counter_val = 0;
TIMEVAL elapsed_time = 0;

CO_Data *co_data = NULL;


#if 1
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

#else
	// Initializes the timer, turn on the interrupt and put the interrupt time to zero
void initTimer(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Compute the prescaler value */
	/* SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f37x.c file */
	uint16_t PrescalerValue = (uint16_t) (SystemCoreClock  / 100000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ClearITPendingBit(TIM3, TIM_SR_UIF);

	/* TIM3 disable counter */
	TIM_Cmd(TIM3, DISABLE);

	/* Preset counter for a safe start */
	TIM_SetCounter(TIM3, 1);

	/* TIM Interrupts enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

//Set the timer for the next alarm.
void setTimer(TIMEVAL value)
{
  	uint32_t timer = TIM_GetCounter(TIM3);        // Copy the value of the running timer
	elapsed_time += timer - last_counter_val;
	last_counter_val = 65535-value;
	TIM_SetCounter(TIM3, 65535-value);
	TIM_Cmd(TIM3, ENABLE);
	//printf("setTimer %lu, elapsed %lu\r\n", value, elapsed_time);
}

//Return the elapsed time to tell the Stack how much time is spent since last call.
TIMEVAL getElapsedTime(void)
{
  	uint32_t timer = TIM_GetCounter(TIM3);        // Copy the value of the running timer
	if(timer < last_counter_val)
		timer += 65535;
	TIMEVAL elapsed = timer - last_counter_val + elapsed_time;
	//printf("elapsed %lu - %lu %lu %lu\r\n", elapsed, timer, last_counter_val, elapsed_time);
	return elapsed;
}

// This function handles Timer 3 interrupt request.
void TIM3_IRQHandler(void)
{
	//printf("--\r\n");
	if(TIM_GetFlagStatus(TIM3, TIM_SR_UIF) == RESET)
		return;
	last_counter_val = 0;
	elapsed_time = 0;
	TIM_ClearITPendingBit(TIM3, TIM_SR_UIF);
	TimeDispatch();
}


#endif




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
