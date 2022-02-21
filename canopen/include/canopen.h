#ifndef CANOPEN_API_H_
#define CANOPEN_API_H_

#include <stdint.h>

/**
 * @brief stack init
 * @param node_id node_id
 * @param baud baud
 * @return 0 if succes
 */
uint8_t canopen_Init(uint8_t node_id,uint32_t baud);

/**
 * @brief object dict set
 * @param wIndex index
 * @param bSubindex subindex
 * @param data data
 * @param len data len
 * @return 0 if succes
 */
uint8_t canopen_ODWrite(uint16_t wIndex,uint8_t bSubindex, uint8_t*data,uint8_t len);


/**
 * @brief the timer dispatch
 * @return void
 */
void  canopen_TimerDispatch(void);

/**
 * @brief the can message dispatch
 * @param cob_id cob id
 * @param bSubindex subindex
 * @param data data
 * @param len data len
 * @param rtr is CAN_RTR_REMOTE?
 * @return void
 */
void  canopen_canDispatch(uint32_t cob_id,uint8_t*data,uint8_t len,uint8_t rtr);

#endif
