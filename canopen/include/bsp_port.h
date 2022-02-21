#ifndef CANOPEN_BSP_PORT_H_
#define CANOPEN_BSP_PORT_H_

#include <stdint.h>

/**
 * @brief Send a CAN message
 * @param cob_id cob_id
 * @param data data
 * @param len data len
 * @return 0 if succes
 */
uint8_t bsp_CanSend(uint32_t cob_id,uint8_t*data,uint8_t len);

/**
 * @brief Open a CANOpen device
 * @param baud the baudRate
 * @return 0 if succes
 */
uint8_t bsp_CanOpen(uint32_t baud);

/**
 * @brief Close a CANOpen device
 * @return 0 if succes 
 */
uint8_t bsp_CanClose(void);

/**
 * @brief Change the CANOpen device baudrate 
 * @param *baud The new baudrate to assign
 * @return 0 if succes
 */
uint8_t bsp_CanChangeBaudRate(uint32_t baud);


/**
 * @brief Open a timer
 * @param cob_id  the cobid
 * @param baud the baudRate
 * @return 0 if succes
 */
uint8_t bsp_TimerOpen(void);


/**
 * @brief  timer enabled or disable
 * @param enable  is the timer enable
 * @return 0 if succes
 */
uint8_t bsp_TimerEnable(uint8_t enable);

/**
 * @brief set timer
 * @param val  the time count
 * @return 0 if succes
 */
uint8_t bsp_setTimer(unsigned long long val);

/**
 * @brief get timer
 * @return the timer count
 */
unsigned long long  bsp_getTimer(void);


/**
 * @brief Send a CAN message to stack
 * @param cob_id  the cobid
 * @param data data
 * @param len data len
 * @return 0 if succes
 */
uint8_t bsp_CanWrite(uint32_t cob_id,uint8_t*data,uint8_t len);



#endif
