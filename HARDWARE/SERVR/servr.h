#ifndef __SERVR_H
#define __SERVR_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

void Servr_Init(void);
void Servr_GPIO_Init(void);//初始化
void TIM3_Config(void);
void Change_Poi_Arm(int poi);
void Change_Poi_Head(int poi);
		 				    
#define ARM_UP_W		
#define ARM_UP_R		
#define ARM_DOWN_W			1250
#define ARM_DOWN_R			1400

#define HEAD_OPEN_NOR		
#define HEAD_CLOSE_NOR		850
#define HEAD_OPEN_PEN
#define HEAD_CLOSE_PEN

#endif
