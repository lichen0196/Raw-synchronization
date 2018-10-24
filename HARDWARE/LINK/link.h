#ifndef __LINK_H
#define __LINK_H

#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "spi.h"
#include "24l01.h" 



void Link_Init(void);
void Link_Send(u8 link_dat[32]);
void Link_Recv(void);


#endif
