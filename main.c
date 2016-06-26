#include "stm32f10x.h"
#include "usart.h"
#include "LD3320.h"
void Dely_Ms(uint16_t Timing);
uint16_t Delay_Ms=0;
uint8_t Rx_Buff[10]={0};
uint8_t Rx_Length=0;
uint8_t Rx_Over=0;
/******************************************************************************/

int main(void)
{SysTick_Config(SystemCoreClock/1000);
	USART_init();
	LD3320_main();				//LD3320Ö´ÐÐº¯Êý
	for(;;)
	{
		
	}
}
/*********************************************END OF FILE**********************/
void Dely_Ms(uint16_t Timing)
{
Delay_Ms=Timing;
	while(Delay_Ms);

}