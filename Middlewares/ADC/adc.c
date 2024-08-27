#include "adc.h"
#include "delay.h"		 






/********************************************************************************

//��ʼ��ADC		 

//STM32F4x1������
//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com								  

*******************************************************************************/
														   
void  Adc_Init(void)
{    
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;	   //��ʹ��DMA
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;					   //��������ģʽ
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;				   //��Ƶ�������Ƶ��С��12MHZ
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles; //�����ӳ٣���ADCʹ�ò���Ч
	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;							 //����ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;						 //�Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;		 //ѡ���ⲿ�����¼����������⼴�ɣ���Ϊʹ���˲������ⲿ����
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //�������
	ADC_InitStruct.ADC_NbrOfConversion = 1;									 //ͨ������
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;						 //�ֱ���
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;								 //ɨ��ģʽ
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1, ENABLE);

}		


u16 Get_Adc(u8 ch)   
{
	
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );				//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);																						//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));													//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);																		//�������һ��ADC1�������ת�����
	
	
}



/********************************************************************************


//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ


//STM32F4x1������
//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com								  

*******************************************************************************/

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 


void get_gas_real_value(float* value)
{
	u16 gas_val=0;
	float temp= 0.0;
	gas_val = Get_Adc_Average(ADC_Channel_1,10);//�õ�ADCͨ����ȡ��ƽ��ֵ
	temp = ((gas_val/(4095.0))*3.3);//ȼ��ֵ���㹫ʽ���Բ鿴MQ-5�����ֲ�
	*value = 51.0*temp*temp-11.0*temp+10.0;
}
































/********************************************************************************


//STM32F4x1������
//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com								  

*******************************************************************************/




