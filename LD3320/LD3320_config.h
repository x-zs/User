#ifndef _LD3320_CONFIG_H__
#define _LD3320_CONFIG_H__

///识别码（客户修改处）
#define CODE_LSD	1	 //呼救
#define CODE_SS	  2	 //拒接电话
#define CODE_AJCF	3	 //接电话
#define CODE_QM	  4	 //重播
#define CODE_JT		5  //再见
#define CODE_CALL 6  //挂
#define CODE_CALL1 7  //第一个联系人 
#define CODE_CALL2 8 //第二个联系人
#define CODE_CALL3 9 //第三个联系人
#define CODE_CALL4 10
#define CODE_CALL5 11

///LED引脚相关定义
#define LED1_PIN							GPIO_Pin_13
#define LED1_GPIO_PORT				GPIOD
#define LED1_GPIO_CLK					RCC_APB2Periph_GPIOD

#define LED2_PIN							GPIO_Pin_14
#define LED2_GPIO_PORT				GPIOG
#define LED2_GPIO_CLK					RCC_APB2Periph_GPIOG

#define LED3_PIN							GPIO_Pin_12
#define LED3_GPIO_PORT				GPIOD
#define LED3_GPIO_CLK					RCC_APB2Periph_GPIOD

#define  LED1_ON()								GPIO_SetBits(GPIOD,GPIO_Pin_13)
#define LED2_ON()								GPIO_SetBits(GPIOG,GPIO_Pin_14)
#define  LED3_ON()								GPIO_SetBits(GPIOD,GPIO_Pin_12)

#define LED1_OFF()							GPIO_ResetBits(GPIOD,GPIO_Pin_13)
#define LED2_OFF()							GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define LED3_OFF()							GPIO_ResetBits(GPIOD,GPIO_Pin_12)


///LD3320引脚相关定义
#define LD3320RST_PIN					GPIO_Pin_15		
#define LD3320RST_GPIO_PORT		GPIOB
#define LD3320RST_GPIO_CLK		RCC_APB2Periph_GPIOB
#define LD_RST_H() 						GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define LD_RST_L() 						GPIO_ResetBits(GPIOB, GPIO_Pin_15)

#define LD3320CS_PIN					GPIO_Pin_4		
#define LD3320CS_GPIO_PORT		GPIOA
#define LD3320CS_GPIO_CLK			RCC_APB2Periph_GPIOA
#define LD_CS_H()							GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define LD_CS_L()							GPIO_ResetBits(GPIOA, GPIO_Pin_4)

#define LD3320IRQ_GPIO_CLK		RCC_APB2Periph_GPIOB
#define LD3320IRQ_PIN					GPIO_Pin_12
#define LD3320IRQ_GPIO_PORT		GPIOB
#define LD3320IRQEXIT_PORTSOURCE		GPIO_PortSourceGPIOB
#define LD3320IRQPINSOURCE		GPIO_PinSource12
#define LD3320IRQEXITLINE			EXTI_Line12
#define LD3320IRQN						EXTI15_10_IRQn

#define LD3320WR_PIN					GPIO_Pin_13
#define LD3320WR_GPIO_PORT		GPIOB
#define LD3320WR_GPIO_CLK			RCC_APB2Periph_GPIOB
#define LD_SPIS_H()  					GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define LD_SPIS_L()  					GPIO_ResetBits(GPIOB, GPIO_Pin_13)

#define	LD3320SPI							SPI1
#define LD3320SPI_CLK					RCC_APB2Periph_SPI1						

#define LD3320SPIMISO_PIN					GPIO_Pin_6
#define LD3320SPIMISO_GPIO_PORT		GPIOA
#define LD3320SPIMISO_GPIO_CLK		RCC_APB2Periph_GPIOA

#define LD3320SPIMOSI_PIN					GPIO_Pin_7
#define LD3320SPIMOSI_GPIO_PORT		GPIOA
#define LD3320SPIMOSI_GPIO_CLK		RCC_APB2Periph_GPIOA

#define LD3320SPISCK_PIN					GPIO_Pin_5
#define LD3320SPISCK_GPIO_PORT		GPIOA
#define LD3320SPISCK_GPIO_CLK			RCC_APB2Periph_GPIOA

#endif
