	

#ifndef __tim_H
#define __tim_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f4xx.h"
#include "main.h"


void TIM3_PWM_Init(u16 arr,u16 psc);//风扇
void TIM4_PWM_Init(u16 arr,u16 psc);//舵机
void TIM5_Int_Init(u16 arr,u16 psc);
	 
#ifdef __cplusplus
}
#endif

#endif


























	
/********************************************************************************

//代码功能：  标准库 TIM 定时

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/







