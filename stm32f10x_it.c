/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdio.h>

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern uint16_t Delay_Ms;
extern int16_t Delay_3s;
extern int16_t Delay_500ms;
extern int16_t Shake_Delay;
void SysTick_Handler(void)
{Delay_Ms--;
if(Delay_3s>=-1)Delay_3s--;
if(Delay_500ms>=-1)Delay_500ms--;
if(Shake_Delay>=-1)Shake_Delay--;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
extern uint8_t Rx_Buff[10];
extern uint8_t Rx_Length;
extern uint8_t Rx_Over;
void USART1_IRQHandler(void)
{
	uint8_t ch;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
			ch = USART_ReceiveData(USART1);
		
	  	printf( "%c", ch );    
	} 
}	
#define DATE_A 10    //数组二维数值
#define DATE_B 17
extern	uint8_t  sRecog[DATE_A][DATE_B];
extern uint8_t help_switch;
extern uint8_t Call_Switch;
extern _Bool Get_through;
extern _Bool Shake_Switch;
uint8_t Ch_Sw=0;
void USART2_IRQHandler(void)
{ 
	uint8_t ch;
	uint16_t i=0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 	
			ch = USART_ReceiveData(USART2);
		 if(ch!='\n')
		 {Rx_Buff[Rx_Length++]=ch;
		 }
		 if(ch=='\n')
		 {if((Rx_Buff[0]=='I'||Rx_Buff[0]=='P')&&Rx_Buff[1]=='R')///目前无法判断对方打过来电话后挂断了电话，或者打了电话对方没有接听 总之为对方挂断电话的情况。
			 { 
			   Call_Switch=1;
			 }
			 if(Rx_Buff[0]=='I'&&Rx_Buff[1]=='R')//如果对方打电话过来,开启震动
			 {
			   Shake_Switch=1;
			 }
			 if(Call_Switch==1&&strstr(Rx_Buff,"APR+call_end\r"))
			 {Call_Switch=0;
			  Get_through=0;
				Shake_Switch=0;
			 }//如果对方主动挂断电话.程序主动发送一个call_end 
			 if(strstr(Rx_Buff,"II\r"))
			 {for(i=5000;i>0;i--);
			   printf("APT+SPP8888\r\n");
			 }
			 if(strstr(Rx_Buff,"APR+help_off\r"))
			 {help_switch=0;			  
			 }
			 if(strstr(Rx_Buff,"APR+help_on\r"))
			 {help_switch=1;			  
			 }
    
       if(strstr(Rx_Buff,"APR+Change_Finish\r"))
			 {Ch_Sw=0;
			 }		 
       if(Ch_Sw==1&&Rx_Buff[0]=='A')
			 {  
			  for(i=0;i<17;i++)//清空上一次的命令
				 {sRecog[Rx_Buff[4]-48][i]=0;
				 }
				 i=0;
			   while(Rx_Buff[i+6]!='\r')
				 {sRecog[Rx_Buff[4]-48][i]=Rx_Buff[i+6];
				  i++;
				 }
				 //printf("%d%s",Rx_Buff[4]-48,sRecog[Rx_Buff[4]-48]);////测试用的程序
				 printf("APT+OK1\r\n");
			 }	   
			 if(strstr(Rx_Buff,"APR+Change_Begin\r"))
			 {Ch_Sw=1;
			   printf("APT+OK1\r\n");
			   
			 }			 
			for(i=0;i<Rx_Length;i++)Rx_Buff[i]=0;
			ch=0;
		  Rx_Length=0;  
		 }
	  	//printf( "%c", ch );    
	}  
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILe****/
