#ifndef __KEY_H
#define __KEY_H	 

#include "stdio.h"	
#include "stm32f4xx_conf.h"


/*下面的方式是通过直接操作库函数方式读取IO*/

#define  K0_ID 	  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)	//对应DOWN按键
#define  K1_ID 	  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)	//对应LEFT按键
#define  K2_ID 	  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)	//对应up按键
#define  K3_ID 	  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)	//对应RIGHT按键

/*下面方式是通过位带操作方式读取IO*/
/*
#define WK_UP 	PAin(0)		//PA0
*/


#define K0_PRES   1
#define K1_PRES   2
#define K2_PRES   3
#define K3_PRES   4

void KEY_Init(void);	//IO初始化

u8 KEY_Scan(u8);  		//按键扫描函数	
void boma_Init(void);

#endif


