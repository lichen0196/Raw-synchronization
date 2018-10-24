#include "ctrl.h"

void Ctrl_Init(void)
{
	LE_GPIO_Init();
	Dir_GPIO_Init();
	Ctrl_Tim_Init();
	Left_Dir(0);
	Right_Dir(0);
}
/*
	Black---1
	White---0
	
	PB 012 567
*/
void LE_GPIO_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure; 
 
	// GPIOB clock enable  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);  
 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           // 上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

/*
	PB12---left up
	PB13---left down
	PB14---right up
	PB15---right down

	up--AIN2=1
		AIN1=0
		BIN2=1
		BIN1=0
*/
void Dir_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
 
	/* GPIOB clock enable */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  
 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           // 推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void Ctrl_Tim_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��ʱ�� TIM ʹ��       
	   //��ʱ�� TIM ��ʼ��  
	TIM_TimeBaseStructure.TIM_Period = 999;    //�����Զ���װ�ؼĴ������ڵ�ֵ    
	TIM_TimeBaseStructure.TIM_Prescaler =71;  //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�  
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM ���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);          //�ڳ�ʼ�� TIM  
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );             //�����������ж�   
       //�ж����ȼ� NVIC ����  
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;        //TIM �ж�  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ� 1 ��  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //�����ȼ� 1 ��  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      //IRQ ͨ����ʹ��  
	NVIC_Init(&NVIC_InitStructure);                         //�ܳ�ʼ�� NVIC �Ĵ���  
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)   //TIM �ж� 
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
    	TIM_ClearITPendingBit(TIM2, TIM_IT_Update  ); //���� TIM �����жϱ�־

    	FLAG_OVERFLOW = 1;
    	TIME_PASS ++;
    }
}

void Change_Speed_Left(int speed_left)
{
	TIM_SetCompare4(TIM4,speed_left);
}

void Change_Speed_Right(int speed_right)
{
	TIM_SetCompare3(TIM4,speed_right);
}

/*
	Black---1
	White---0
	
	PB 012 567
*/

/*
	result:
		0 0 L3 L2 L1 R1 R2 R3
*/
u8 Scan_Road(void)
{
	u8 result = 0;

	if(LE_L3==0){
		result |= BIT_L3;
	}
	if(LE_L2==0){
		result |= BIT_L2;
	}
	if(LE_L1==0){
		result |= BIT_L1;
	}
	if(LE_R1==0){
		result |= BIT_R1;
	}
	if(LE_R2==0){
		result |= BIT_R2;
	}
	if(LE_R3==0){
		result |= BIT_R3;
	}

	return result;
}

u8 Scan_Road_Cal(void)
{
	u8 result;
	u8 times = 0;

	result = Scan_Road();
	while(times < 10){
		if(Scan_Road()==result){
			times ++;
		}
		else{
			break;
		}
	}
	if(times == 10){
		//printf("%x ",result);
		return result;
	}
	else{
		Scan_Road_Cal();
	}
}

////////////////////////////////
//此处有一个大bug
u8 Config_Line(u8 res)
{
	u8 times = 0;
	
	while(times < 10){
		if((BIT_L1|BIT_L2|BIT_R1|BIT_R2) == (res & (BIT_L1|BIT_L2|BIT_R1|BIT_R2))){
			times++;
		}
		else{
			break;
		}
	}

	if(times==10){
		return 1;
	}
	else{
		return 0;
	}
}

/* 
	0 -- go up
	1 -- go down
	2 -- stop
*/
void Left_Dir(u8 dir)
{
	switch(dir){
		case 0: 	left_up = 1;left_down = 0;break;
		case 1:		left_up = 0;left_down = 1;break;
		default:	left_up = 0;left_down = 0;break;
	}
}

void Right_Dir(u8 dir)
{
	switch(dir){
		case 0: 	right_up = 1;right_down = 0;break;
		case 1:		right_up = 0;right_down = 1;break;
		default:	right_up = 0;right_down = 0;break;
	}
}

/*
	dis -- the distance need to go 
	mode --
		0 - without slow down
		1 - slow down and stop
*/
void Go_Up(u8 dis,u8 mode)
{
	u8 flag_finish = 0;
	u8 flag_start = 1;
	u8 res;

	Left_Dir(0);
	Right_Dir(0);
	TIME_PASS = 0;

	if(dis != 1){
		Go_Up(1,0);
		Go_Up(dis-1,mode);
	}
	else{
		while(flag_start){
			if(1 == FLAG_OVERFLOW){
				res = Scan_Road_Cal();
				Cal_Speed_Up(res,mode);
				if(1 == Config_Line(res)){		//发现白线
					;
				}
				else{
					flag_start = 0;
				}
				FLAG_OVERFLOW = 0;
			}
		}
		while(!flag_finish){
			if(1 == FLAG_OVERFLOW){
				//检测
				printf("1\r\n");
				res = Scan_Road_Cal();
				printf("2\r\n");
				Cal_Speed_Up(res,mode);
				printf("3\r\n");
				if(0 == Config_Line(res)){		//未发现白线
					;
					if(2 == mode){
						if(TIME_PASS > 2000){
							flag_finish = 1;
						}
					}
				}
				else{
					LINE_PASS ++;
					flag_finish = 1;
					//printf("white line\r\n");
				}
				printf("4\r\n");
				FLAG_OVERFLOW = 0;
			}
			else{
				;
			}
			printf("5\r\n");
		}
		if(1==mode){
			Left_Dir(2);
			Right_Dir(2);
			delay_ms(500);
			//printf("stop\r\n");

		}
		
	}
	//printf("finish!\r\n");
	return;
}

void Cal_Speed_Up(u8 res,u8 mode)
{
	int speed_left,speed_right;

	speed_left = 500;
	speed_right = 500;

	if(res & BIT_L3){
		speed_left -= LEVEL_3;
		speed_right += LEVEL_3;
	}
	else if(res & BIT_L2){
		speed_left -= LEVEL_2;
		speed_right += LEVEL_2;
	}
	else if(res & BIT_L1){
		speed_left -= LEVEL_1;
		speed_right += LEVEL_1;
	}

	if(res & BIT_R3){
		speed_left += LEVEL_3;
		speed_right -= LEVEL_3;
	}
	else if(res & BIT_R2){
		speed_left += LEVEL_2;
		speed_right -= LEVEL_2;
	}
	else if(res & BIT_R1){
		speed_left += LEVEL_1;
		speed_right -= LEVEL_1;
	}

	if(0 == mode){
		Change_Speed_Right((int)speed_right);
		Change_Speed_Left((int)speed_left);
	}
	else if(1 == mode){
		if(TIME_PASS > 400){
			Change_Speed_Right((int)speed_right*0.7);
			Change_Speed_Left((int)speed_left*0.7);
			//printf("slow1\r\n");
		}
		else if(TIME_PASS >200){
			Change_Speed_Right((int)speed_right*0.8);
			Change_Speed_Left((int)speed_left*0.8);
			//printf("slow0\r\n");
		}
	}
	else if(2 == mode){
		Change_Speed_Right((int)speed_right*0.6);
		Change_Speed_Left((int)speed_left*0.6);
	}
}

void Go_Down(void)
{
	u8 flag_finish = 0;
	u8 flag_start = 1;
	u8 res;

	Left_Dir(1);
	Right_Dir(1);
	TIME_PASS = 0;

	while(flag_start){
		if(1 == FLAG_OVERFLOW){
			res = Scan_Road_Cal();
			Cal_Speed_Down(res);
			if(1 == Config_Line(res)){		//发现白线
				;
			}
			else{
				flag_start = 0;
			}
			FLAG_OVERFLOW = 0;
		}
	}
	//printf("start finish\r\n");
	while(!flag_finish){
		if(1 == FLAG_OVERFLOW){
			//检测
			res = Scan_Road_Cal();
			Cal_Speed_Down(res);
			if(0 == Config_Line(res)){		//未发现白线
				;
			}
			else{
				LINE_PASS ++;
				flag_finish = 1;
				//printf("white line\r\n");
			}
			FLAG_OVERFLOW = 0;
		}
		else{
			;
		}
	}
	
	Left_Dir(2);
	Right_Dir(2);
	return;
}

void Cal_Speed_Down(u8 res)
{
	int speed_left,speed_right;

	speed_left = 300;
	speed_right = 300;

	if(res & BIT_L3){
		speed_left -= LEVEL_3_B;
		speed_right += LEVEL_3_B;
	}
	else if(res & BIT_L2){
		speed_left -= LEVEL_2_B;
		speed_right += LEVEL_2_B;
	}
	else if(res & BIT_L1){
		speed_left -= LEVEL_1_B;
		speed_right += LEVEL_1_B;
	}

	if(res & BIT_R3){
		speed_left += LEVEL_3_B;
		speed_right -= LEVEL_3_B;
	}
	else if(res & BIT_R2){
		speed_left += LEVEL_2_B;
		speed_right -= LEVEL_2_B;
	}
	else if(res & BIT_R1){
		speed_left += LEVEL_1_B;
		speed_right -= LEVEL_1_B;
	}

	Change_Speed_Right((int)speed_right*0.8);
	Change_Speed_Left((int)speed_left*0.8);
	
}

void Turn_Left(void)
{
	int times;
	u8 flag_state = 0;
	u8 res;

	Change_Speed_Right(400);
	Change_Speed_Left(400);
	
	Right_Dir(0);
	Left_Dir(1);

	TIME_PASS = 0;

	while(1){
		while(0 == flag_state){
			if(1 == FLAG_OVERFLOW){
				res = Scan_Road_Cal();
				if(((res & 0x70)==0x0) ||((res & 0x70)==0x40)){		//XXX XBB
					;
				}
				else{
					if((TIME_PASS >= 100) && (0x20 == (res & 0x60))){	//enter xxx BWx
						flag_state = 1;
						times = TIME_PASS;
					}
				}	
				FLAG_OVERFLOW = 0;
			}
			else{
				;
			}
		}

		while(1 == flag_state){
			if(1 == FLAG_OVERFLOW){
				res = Scan_Road_Cal();
				if(((res & 0x60)==0x20) || ((res & 0x60)==0x60)){		//config XXX xWx
					if((times - TIME_PASS) > 100){
						flag_state = 2;
					}
					else{
						;
					}
				}
				else{
					flag_state = 0;
				}	
				FLAG_OVERFLOW = 0;
			}
			else{
				;
			}
		}

		while(2 == flag_state){
			if(1 == FLAG_OVERFLOW){
				res = Scan_Road_Cal();
				if(((res & 0x60)==0x20) || ((res & 0x60)==0x60)){		//XXX xWx
					;
				}
				else{
					if((res & 0x60)==0x40){
						flag_state = 3;
						times = TIME_PASS;
					}
					else{
						;
					}
				}	
				FLAG_OVERFLOW = 0;
			}
			else{
				;
			}
		}

		while(3 == flag_state){
			if(1 == FLAG_OVERFLOW){
				res = Scan_Road_Cal();
				if((res & 0x60)==0x40){		//XXX WBx
					if((times - TIME_PASS) > 100){
						flag_state = 4;
					}
					else{
						;
					}
				}
				else{
					;
				}	
				FLAG_OVERFLOW = 0;
			}
			else{
				;
			}
		}

		if(4 == flag_state){
			break;
		}
	}

	Right_Dir(2);
	Left_Dir(2);
}

void Turn_Right(void)
{
	int times;
	u8 flag_state = 0;
	u8 res;

	Change_Speed_Right(400);
	Change_Speed_Left(400);
	
	Right_Dir(1);
	Left_Dir(0);

	TIME_PASS = 0;

	while(1){
		while(0 == flag_state){
			if(1 == FLAG_OVERFLOW){
				res = Scan_Road_Cal();
				if(((res & BIT_RT)==0x0) ||((res & BIT_RT)==BIT_R1)){		//XXX XBB
					;
				}
				else{
					if((TIME_PASS >= 100) && (BIT_R2 == (res & (BIT_R2|BIT_R1)))){	//enter xxx BWx
						flag_state = 1;
						times = TIME_PASS;
					}
				}	
				FLAG_OVERFLOW = 0;
			}
			else{
				;
			}
		}

		while(1 == flag_state){
			if(1 == FLAG_OVERFLOW){
				res = Scan_Road_Cal();
				if(((res & (BIT_R2|BIT_R1))==BIT_R2) || ((res & (BIT_R2|BIT_R1))==(BIT_R2|BIT_R1))){		//config XXX xWx
					if((times - TIME_PASS) > 100){
						flag_state = 2;
					}
					else{
						;
					}
				}
				else{
					flag_state = 0;
				}	
				FLAG_OVERFLOW = 0;
			}
			else{
				;
			}
		}

		while(2 == flag_state){
			if(1 == FLAG_OVERFLOW){
				res = Scan_Road_Cal();
				if(((res & (BIT_R2|BIT_R1))==BIT_R2) || ((res & (BIT_R2|BIT_R1))==(BIT_R2|BIT_R1))){		//XXX xWx
					;
				}
				else{
					if((res & (BIT_R2|BIT_R1))==BIT_R1){
						flag_state = 3;
						times = TIME_PASS;
					}
					else{
						;
					}
				}	
				FLAG_OVERFLOW = 0;
			}
			else{
				;
			}
		}

		while(3 == flag_state){
			if(1 == FLAG_OVERFLOW){
				res = Scan_Road_Cal();
				if((res & (BIT_R2|BIT_R1))==BIT_R1){		//XXX WBx
					if((times - TIME_PASS) > 100){
						flag_state = 4;
					}
					else{
						;
					}
				}
				else{
					;
				}	
				FLAG_OVERFLOW = 0;
			}
			else{
				;
			}
		}
		
		if(4 == flag_state){
			if(1 == FLAG_OVERFLOW){
				res = Scan_Road_Cal();
				if((res & (BIT_R2|BIT_R1))==0x00){		//XXX WBx
					if((times - TIME_PASS) > 100){
						flag_state = 5;
					}
					else{
						;
					}
				}
				else{
					;
				}	
				FLAG_OVERFLOW = 0;
			}
			else{
				;
			}
		}
		
		if(5 == flag_state){
			//delay_ms(50);
			break;
		}
	}

	Right_Dir(0);
	Left_Dir(1);
	
	delay_ms(50);
	
	Right_Dir(2);
	Left_Dir(2);
	delay_ms(50);
}
