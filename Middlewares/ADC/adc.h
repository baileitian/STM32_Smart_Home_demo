#ifndef __ADC_H
#define __ADC_H	


#include "stdio.h"	
#include "stm32f4xx_conf.h"

 
 
 



void Adc_Init(void); 										//ADCͨ����ʼ��
u16  Get_Adc(u8 ch); 										//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);		//�õ�ĳ��ͨ����������������ƽ��ֵ  
void get_gas_real_value(float* value);



#endif 




















/********************************************************************************


//STM32F4x1������
//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com								  

*******************************************************************************/



