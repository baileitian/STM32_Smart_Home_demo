	

#ifndef __tim_H
#define __tim_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f4xx.h"
#include "main.h"


void TIM3_PWM_Init(u16 arr,u16 psc);//����
void TIM4_PWM_Init(u16 arr,u16 psc);//���
void TIM5_Int_Init(u16 arr,u16 psc);
	 
#ifdef __cplusplus
}
#endif

#endif


























	
/********************************************************************************

//���빦�ܣ�  ��׼�� TIM ��ʱ

//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/







