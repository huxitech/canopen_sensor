  
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx.h" 
#include "stm32f4xx_can.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

// CAN bus defines for cortex-M4 STM32F373

#define CANx                       CAN1
#define CAN_CLK                    RCC_APB1Periph_CAN1
#define CAN_RX_PIN                 GPIO_Pin_11
#define CAN_TX_PIN                 GPIO_Pin_12
#define CAN_GPIO_PORT              GPIOA
#define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define CAN_AF_PORT                GPIO_AF_CAN1
#define CAN_RX_SOURCE              GPIO_PinSource11
#define CAN_TX_SOURCE              GPIO_PinSource12


#endif 

