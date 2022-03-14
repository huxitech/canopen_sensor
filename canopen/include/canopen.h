#ifndef CANOPEN_API_H_
#define CANOPEN_API_H_

#include <stdint.h>

/** definitions used for object dictionary access. ie SDO Abort codes . (See DS 301 v.4.02 p.48)
 */
#define OD_SUCCESSFUL 	             0x00000000
#define OD_READ_NOT_ALLOWED          0x06010001
#define OD_WRITE_NOT_ALLOWED         0x06010002
#define OD_NO_SUCH_OBJECT            0x06020000
#define OD_NOT_MAPPABLE              0x06040041
#define OD_ACCES_FAILED              0x06060000
#define OD_LENGTH_DATA_INVALID       0x06070010
#define OD_NO_SUCH_SUBINDEX 	     0x06090011
#define OD_VALUE_RANGE_EXCEEDED      0x06090030 /* Value range test result */
#define OD_VALUE_TOO_LOW             0x06090031 /* Value range test result */
#define OD_VALUE_TOO_HIGH            0x06090032 /* Value range test result */
/* Others SDO abort codes 
 */
#define SDOABT_TOGGLE_NOT_ALTERNED   0x05030000
#define SDOABT_TIMED_OUT             0x05040000
#define SDOABT_CS_NOT_VALID          0x05040001
#define SDOABT_INVALID_BLOCK_SIZE    0x05040002
#define SDOABT_OUT_OF_MEMORY         0x05040005 /* Size data exceed SDO_MAX_LENGTH_TRANSFER */
#define SDOABT_GENERAL_ERROR         0x08000000 /* Error size of SDO message */
#define SDOABT_LOCAL_CTRL_ERROR      0x08000021 

/**
 * @brief stack init
 * @param node_id node_id
 * @param baud baud
 * @param tick_time tick for us 
 * @param max_tick_count the period  for timer
 * @return 0 if succes
 */
uint8_t canopen_Init(uint8_t node_id,uint32_t baud,uint32_t tick_time,uint32_t timer_period);

/**
 * @brief object dict set
 * @param wIndex index
 * @param bSubindex subindex
 * @param data data
 * @param len data len
 * @return SDO Abort codes . (See DS 301 v.4.02 p.48)
 */
uint32_t canopen_ODWrite(uint16_t wIndex,uint8_t bSubindex, uint8_t*data,uint8_t len);

/**
 * @brief object dict get
 * @param wIndex index
 * @param bSubindex subindex
 * @param data data
 * @param len data len
 * @return SDO Abort codes . (See DS 301 v.4.02 p.48)
 */
uint32_t canopen_ODRead(uint16_t wIndex,uint8_t bSubindex, uint8_t*data,uint8_t *len,uint8_t* data_type);

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
