#include "LD3320.h"
#include "stm32f10x_it.h"
#include "PWM_LED.h"
#include "Key_Exti.h"
#include "System.h"
#include "GPS_Data.h"
/************************************************************************************
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:			表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:			表示一次识别流程中LD3320芯片内部出现不正确的状态
*********************************************************************************/
void System_Init(void);
void Delay_1ms(uint16_t Timing);
void Send(uint8_t *str);
uint16_t Delay_Ms=0;
uint8 nAsrStatus = 0;	
uint8 nLD_Mode = LD_MODE_IDLE;//用来记录当前是在进行ASR识别还是在播放MP3
uint8 ucRegVal;
uint8 nAsrRes=0;
uint8_t Filck_LED=0;
int16_t Delay_3s=1;
uint8_t help_switch=1;
int16_t Delay_125ms=0;
int16_t Shake_Delay=0;
uint8_t Call_Switch=0;
_Bool Get_through=0;  ///当接听电话时屏蔽其他功能的口令
_Bool Shake_Switch=0;
_Bool Flick_Switch=0;
/////////////////
  uint8_t str1[100]="AT+CIPSTART=\"TCP\",\"150p1221e9.iok.la\",47471\r\n";  //连接服务器
	uint8_t str2[30]="AT+CIPSEND=24\r\n";            //向服务器发送20位经纬度数据
	uint8_t str4[30]="AT+CSCS=\"GB2312\"\r\n";      //发中文短信第一步
	uint8_t str5[30]="AT+CMGF=1\r\n";               //发中文短信第二步
	uint8_t str6[30]="AT+CMGS=\"17862347665\"\r\n"; //发中文短信第三步
	uint8_t str7[50]="您的背包已成功定位！\r"; //发中文短信最后一步
extern uint8_t RXOVER;
uint32_t Delay_30s=0;//定时30秒
uint8_t USART2_RxBuff[100];
uint8_t Jingweidu[50],weidu[20],jingdu[20];
extern uint8_t x,y;
////////////////////////
extern uint8_t UART_UpdataFlag;
extern uint8_t IMU_SampleFlag;
extern uint8_t Press_SampleFlag;
	#define DATE_A 20    //数组二维数值
	#define DATE_B 17		//数组一维数值
	//添加关键词，用户修改
	uint8_t  sRecog[DATE_A][DATE_B] = {
	 			"liu shui deng",\
				"zai jian bai bai",\
				"jie",\
				"chong bo dian hua",\
		    "gua duan dian hua",\
				"da dian hua" 
		
																	};	
///用户修改
void LD3320_main(void)
{	uint8_t i=0;

	
	LD3320_init();
	System_Init();//陀螺仪的初始化
  TIM3_PWM_Config(0);	
	Key_Exti_Config();
	nAsrStatus = LD_ASR_NONE;//初始状态：没有在作ASR
	//Send(str1);
	LED1_ON();
	LED2_ON();
	//Delay_1ms(5000);
	//Send(str4);
	//Delay_1ms(500);
	//Send(str5);
	//Delay_1ms(500);
	while(1)
	{	if(RXOVER == 1&& UART_UpdataFlag){
				gpsdata(USART2_RxBuff,Jingweidu);
				GPS_Dispose(Jingweidu,weidu,jingdu);
				if(Jingweidu[3])
				{
					//Send(str2);
					Send(Jingweidu);
				}
			for(i=0;i<100;i++){
				USART2_RxBuff[i] = 0;  //清空接收区
			}
			RXOVER = 0;
			Delay_30s=0;
			x=0;
			y=0;
			USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
			}
				
				
		if(Filck_LED==1&&Delay_125ms<=0)///是否开启危险报警灯
		 {
		  GPIO_WriteBit(LED3_GPIO_PORT,LED3_PIN,(BitAction)((1-GPIO_ReadOutputDataBit(LED3_GPIO_PORT,LED3_PIN))));
			TIM3_Config(Flick_Switch=!Flick_Switch); //前照明灯当做闪光灯
		 } 
		if(IMU_SampleFlag)
		{
			IMU_GetYawPitchRoll(angles);	
			IMU_SampleFlag=0;
		}
		if(Press_SampleFlag)
		{ 				
			CalTemperatureAndPressureAndAltitude();
			Press_SampleFlag=0;
		}
		if(UART_UpdataFlag)
		{		    
			//printf("APT+/------------------------------------/ \r\n");
			//Delay_1ms(10);
			//printf("APT+Roll: %.2f     Pitch: %.2f     Yaw: %.2f \r\n",angles[2],angles[1],angles[0]);
			//Delay_ms(10);
			//printf("APT+Acceleration: X: %d     Y: %d     Z: %d \r\n",accel[0],accel[1],accel[2]);
			//Delay_ms(10);
			//printf("APT+Gyroscope: X: %d     Y: %d     Z: %d \r\n",gyro[0],gyro[1],gyro[2]);
			//Delay_ms(10);
 			//printf("APT+Magnetic: X: %d     Y: %d     Z: %d \r\n",magn[0],magn[1],magn[2]);
			//	Delay_ms(10);
 			//printf("APT+Pressure: %.2f     Altitude:%.2f \r\n",(float)PressureVal / 100, (float)AltitudeVal / 100);
			printf("APT+Altitude:%.2f\r\n",(float)AltitudeVal / 100);
			//Delay_ms(10);
 			//printf("APT+Temperature: %.1f \r\n", (float)TemperatureVal / 10);
	
			UART_UpdataFlag = 0;
		}
		 if((Shake_Switch==1)&&(Get_through==0)&&(Shake_Delay<=0))
		 {
		  Shake_Delay=1000;
		  GPIO_WriteBit(GPIOA,GPIO_Pin_11,(BitAction)(1-GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_11)));
		 }
		 else if(Get_through==1||Shake_Switch==0)
		 {
		  GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			 
		 }
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
					break;
			case LD_ASR_NONE:
					nAsrStatus=LD_ASR_RUNING;
					if (RunASR()==0)//启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
					{		
						nAsrStatus = LD_ASR_ERROR;
					}
					break;
			case LD_ASR_FOUNDOK:
					 nAsrRes = LD_GetResult();//一次ASR识别流程结束，去取ASR识别结果										 
					 nAsrStatus = LD_ASR_NONE;
					break;
			case LD_ASR_FOUNDZERO:
			default:
					nAsrStatus = LD_ASR_NONE;
					break;
			}//switch
		//开发板测试
			Board_text(nAsrRes );
		
	}// while
}


void System_Init(void)//陀螺仪初始化
{
	__disable_irq();
	I2Cx_Init();
	IMU_Init();
	__enable_irq();
} 
void Delay_1ms(uint16_t Timing)
{
Delay_Ms=Timing;
	while(Delay_Ms);

}
void Send(uint8_t *str)     //发送字符串函数
{
	uint32_t i=0;
	while(str[i])
	{
		USART_SendData(USART1,str[i]);
		i++;
		Delay_1ms(10);
	}
	
}
static uint8 LD_AsrAddFixed(void)
{
	uint8 k, flag;
	uint8 nAsrAddLength;

	uint8  pCode[DATE_A] = {
	 															CODE_LSD,	\
																CODE_SS,	\
																CODE_AJCF,\
																CODE_QM,	\
																CODE_JT,	\
		                            CODE_CALL, \
		                            CODE_CALL1, \
	                              CODE_CALL2,	\
                                CODE_CALL3,  \
                                CODE_CALL4, \
	                              CODE_CALL5 };	//添加识别码，用户修改
	flag = 1;
	for (k=0; k<DATE_A; k++)
	{			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}

		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0);
		LD_WriteReg(0x08, 0x04);
		LD3320_delay(1);
		LD_WriteReg(0x08, 0x00);
		LD3320_delay(1);

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
	}	 
	return flag;
}

static void Board_text(uint8 Code_Val)
{if(Delay_3s<0)
	{LED2_OFF();	
		switch(Code_Val)  //对结果执行相关操作
	  {
		case CODE_LSD:  //命令“流水灯”
		{	
		 if(help_switch==1)
		 {
			 Glide_LED();
			printf("APT+help\r\n");
		  Filck_LED=1; 
		 }
		 nAsrRes=0;
		}
		break;
		case CODE_SS:	  //再见拜拜
		{if(Call_Switch==1)
			{Flicker_LED();
		   printf("AT+CG\r\n");	
       Call_Switch=0;	
       Get_through=0;
			Shake_Switch=0;				
			}
			nAsrRes=0;
		}
		break;
		case CODE_AJCF:	//命令接
		{if(Call_Switch==1&&Get_through==0)
			{Flicker_LED();
		   printf("AT+CE\r\n");
				Get_through=1;
		  }
		nAsrRes=0;
		}
		break;
		case CODE_QM:		//命令重播电话
		{if(Call_Switch==0)
			{Flicker_LED();
		   printf("AT+CH\r\n");}
		 nAsrRes=0;
		}
		break;
		case CODE_JT:		//命令挂断电话
		{	if(Call_Switch==1)
			{Flicker_LED();
			printf("AT+CF\r\n");
       Call_Switch=0;
			 Get_through=0;
			 Shake_Switch=0;
			}	
		 nAsrRes=0;
		}
		break;
		case CODE_CALL:  //命令打电话
		{if(Call_Switch==0)
		  {Delay_3s=5000;
		   nAsrRes=0;
			}
		}
		break;
		default :break;
	 }	
 }
	else
	{LED2_ON();
	switch(Code_Val)
	 {case CODE_CALL1:
		{printf("APT+CALL_1\r\n");		
			Flicker_LED();
		  nAsrRes=0;
		}
		break;
   case CODE_CALL2:
	   {printf("APT+CALL_2\r\n");		
			Flicker_LED();
		  nAsrRes=0;
	  }		 
		break;
	 case CODE_CALL3:
		 {printf("APT+CALL_3\r\n");		
			Flicker_LED();
		  nAsrRes=0;
	   }
		 break;
	 case CODE_CALL4:
		 {printf("APT+CALL_4\r\n");		
			Flicker_LED();
		  nAsrRes=0;
	   }
		 break;
	 case CODE_CALL5:
		 {printf("APT+CALL_5\r\n");		
			Flicker_LED();
		  nAsrRes=0;
	   }
		 break;
	  default:
			nAsrRes=0;
		break;	
	 }
	}
}

static void Delayms(uint16 i)
{
	unsigned char a,b;
	for(;i>0;i--)
	        for(b=4;b>0;b--)
	            for(a=113;a>0;a--);	
}

static void Glide_LED(void)
{
	LED1_ON();
	LED2_OFF();
	Delayms(0xfff);
	LED2_ON();
	LED1_OFF();
	Delayms(0xfff);
	LED2_OFF();
}

static void Flicker_LED(void)
{
	LED1_ON();
	LED2_ON();
	Delayms(0XFFF);
	LED1_OFF();
	LED2_OFF();
	Delayms(0XFFF);
}

///用户修改 end

///相关初始化
static void LD3320_init(void)
{
	LD3320_GPIO_Cfg();	
	LD3320_EXTI_Cfg();
	LD3320_SPI_cfg();
	LED_GPIO_cfg();
	LD_reset();
}

static void LD3320_GPIO_Cfg(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(LD3320RST_GPIO_CLK | LD3320CS_GPIO_CLK,ENABLE);
		//LD_CS	/RSET
		GPIO_InitStructure.GPIO_Pin =LD3320CS_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(LD3320CS_GPIO_PORT,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin =LD3320RST_PIN;
		GPIO_Init(LD3320RST_GPIO_PORT,&GPIO_InitStructure);
}

static void LD3320_EXTI_Cfg(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	

	RCC_APB2PeriphClockCmd(LD3320IRQ_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =LD3320IRQ_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LD3320IRQ_GPIO_PORT, &GPIO_InitStructure);
	//外部中断线配置
  GPIO_EXTILineConfig(LD3320IRQEXIT_PORTSOURCE, LD3320IRQPINSOURCE);
  EXTI_InitStructure.EXTI_Line = LD3320IRQEXITLINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	//中断嵌套配置
  NVIC_InitStructure.NVIC_IRQChannel = LD3320IRQN;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

static void LD3320_SPI_cfg(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  //spi端口配置
	RCC_APB2PeriphClockCmd(LD3320SPI_CLK,ENABLE);		
  RCC_APB2PeriphClockCmd(LD3320WR_GPIO_CLK | LD3320SPIMISO_GPIO_CLK | LD3320SPIMOSI_GPIO_CLK | LD3320SPISCK_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LD3320SPIMISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(LD3320SPIMISO_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LD3320SPIMOSI_PIN;
	GPIO_Init(LD3320SPIMOSI_GPIO_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LD3320SPISCK_PIN;
	GPIO_Init(LD3320SPISCK_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LD3320WR_PIN;				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LD3320WR_GPIO_PORT, &GPIO_InitStructure);
	
	LD_CS_H();
	SPI_Cmd(LD3320SPI, DISABLE);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   	//全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   						//主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   					//8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   							//时钟极性 空闲状态时，SCK保持低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   						//时钟相位 数据采样从第一个时钟边沿开始
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   							//软件产生NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;   //波特率控制 SYSCLK/128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   					//数据高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;							   							//CRC多项式寄存器初始值为7
	SPI_Init(LD3320SPI, &SPI_InitStructure);

	SPI_Cmd(LD3320SPI, ENABLE);
}

static void LED_GPIO_cfg(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK,ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = LED3_PIN;
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//初始化震动端口A11
    GPIO_Init(GPIOA,&GPIO_InitStructure);
		LED1_OFF();
		LED2_OFF();
    LED3_OFF();
    GPIO_ResetBits(GPIOA,GPIO_Pin_11);//
}
///相关初始化 end 

///中间层
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(LD3320IRQEXITLINE)!= RESET ) 
	{
		ProcessInt(); 
 	//	printf("进入中断12\r\n");	
		EXTI_ClearFlag(LD3320IRQEXITLINE);
		EXTI_ClearITPendingBit(LD3320IRQEXITLINE);//清除LINE上的中断标志位  
	} 
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)//voice substract
	{
		printf("AT+CL\r\n");
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)//voice puls
	{
		printf("AT+CK\r\n");
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)//open help function
	{
		printf("APT+help\r\n");
		Filck_LED=1;
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}
static void LD3320_delay(unsigned long uldata)
{
	unsigned int i  =  0;
	unsigned int j  =  0;
	unsigned int k  =  0;
	for (i=0;i<5;i++)
	{
		for (j=0;j<uldata;j++)
		{
			k = 200;
			while(k--);
		}
	}
}

static uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	for (i=0; i<5; i++)		//防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_AsrStart();			//初始化ASR
		LD3320_delay(100);
		if (LD_AsrAddFixed()==0)	//添加关键词语到LD3320芯片中
		{
			LD_reset();				//LD3320芯片内部出现不正常，立即重启LD3320芯片
			LD3320_delay(50);	//并从初始化开始重新ASR识别流程
			continue;
		}
		LD3320_delay(10);
		if (LD_AsrRun() == 0)
		{
			LD_reset();			 //LD3320芯片内部出现不正常，立即重启LD3320芯片
			LD3320_delay(50);//并从初始化开始重新ASR识别流程
			continue;
		}
		asrflag=1;
		break;						//ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}	
	return asrflag;
}

static void LD_reset(void)
{
	LD_RST_H();
	LD3320_delay(100);
	LD_RST_L();
	LD3320_delay(100);
	LD_RST_H();
	LD3320_delay(100);
	LD_CS_L();
	LD3320_delay(100);
	LD_CS_H();		
	LD3320_delay(100);
}

static void LD_AsrStart(void)
{
	LD_Init_ASR();
}

uint8 LD_Check_ASRBusyFlag_b2(void)
{
	uint8 j;
	uint8 flag = 0;
	for (j=0; j<10; j++)
	{
		if (LD_ReadReg(0xb2) == 0x21)
		{
			flag = 1;
			break;
		}
		LD3320_delay(10);		
	}
	return flag;
}
///中间层end


///寄存器操作
static uint8 spi_send_byte(uint8 byte)
{
	while (SPI_I2S_GetFlagStatus(LD3320SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(LD3320SPI,byte);
	while (SPI_I2S_GetFlagStatus(LD3320SPI,SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(LD3320SPI);
}

static void LD_WriteReg(uint8 data1,uint8 data2)
{
	LD_CS_L();
	LD_SPIS_L();
	spi_send_byte(0x04);
	spi_send_byte(data1);
	spi_send_byte(data2);
	LD_CS_H();
}

static uint8 LD_ReadReg(uint8 reg_add)
{
	uint8 i;
	LD_CS_L();
	LD_SPIS_L();
	spi_send_byte(0x05);
	spi_send_byte(reg_add);
	i=spi_send_byte(0x00);
	LD_CS_H();
	return(i);
}

static uint8 LD_GetResult(void)
{
	return LD_ReadReg(0xc5);
}

static uint8 LD_AsrRun(void)
{
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	LD3320_delay( 5 );
	LD_WriteReg(0x08, 0x00);
	LD3320_delay( 5);

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff);	
	LD_WriteReg(0x37, 0x06);
	LD_WriteReg(0x37, 0x06);
	LD3320_delay(5);
	LD_WriteReg(0x1C, 0x0b);
	LD_WriteReg(0x29, 0x10);
	LD_WriteReg(0xBD, 0x00);   
	return 1;
}

static void ProcessInt(void)
{
	uint8 nAsrResCount=0;

	ucRegVal = LD_ReadReg(0x2B);

// 语音识别产生的中断
//（有声音输入，不论识别成功或失败都有中断）
	LD_WriteReg(0x29,0) ;
	LD_WriteReg(0x02,0) ;

	if((ucRegVal & 0x10) && LD_ReadReg(0xb2)==0x21 && LD_ReadReg(0xbf)==0x35)		
	{	 
			nAsrResCount = LD_ReadReg(0xba);

			if(nAsrResCount>0 && nAsrResCount<=4) 
			{
				nAsrStatus=LD_ASR_FOUNDOK; 				
			}
			else
			{
				nAsrStatus=LD_ASR_FOUNDZERO;
			}	
	}
	else
	{
		nAsrStatus=LD_ASR_FOUNDZERO;//执行没有识别
	}

	LD_WriteReg(0x2b,0);
	LD_WriteReg(0x1C,0);//写0:ADC不可用
	LD_WriteReg(0x29,0);
	LD_WriteReg(0x02,0);
	LD_WriteReg(0x2B,0);
	LD_WriteReg(0xBA,0);	
	LD_WriteReg(0xBC,0);	
	LD_WriteReg(0x08,1);//清除FIFO_DATA
	LD_WriteReg(0x08,0);//清除FIFO_DATA后 再次写0
}

static void LD_Init_Common(void)
{
	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	LD3320_delay(5);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03);  
	LD3320_delay(5);
	LD_WriteReg(0xCF, 0x43);   
	LD3320_delay(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E, 0x00); 
		LD_WriteReg(0x19, LD_PLL_MP3_19);   
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);   
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19); 
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
	  LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}
	LD3320_delay(5);
	
	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c); 
	LD3320_delay(1);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}

static void LD_Init_ASR(void)
{
	nLD_Mode=LD_MODE_ASR_RUN;
	LD_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);	
	LD3320_delay(5);
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
	LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8); 
	LD3320_delay( 1 );
}
///寄存器操作 end
/*********************************************END OF FILE**********************/
