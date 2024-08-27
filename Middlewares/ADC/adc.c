#include "adc.h"
#include "delay.h"		 






/********************************************************************************

//初始化ADC		 

//STM32F4x1开发板
//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com								  

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

	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;	   //不使用DMA
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;					   //独立工作模式
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;				   //分频，建议分频后小于12MHZ
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles; //采样延迟，多ADC使用才有效
	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;							 //连续模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;						 //右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;		 //选择外部触发事件，这里随意即可，因为使用了不允许外部触发
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //软件触发
	ADC_InitStruct.ADC_NbrOfConversion = 1;									 //通道数量
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;						 //分辨率
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;								 //扫描模式
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1, ENABLE);

}		


u16 Get_Adc(u8 ch)   
{
	
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );				//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);																						//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));													//等待转换结束

	return ADC_GetConversionValue(ADC1);																		//返回最近一次ADC1规则组的转换结果
	
	
}



/********************************************************************************


//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值


//STM32F4x1开发板
//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com								  

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
	gas_val = Get_Adc_Average(ADC_Channel_1,10);//得到ADC通道获取的平均值
	temp = ((gas_val/(4095.0))*3.3);//燃气值计算公式可以查看MQ-5数据手册
	*value = 51.0*temp*temp-11.0*temp+10.0;
}
































/********************************************************************************


//STM32F4x1开发板
//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com								  

*******************************************************************************/




