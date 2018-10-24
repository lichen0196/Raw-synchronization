#include "led.h"
//#include "delay.h"
#include "sys.h"
#include "usart.h"
//#include "pwm.h"
//#include "ctrl.h"
//#include "servr.h"
//#include "link.h"
//#include "24l01.h"
//#include "spi.h"
#include "exti.h"


volatile u8 COMMAND = 0;


void Sys_Init(void);

int main(void)
{ 	 
//	TIM_SetCompare3(TIM4,500);
//	TIM_SetCompare4(TIM4,500);
	Sys_Init();


   	while(1)
	{
		
		
	} 
}

void Sys_Init(void)
{
	
	uart_init(115200);//for debug
	    	 //???????
	EXTIX_Init();
	LED_Init();		  	//????LED???????

}

























