#ifndef __AD_H
#define __AD_H

extern uint16_t AD_Value[2];

void AD_Init(void);
void AD_StartGetValue_DMA(void);

#endif