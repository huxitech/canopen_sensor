#include "canopen.h"
#include "led/led.h"
#include "delay/delay.h"

int main(){
	//SCB->VTOR = FLASH_BASE | 0x10000;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  delay_init(168);   
	canopen_Init(3,250000);
	
	LED_Init();
	while(1)
	{
		LED0 = !LED0;
		delay_ms(1000); 
	}
}
