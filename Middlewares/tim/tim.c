#include "tim.h"


/*********************************************************************************

  * @brief  Configures the TIM IRQ Handler. ���ö�ʱ��
  * @param  None
  * @retval None
	
	
********************************************************************************/
	
	
void TIM3_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  uint16_t PrescalerValue = 0;
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000000) - 1; // 1Mhz

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 100;  // 0.1ms
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
  
  /* TIM Interrupts enable */
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
  
  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE); 
}



/*********************************************************************************

  * @brief  ���� PWM ���
  * @param  None
  * @retval None
	
	
********************************************************************************/

//void TIM3_IT_Update_Callback()
//{
//	
//	
//#if !soft_pwm
//	
//	/* LED PC13 �����Ʋ��� */
//	
//	static uint8_t pwmset;
//	static uint16_t time;
//	static uint8_t timeflag;
//	static uint8_t timecount;

//	 /* ������ */
//	if(timeflag == 0)
//	{
//		time ++;
//		if(time >= 16000) timeflag = 1;
//	}
//	else
//	{
//		time --;
//		if(time == 0) timeflag = 0;
//	}

//	/* ռ�ձ����� */
//	pwmset = time/320;

//	/* 5ms ���� */
//	if(timecount > 50) timecount = 0;
//		else timecount ++;

//	if(timecount >= pwmset ) 
//		GPIO_SetBits(LED_PORT,LED_PIN);
//	else 
//		GPIO_ResetBits(LED_PORT,LED_PIN);
//	
//	//GPIO_ToggleBits(LED_PORT,LED_PIN);
//	
//#endif
//	
//	
//}



/********************************************************************************

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!


//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}








/********************************************************************************

//��ʱ��3�жϷ�����


//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/

//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
//	{
//		LED2=!LED2;//DS1��ת
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
//}



/********************************************************************************

//TIM2 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��


//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/


void TIM2_PWM_Init(u32 arr,u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	  //TIM2ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PBʱ��	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2); //PB10����Ϊ��ʱ��2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;           //PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB10
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ����ʱ��2
	
	//��ʼ��TIM2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC3 ( PB10-Tim2��CH3ͨ������Ҫ��ʼ��OS3 )

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ��� ( PB10-Tim2��CH3ͨ������Ҫ��ʼ��OS3 )
 
  TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
 
										  
}  







/********************************************************************************

//��ʱ��5ͨ��1���벶������ PA0 -- K0
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��


//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/

TIM_ICInitTypeDef  TIM5_ICInitStructure;


void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  		//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ�� PA ʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 						//PA0---��ӦK0����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 				//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 				  //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 								//��ʼ��PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);			 //PA0����λ��ʱ��5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  									//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   										//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	

	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; 								 //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;			 //�½��ز���
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	 //ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;						 //���������Ƶ,����Ƶ 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;														//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5

 
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	
}



/********************************************************************************

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)


//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/




u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM5CH1_CAPTURE_VAL;	  //���벶��ֵ(TIM2/TIM5��32λ)

////��ʱ��5�жϷ������	 
//void TIM5_IRQHandler(void)
//{ 		    

// 	if((TIM5CH1_CAPTURE_STA&0x80)==0)                		 //��δ�ɹ�����	
//	{
//		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)		//���
//		{	     
//			if(TIM5CH1_CAPTURE_STA&0x40)											//�Ѿ����񵽸ߵ�ƽ��
//			{
//				if((TIM5CH1_CAPTURE_STA&0x3F)==0x3F)						//�ߵ�ƽ̫����
//				{
//					TIM5CH1_CAPTURE_STA|=0x80;										//��ǳɹ�������һ��
//					TIM5CH1_CAPTURE_VAL=0xFFFFFFFF;
//				}
//				else TIM5CH1_CAPTURE_STA++;
//			}	 
//		}
//		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)						//����1���������¼�
//		{	
//			if(TIM5CH1_CAPTURE_STA&0x40)														//����һ���½��� 		
//			{	  			
//				TIM5CH1_CAPTURE_STA|=0x80;														//��ǳɹ�����һ�ε͵�ƽ����
//			  TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);						//��ȡ��ǰ�Ĳ���ֵ.
//	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling); 	//CC1P=0 ����Ϊ�½��ز���
//			}
//			else  																									//��δ��ʼ,��һ�β���������
//			{
//				TIM5CH1_CAPTURE_STA=0;																//���
//				TIM5CH1_CAPTURE_VAL=0;
//				TIM5CH1_CAPTURE_STA|=0X40;														//��ǲ������½���
//				TIM_Cmd(TIM5,DISABLE ); 															//�رն�ʱ��5
//	 			TIM_SetCounter(TIM5,0);
//	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);		//CC1P=1 ����Ϊ�����ز���
//				TIM_Cmd(TIM5,ENABLE ); 																//ʹ�ܶ�ʱ��5
//			}		    
//		}			     	    					   
// 	}
//	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
//}

void TIM4_PWM_Init(u16 arr,u16 psc)//���
{
	GPIO_InitTypeDef GPIO_InitStrue;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); //PB9����Ϊ��ʱ��2
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStrue);
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
	TIM_Cmd(TIM4,ENABLE);
}



void TIM3_PWM_Init(u16 arr,u16 psc)//����
{

	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOF9����Ϊ��ʱ��14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��4
	
	//��ʼ��TIM3 Channe6 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM3, ENABLE);  //

}

void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///ʹ��TIM5ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM5
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM5,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
