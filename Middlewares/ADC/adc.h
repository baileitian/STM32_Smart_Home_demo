#ifndef __ADC_H
#define __ADC_H	


#include "stdio.h"	
#include "stm32f4xx_conf.h"

 
 
 



void Adc_Init(void); 										//ADC通道初始化
u16  Get_Adc(u8 ch); 										//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);		//得到某个通道给定次数采样的平均值  
void get_gas_real_value(float* value);



#endif 




















/********************************************************************************


//STM32F4x1开发板
//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com								  

*******************************************************************************/



