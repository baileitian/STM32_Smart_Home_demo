#ifndef __KEY_H
#define __KEY_H	 

#include "stdio.h"	
#include "stm32f4xx_conf.h"


/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/

#define  K0_ID 	  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)	//��ӦDOWN����
#define  K1_ID 	  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)	//��ӦLEFT����
#define  K2_ID 	  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)	//��Ӧup����
#define  K3_ID 	  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)	//��ӦRIGHT����

/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define WK_UP 	PAin(0)		//PA0
*/


#define K0_PRES   1
#define K1_PRES   2
#define K2_PRES   3
#define K3_PRES   4

void KEY_Init(void);	//IO��ʼ��

u8 KEY_Scan(u8);  		//����ɨ�躯��	
void boma_Init(void);

#endif


