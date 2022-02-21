#include "object/MagNet.h"
#include "include/canopen.h"

extern unsigned char canInit(CO_Data * d, uint32_t bitrate);
extern void initTimer(void);


/**
 * @brief stack init
 * @param node_id node_id
 * @param baud baud
 * @return 0 if succes
 */
uint8_t canopen_Init(uint8_t node_id,uint32_t baud){
  initTimer(); 
  canInit(&MagNet_Data,baud);
  setNodeId (&MagNet_Data, node_id);
  setState(&MagNet_Data, Initialisation);	// Init the state
  return 1;
}
