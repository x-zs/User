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
#include "PWM_LED.h"
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


uint8_t UART_UpdataFlag = 0;
uint8_t IMU_SampleFlag = 0;
uint8_t Press_SampleFlag = 0;

static uint8_t Tim_48ms, Tim_32ms;
static uint16_t	Tim_500mS;
void SysTick_Handler(void)
{	
	Delay_Ms--;
if(Delay_3s>=-1)Delay_3s--;
if(Delay_500ms>=-1)Delay_500ms--;
if(Shake_Delay>=-1)Shake_Delay--;
	if(++ Tim_48ms >=48)
	{
		IMU_SampleFlag=1;
		Tim_48ms = 0;
	}

	if(++ Tim_32ms >=32)
	{
		Press_SampleFlag=1;
		Tim_32ms = 0;
	}

	if(++ Tim_500mS >= 1000)
	{
		Tim_500mS = 0;
		UART_UpdataFlag = 1;
	}
	
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
#define DATE_A 10    //Êý×é¶þÎ¬ÊýÖµ
#define DATE_B 17
extern	uint8_t  sRecog[DATE_A][DATE_B];
extern uint8_t help_switch;
extern uint8_t Call_Switch;
extern _Bool Get_through;
extern _Bool Shake_Switch;
uint8_t Ch_Sw=0;
void USART3_IRQHandler(void)
{ 
	uint8_t ch;
	uint16_t i=0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{  	
			ch = USART_ReceiveData(USART3);
		 if(ch!='\n')
		 {Rx_Buff[Rx_Length++]=ch;
		 }
		 if(ch=='\n')
		 {if((Rx_Buff[0]=='I'||Rx_Buff[0]=='P')&&Rx_Buff[1]=='R')///Ä¿Ç°ÎÞ·¨ÅÐ¶Ï¶Ô·½´ò¹ýÀ´µç»°ºó¹Ò¶ÏÁËµç»°£¬»òÕß´òÁËµç»°¶Ô·½Ã»ÓÐ½ÓÌý ×ÜÖ®Îª¶Ô·½¹Ò¶Ïµç»°µÄÇé¿ö¡£
			 { 
			   Call_Switch=1;
			 }
			 if(Rx_Buff[0]=='I'&&Rx_Buff[1]=='R')//Èç¹û¶Ô·½´òµç»°¹ýÀ´,¿ªÆôÕð¶¯
			 {
			   Shake_Switch=1;
			 }
			 if(Call_Switch==1&&strstr(Rx_Buff,"APR+call_end\r"))
			 {Call_Switch=0;
			  Get_through=0;
				Shake_Switch=0;
			 }//Èç¹û¶Ô·½Ö÷¶¯¹Ò¶Ïµç»°.³ÌÐòÖ÷¶¯·¢ËÍÒ»¸öcall_end 
			 if(strstr(Rx_Buff,"II\r"))
			 {for(i=5000;i>0;i--);
			   printf("APT+SPP8888\r\n");
			 }
			 if(strstr(Rx_Buff,"APR+help_off\r"))help_switch=0;		//¹ØÇó¾È¹¦ÄÜ	  			 
			 else if(strstr(Rx_Buff,"APR+help_on\r"))help_switch=1;	//¿ªÇó¾È¹¦ÄÜ		  
			 else if(strstr(Rx_Buff,"APR+LED_OFF\r"))TIM3_Config(0);//¹Ø
			 else if(strstr(Rx_Buff,"APR+LED_H\r"))TIM3_Config(20);//ledÁÁ¶È¸ß
			 else if(strstr(Rx_Buff,"APR+LED_M\r"))TIM3_Config(8);//ÖÐ
			 else if(strstr(Rx_Buff,"APR+LED_L\r"))TIM3_Config(2);//µÍ
       else if(strstr(Rx_Buff,"APR+Change_Finish\r"))Ch_Sw=0;//¸Ä±ä¿ÚÁîÍê³É±êÖ+¾		 		 
       if(Ch_Sw==1&&Rx_Buff[0]=='A')
			 {  
			  for(i=0;i<17;i++)//Çå¿ÕÉÏÒ»´ÎµÄÃüÁî
				 {sRecog[Rx_Buff[4]-48][i]=0;
				 }
				 i=0;
			   while(Rx_Buff[i+6]!='\r')
				 {sRecog[Rx_Buff[4]-48][i]=Rx_Buff[i+6];
				  i++;
				 }
				 //printf("%d%s",Rx_Buff[4]-48,sRecog[Rx_Buff[4]-48]);////²âÊÔÓÃµÄ³ÌÐò
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
