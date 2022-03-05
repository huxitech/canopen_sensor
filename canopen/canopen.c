#include "object/MagNet.h"
#include "include/canopen.h"

extern unsigned char canInit(CO_Data * d, uint32_t bitrate);
extern void initTimer(void);
extern TIMEVAL last_counter_val;
extern TIMEVAL elapsed_time ;
extern CO_Data *co_data;

#define CANOPEN_CO_DATA MagNet_Data

/**
 * @brief stack init
 * @param node_id node_id
 * @param baud baud
 * @return 0 if succes
 */
uint8_t canopen_Init(uint8_t node_id,uint32_t baud){
  initTimer(); 
  canInit(&CANOPEN_CO_DATA,baud);
  setNodeId (&CANOPEN_CO_DATA, node_id);
  setState(&CANOPEN_CO_DATA, Initialisation);	// Init the state
  return 1;
}


/**
 * @brief object dict set
 * @param wIndex index
 * @param bSubindex subindex
 * @param data data
 * @param len data len
 * @return 0 if succes
 */
uint32_t canopen_ODWrite(uint16_t wIndex,uint8_t bSubindex, uint8_t*data,uint8_t len){
	

	return writeLocalDict( 
					&CANOPEN_CO_DATA,       /*CO_Data* d*/
					wIndex,                 /*UNS16 index*/
					bSubindex,                   /*UNS8 subind*/ 
					data,            /*void * pSourceData,*/ 
					(UNS32*)len,                  /* UNS8 * pExpectedSize*/
					0);                    /* UNS8 checkAccess */

}

/**
 * @brief object dict get
 * @param wIndex index
 * @param bSubindex subindex
 * @param data data
 * @param len data len
 * @return 0 if succes
 */
uint32_t canopen_ODRead(uint16_t wIndex,uint8_t bSubindex, uint8_t*data,uint8_t *len,uint8_t* data_type){
	return readLocalDict( 
					&CANOPEN_CO_DATA,       /*CO_Data* d*/
					wIndex,                 /*UNS16 index*/
					bSubindex,                   /*UNS8 subind*/ 
					data,            /*void * pSourceData,*/ 
					(UNS32*)len,                  /* UNS8 * pExpectedSize*/
					(char*)data_type,
					0);                    /* UNS8 checkAccess */	

}

/**
 * @brief the timer dispatch
 * @return void
 */
void  canopen_TimerDispatch(void){
	last_counter_val = 0;
	elapsed_time = 0;
	TimeDispatch();
}

/**
 * @brief the can message dispatch
 * @param cob_id cob id
 * @param bSubindex subindex
 * @param data data
 * @param len data len
 * @param rtr is CAN_RTR_REMOTE?
 * @return void
 */
void  canopen_canDispatch(uint32_t cob_id,uint8_t*data,uint8_t len,uint8_t rtr){
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
