#include "bsp_port.h"
#include "canopen.h"
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"

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



/**
 * @brief Send a CAN message
 * @param cob_id cob_id
 * @param data data
 * @param len data len
 * @return 0 if succes
 */
uint8_t bsp_CanSend(uint32_t cob_id,uint8_t*data,uint8_t len){
	int i, res;
	CanTxMsg TxMessage = {0};
	TxMessage.StdId = cob_id;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = len;
	for(i=0 ; i<  len ; i++)
		TxMessage.Data[i] = data[i]; 
    res = CAN_Transmit(CANx, &TxMessage);
	if(res == CAN_TxStatus_NoMailBox)
		return 0; 	// error
    return 1;		// succesful	
}

/* prescaler values for 87.5%  sampling point (88.9% at 1Mbps)
   if unknown bitrate default to 50k
*/
uint16_t brp_from_birate(uint32_t bitrate)
{
	if(bitrate == 10000)
		return 96;
	if(bitrate == 50000)
		return 48;
	if(bitrate == 125000)
		return 24;
	if(bitrate == 250000)
		return 12;
	if(bitrate == 500000)
		return 6;
	//default 1000000
	return 3;
}


/**
 * @brief Open a CANOpen device
 * @param baud the baudRate
 * @return 0 if succes
 */
uint8_t bsp_CanOpen(uint32_t baud){
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  

	//使能相关时钟
	RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);                   											 
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);	
	
  //初始化GPIO
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN| CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);
	
	//引脚复用映射配置
	GPIO_PinAFConfig(CAN_GPIO_PORT,CAN_RX_SOURCE,CAN_AF_PORT); 
	GPIO_PinAFConfig(CAN_GPIO_PORT,CAN_TX_SOURCE,CAN_AF_PORT); 
	  
	//CAN单元设置
	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //模式设置
	//波特率配置
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_7tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
	CAN_InitStructure.CAN_Prescaler=brp_from_birate(baud);  //分频系数(Fdiv)为brp+1	
	
	CAN_Init(CANx, &CAN_InitStructure);   // 初始化CAN1 
    
		//配置过滤器
 	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		

	//中断配置
	CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	return 1;
}


/**
  * @brief  This function handles CAN1 RX0 interrupt request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage = {0};
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	// Drop extended frames
	if(RxMessage.IDE == CAN_ID_EXT)
		return;
	canopen_canDispatch(RxMessage.StdId, RxMessage.Data,RxMessage.DLC,RxMessage.RTR);
}

/**
 * @brief Close a CANOpen device
 * @return 0 if succes 
 */
uint8_t bsp_CanClose(void){
	
	return 1;
}

/**
 * @brief Change the CANOpen device baudrate 
 * @param *baud The new baudrate to assign
 * @return 0 if succes
 */
uint8_t bsp_CanChangeBaudRate(uint32_t baud){
	
	return 1;
}


/**
 * @brief Open a timer
 * @param cob_id  the cobid
 * @param baud the baudRate
 * @return 0 if succes
 */
uint8_t bsp_TimerOpen(void){
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
	/* TIM3 TICK = 84M  10KHZ Set  1 TICK = 100us */
	uint16_t PrescalerValue = (uint16_t) (8400) - 1;

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
		
	return 1;
}


/**
 * @brief  timer enabled or disable
 * @param enable  is the timer enable
 * @return 0 if succes
 */
uint8_t bsp_TimerEnable(uint8_t enable){
	TIM_Cmd(TIM3, enable?ENABLE:DISABLE);
	return 1;
}

/**
 * @brief set timer
 * @param val  the time count
 * @return 0 if succes
 */
uint8_t bsp_setTimer(unsigned long long val){
	
	TIM_SetCounter(TIM3, val);
	return 1;
}

/**
 * @brief get timer
 * @return the timer count
 */
unsigned long long  bsp_getTimer(void){
	return TIM_GetCounter(TIM3);
	
}


// This function handles Timer 3 interrupt request.
void TIM3_IRQHandler(void)
{
	//printf("--\r\n");
	if(TIM_GetFlagStatus(TIM3, TIM_SR_UIF) == RESET)
		return;
	TIM_ClearITPendingBit(TIM3, TIM_SR_UIF);
	canopen_TimerDispatch();
}

