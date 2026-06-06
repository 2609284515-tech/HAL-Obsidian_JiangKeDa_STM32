#ifndef __MYCAN_H
#define __MYCAN_H

void MyCAN_Init(void);
void MyCAN_Transmit(CAN_TxHeaderTypeDef *TxMessage, uint8_t *Data);
uint8_t MyCAN_ReceiveFlag(void);
void MyCAN_Receive(CAN_RxHeaderTypeDef *RxMessage, uint8_t *Data);

#endif