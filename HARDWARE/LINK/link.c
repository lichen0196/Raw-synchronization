#include "link.h"
#include "exti.h"


void Link_Init(void)
{
	NRF24L01_Init(); 


	while(NRF24L01_Check())	//检查NRF24L01是否在位.	
	{
		printf("NRF24L01 Error!\n");
		delay_ms(200);
	}								   
 	printf("NRF24L01 OK!\n");


}













void Link_Send(u8 link_dat[32])
{

	EXTI->IMR = 0;

	NRF24L01_TX_Mode();
	  		   				 
	if(NRF24L01_TxPacket(link_dat)==TX_OK)
	{
		printf("Sended DATA");		   
	}
	else
	{										   	
		printf("Send Failed");
	};			    
	

	
	NRF24L01_RX_Mode();
	EXTI->IMR = 0x04;

}













void Link_Recv(void)
{
    NRF24L01_RX_Mode();

	EXTIX_Init();
}
