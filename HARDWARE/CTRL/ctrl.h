#ifndef __CTRL_H
#define __CTRL_H	 
#include "sys.h"
#include "delay.h"

#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//?????????,??????,??????????
//ALIENTEK??STM32???
//LED????	   
//????@ALIENTEK
//????:www.openedv.com
//????:2012/9/2
//??:V1.0
//????,?????
//Copyright(C) ????????????? 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

extern volatile u8 FLAG_OVERFLOW;
extern volatile u32 TIME_PASS;
extern volatile u8 LINE_PASS;

void Ctrl_Init(void);
void LE_GPIO_Init(void);
void Dir_GPIO_Init(void);
void Ctrl_Tim_Init(void);
void Change_Speed_Left(int speed_left);
void Change_Speed_Right(int speed_right);
u8 Scan_Road(void);
u8 Config_Line(u8 res);
//void Cal_Speed_Up(u8);
//void Turn_Left_Ahead(void);
//void Turn_Right(void);
void Left_Dir(u8 dir);
void Right_Dir(u8 dir);
void Go_Up(u8 dis,u8 mode);
void Cal_Speed_Up(u8 res,u8 mode);
void Go_Down(void);
void Cal_Speed_Down(u8 res);
void Turn_Left(void);
void Turn_Right(void);


#define LE_L1			PAin(8)
#define LE_L2			PAin(11)		 
#define LE_L3			PAin(12)
#define LE_R1			PBin(5)
#define LE_R2			PBin(6)
#define LE_R3			PBin(7)

/////////////////////////////////////

#define right_up		PBout(12)
#define right_down		PBout(13)
#define left_up			PBout(14)
#define left_down		PBout(15)

/////////////////////////////////////

#define LEVEL_1			100
#define LEVEL_2			150
#define LEVEL_3			175

#define LEVEL_1_B		150
#define LEVEL_2_B		175
#define LEVEL_3_B		200
/////////////////////////////////////
#define BIT_L1			0x40 
#define BIT_L2			0x20
#define BIT_L3			0x10
#define BIT_R1			0x04
#define BIT_R2			0x02
#define BIT_R3			0x01

#define BIT_RT			0x07
#define BIT_LT			0x70

#endif
