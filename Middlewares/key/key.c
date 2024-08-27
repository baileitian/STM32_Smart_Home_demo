#include "key.h"
#include "delay.h" 
//////////////////////////////////////////////////////////////////////////////////	 

// STM32F4x1开发板
//按键输入驱动代码	 
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
							  
////////////////////////////////////////////////////////////////////////////////// 	 

//按键初始化函数
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);			//使能GPIOA,时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_4;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;							//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;						  //上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);										//初始化GPIOA
 
	
} 

void boma_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);			//使能GPIOA,时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12|GPIO_Pin_15;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;							//普通输入模式
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;						  //下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOA
	

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);			//使能GPIOA,时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;									
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;							//普通输入模式
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;						  //下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);										//初始化GPIOA
	
} 


/********************************************************************************

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，K0按下 --对应K0按键

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/



u8 key_up=1;//按键按松开标志

u8 KEY_Scan(u8 mode)
{	 
	
	if(mode)key_up=1;  			//支持连按		  
	if(key_up&&(K0_ID==0||K1_ID==0||K2_ID==0||K3_ID==0))
	{
		delay_ms(10);					//去抖动 
		key_up=0;
		if(K0_ID==0)return 1;
		else if(K1_ID==0)return 2;
		else if(K2_ID==0)return 3;
		else if(K3_ID==0)return 4;
	}
	else if(K0_ID==1&&K1_ID==1&&K2_ID==1&&K3_ID==1)key_up=1;
	
 	return 0;								// 无按键按下
	
}
