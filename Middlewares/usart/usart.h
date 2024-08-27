#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_DEBUG		USART1		//调试打印所使用的串口
////////////////////////////////////////////////////////////////////////////////// 	
//#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	//引脚定义
/*******************************************************/
#define USARTx                             USART2

/* 不同的串口挂载的总线不一样，时钟使能函数也不一样，移植时要注意 
 * 串口1和6是      RCC_APB2PeriphClockCmd
 * 串口2/3/4/5/7是 RCC_APB1PeriphClockCmd
 */
#define USARTx_CLK                         RCC_APB1Periph_USART2
#define USARTx_CLOCKCMD                    RCC_APB1PeriphClockCmd
#define USARTx_BAUDRATE                    115200  //串口波特率

#define USARTx_RX_GPIO_PORT                GPIOA
#define USARTx_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define USARTx_RX_PIN                      GPIO_Pin_3
#define USARTx_RX_AF                       GPIO_AF_USART2
#define USARTx_RX_SOURCE                   GPIO_PinSource3

#define USARTx_TX_GPIO_PORT                GPIOA
#define USARTx_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define USARTx_TX_PIN                      GPIO_Pin_2
#define USARTx_TX_AF                       GPIO_AF_USART2
#define USARTx_TX_SOURCE                   GPIO_PinSource2

/************************************************************/
#define SEND_BUF_SIZE 200
extern u8 SendBuff[SEND_BUF_SIZE];	
void USARTx_Config(void);  	

////如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);

void Usart1_SendString(unsigned char *str, unsigned short len);
void Usart2_SendString(unsigned char *str, unsigned short len);
void Usart1_Send(u8 *buf,u8 len);
void UsartPrintf(char *fmt,...);

void DEBUG_LOG(char *fmt,...);
#endif


