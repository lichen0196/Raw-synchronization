#include "servr.h"

void Servr_Init(void)
{
	Servr_GPIO_Init();
	TIM3_Config();
}

void Servr_GPIO_Init(void)  
{ 
  GPIO_InitTypeDef GPIO_InitStructure; 
 
  /* GPIOA clock enable */ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  
 
  /*GPIOA Configuration: TIM4 channel 3 and 4 as alternate function push-pull */ 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           // 复用推挽输出 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
} 

void TIM3_Config(void)  
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
	TIM_OCInitTypeDef  TIM_OCInitStructure;  
	/* PWM信号电平跳变值 */  
	/*PCLK1经过2倍频后作为TIM4的时钟源等于72MHz*/  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
	/* Time base configuration */                                            
	TIM_TimeBaseStructure.TIM_Period =20000-1;  
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;                                    //设置预分频：预分频=2，即为72/3=24MHz  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //设置时钟分频系数：不分频  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //向上计数溢出模式  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
	/* PWM1 Mode configuration: Channel1 */  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //配置为PWM模式1  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
	TIM_OCInitStructure.TIM_Pulse = 1500;                                       //设置跳变值，当计数器计数到这个值时，电平发生跳变  
	TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //当定时器计数值小于CCR1时为高电平  
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);                                    //使能通道1      
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  
	/* PWM1 Mode configuration: Channel2 */  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
	TIM_OCInitStructure.TIM_Pulse = 1500;                                       //设置通道2的电平跳变值，输出另外一个占空比的PWM  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                    //当定时器计数值小于CCR2时为低电平 
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);                                    //使能通道2  
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  
	TIM_ARRPreloadConfig(TIM3, ENABLE);                                         //使能TIM4重载寄存器ARR  
	/* TIM4 enable counter */  
	TIM_Cmd(TIM3, ENABLE);                                                      //使能TIM4   
} 

/*
input from 500-2500
*/
void Change_Poi_Arm(int poi)
{
	TIM_SetCompare3(TIM3,poi);
}

void Change_Poi_Head(int poi)
{
	TIM_SetCompare4(TIM3,poi);
}
