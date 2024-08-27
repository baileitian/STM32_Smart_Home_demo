#include "main.h"  //ϵͳͷ�ļ�
#include "dht11.h"
#include "adc.h"
#include "esp8266.h"
#include "onenet.h"
#include "i2c_bh1750_gpio.h"
#include "i2c_bh1750.h"
#include "gpio.h"
#include "tim.h"

void access_control(int mode); //0����,1����
void publish_tip();//��������
void alarm_set(void);//���ñ�������
void ESP8266_init();//
void breathing_lamp();//������
void Fan_speed_set(int speed_level);//���ȵ��� 0,1,2,3
	
char PUB_BUF[256];//�ϴ����ݵ�buf
const char *devSubTopic[] = {"/smart/sub"};//���ĵ�MQTT����
const char devPubTopic[] = "/smart/pub";//������MQTT����
u8 ESP8266_INIT_OK = 0;//esp8266��ʼ����ɱ�־

u8 clear_control=0;//������Ļ�Ƿ�����
u8 x=0,y=0;//���ڶ�λ����λ�ã��������ö�ʱʱ��
int alarm[3]={30,200,40};//�¶ȡ�����ǿ�Ⱥ�����Ũ�ȵ����ֵ
int light_limit = 20; 

/*���ƺ�����*/
static uint8_t pwmset; 
static uint16_t time;
static uint8_t timecount;

char oledBuf[100];//oled ������
DHT11_Data_TypeDef DHT11_Data;//��ʪ�ȴ���������
float gas_value = 0.0;//����Ũ��
float illumination = 0.0;//����ǿ��
int body_status = 0;//�����Ӧ��1�������ˡ�0����û��
int key_number = 0;//��ȡ��������ֵ
int liv_LED_flag = 1; //���ҵƱ�־λ 1����� 0����
int bed_LED_flag = 1;//�����Ʊ�־λ 1����� 0����
int illumination_flag = 1; //������־λ 1�������� 0�����
int fanflag = 0; //���ȱ�־λ 1���� 0�����
int time_count = 0; //���������ϴ�ʱ��
int main(void)
{
	unsigned char *dataPtr = NULL;

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  delay_ms(50);           // ��ʱ����--50mS
	uart_init(115200);		//��ʼ������	
	
	GPIO_Config();          // ��ʼ�� ��Ӧ������
	
	LCD_Init();             // ��ʼ�� 1.8��LCDҺ���� SPI �ӿ�
	GBK_Lib_Init();         // Ӳ��GBK�ֿ��ʼ��--(���ʹ�ò����ֿ��Һ�����汾���˴��������Σ������ֿ��ʼ����
	LCD_Clear(WHITE);       // ����

	KEY_Init();             //��ʼ���밴�����ӵ�Ӳ���ӿ�
  boma_Init(); //��ʼ������
	DHT11_GPIO_Config();
	Adc_Init();         //��ʼ��ADC-- ( PA1-ADC1_CH1 )

	TIM4_PWM_Init(200-1,8400-1);//���
	TIM3_PWM_Init(200-1,8400-1);//����
	TIM_SetCompare1(TIM3,0);
	TIM5_Int_Init(20-1,840-1);
	TIM_Cmd(TIM5,DISABLE);//�ض�ʱ��
	if(i2c_CheckDevice(BH1750_Addr)==0)
		printf("BH1750�豸���ɹ�\r\n");
	else printf("BH1750�豸���ʧ��\r\n");
	BH1750_Init();
	
	bed_LED_OFF;//Ĭ�ϵƹ�
	liv_LED_OFF;
	
	ESP8266_init();
  while (1)
  { 
			clear_control=0;//���ڿ�����Ļ�Ƿ�����
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==1)//����1���ϣ��������ý���
			{
					delay_ms(1);//����
					if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==1)
					{ 
						alarm_set();//���ö�ʱʱ��
					}
			}
			if(clear_control==1)//������ʱ���˳�����
			{
					LCD_Clear(WHITE);       // ����
			}
			get_gas_real_value(&gas_value);//����Ũ�Ȼ�ȡ
			Read_DHT11(&DHT11_Data);//��ʪ�Ȼ�ȡ
			illumination = LIght_Intensity();//����ǿ�Ȼ�ȡ
			body_status =	HC_SR501_READ(); //��ȡ�����Ӧ״̬
			key_number = KEY_Scan(0);		//�õ���ֵ
		//	printf("temp:%d.%d , humi:%d.%d\r\n",DHT11_Data.temp_int,DHT11_Data.temp_deci,DHT11_Data.humi_int,DHT11_Data.humi_deci);
			//printf("BH1750:%0.2f\r\n",BH1750_GetData());
			//TIM_SetCompare1(TIM3,50);
			//printf("BH1750:%0.2f lx\r\n",LIght_Intensity());
			//printf("HC_SR501:%d\r\n",body_status);
		
			sprintf(oledBuf,"���ܼҾ�");
			Draw_Font16B(46,2,BLACK,oledBuf);
			sprintf(oledBuf,"�¶�:   %d.%d",DHT11_Data.temp_int,DHT11_Data.temp_deci);
			Draw_Font16B(0,20,BLACK,oledBuf);
			sprintf(oledBuf,"ʪ��:   %d.%d",DHT11_Data.humi_int,DHT11_Data.humi_deci);
			Draw_Font16B(0,37,BLACK,oledBuf);
			sprintf(oledBuf,"����ǿ��: %0.2f ",illumination);
			Draw_Font16B(0,54,BLACK,oledBuf);
			sprintf(oledBuf,"����Ũ��: %0.2f ",gas_value);
			Draw_Font16B(0,71,BLACK,oledBuf);
			sprintf(oledBuf,"����״̬:");
			Draw_Font16B(0,88,BLACK,oledBuf);
			
			switch(body_status)
			{
				case 0:	
					      sprintf(oledBuf,"����");
								Draw_Font16B(78,88,GREEN,oledBuf);break;
			
				case 1:	sprintf(oledBuf,"����");
								Draw_Font16B(78,88,RED,oledBuf);break;
			}
			
			sprintf(oledBuf,"����״̬:");
			Draw_Font16B(0,105,BLACK,oledBuf);
			
			switch(illumination_flag)
			{
				case 1:	
					      sprintf(oledBuf,"��");
								Draw_Font16B(78,105,GREEN,oledBuf);break;
			
				case 0:	sprintf(oledBuf,"��");
								Draw_Font16B(78,105,RED,oledBuf);break;
			}
			
			/*�Զ�����*/
			
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)==1)//�ֶ�ģʽ
		{
			//printf("manual mode\r\n");
		}
		else//�Զ�ģʽ
		{
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)//����1
			{
				delay_ms(10);//����
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
				{
					if((int)illumination < light_limit)
					{
							if(body_status == 1 && liv_LED_flag == 1)//�����߹�ȥ
							{
								printf("liv_LED_ON\r\n");
								liv_LED_ON;
								liv_LED_flag = 0;//���������ó�������ִֻ��һ��
							}
						}
					}
			}
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)//����2
			{
				delay_ms(10);//����
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)
				{
					if((int)illumination < light_limit)
					{
							if(body_status == 1&& bed_LED_flag == 1 )//�����߹�ȥ
							{
									printf("bed_LED_ON\r\n");
									breathing_lamp();
									bed_LED_flag = 0;
							}
					}
				}
			}
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)== 0)//����3
			{
				delay_ms(10);//����
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
				{
						if(illumination >= alarm[1] && illumination_flag == 1)//�ش���
						{
								access_control(0);//��
								illumination_flag = 0;
												
						}
						else if(illumination < alarm[1] && illumination_flag == 0)//������
						{
							access_control(1);//��
							illumination_flag = 1;
						}

				}
			}
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)== 1)//����4
			{
				delay_ms(10);//����
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)
				{

						if(illumination < alarm[1] && illumination_flag == 1)//�ش���������
						{
								access_control(0);//��
								illumination_flag = 0;
						}
						liv_LED_ON;

				}
			}
								/*�¶ȿ��Ʒ���*/
			if(DHT11_Data.temp_int >= alarm[0])//������
			{
					Fan_speed_set(1);//��
				  fanflag = 1; 
			}
			else if(DHT11_Data.temp_int < alarm[0])
			{
					Fan_speed_set(0);
					fanflag = 0; 
			}
			
			/*����Ũ�ȿ����ŷ��Ⱥͱ���*/
			if(gas_value >= alarm[2])//
			{
					exhaustfan_ON;
					BEEP_ON;
			}
			else if(gas_value < alarm[2])
			{
					exhaustfan_OFF;
					BEEP_OFF;
			}
			
		}
			

			/*���ذ�������*/
			if(key_number == K1_PRES && illumination_flag == 0)//left ������
			{
				access_control(1);
				illumination_flag = 1;
			}
			if(key_number == K3_PRES && illumination_flag == 1)//right �ش���
			{
				access_control(0);
				illumination_flag = 0;
			}
			if(key_number == K0_PRES)//dowm �����˷���
			{
				bed_LED_OFF;
				bed_LED_flag = 1;
			}
			if(key_number == K2_PRES)//up �ش�����
			{
				liv_LED_OFF;
				liv_LED_flag = 1;
			}
			
			if(time_count > 3)//200ms����һ��
			{
					publish_tip();//�ϴ����ݵ��ƶ�
					time_count=0;
			}
		
		
			//printf("key:%d\r\n",KEY_Scan(0));
		
			/*�������յ�mqtt��Ϣ*/
			dataPtr = ESP8266_GetIPD(3);
			if(dataPtr != NULL)
			 OneNet_RevPro(dataPtr);

			time_count++;
			delay_ms(50);
			
			
	}
}

void access_control(int mode) //0��,1��
{
	if(mode == 1)//��
	{
		TIM_SetCompare4(TIM4,22);
		delay_ms(350);
		TIM_SetCompare4(TIM4,15);// ���ñȽϼĴ�����ֵ
	}
	if(mode == 0)
	{
		TIM_SetCompare4(TIM4,7);
		delay_ms(350);
		TIM_SetCompare4(TIM4,15);// ���ñȽϼĴ�����ֵ
	}
}

void publish_tip()//�ϴ����ݺ���
{
			float temp = DHT11_Data.temp_int+(DHT11_Data.temp_deci/10.0);//�¶�
			float humi = DHT11_Data.humi_int+(DHT11_Data.humi_deci/10.0);//ʪ��
			//������״̬ ������ȡ������Ϊ��APP 1 ��ʾ�� 0��ʾ��
			int liv_LED_status = !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
			int bed_LED_status = !bed_LED_flag;//���ҵ�״̬
	    int fan_status = fanflag;//����״̬
			int window_status = illumination_flag;//����״̬
//			DEBUG_LOG("==================================================================================");
//			DEBUG_LOG("�������� ----- OneNet_Publish");
			sprintf(PUB_BUF,"{\"bed_LED\":%d,\"Temp\":%0.1f,\"humi\":%0.1f,\"liv_LED\":%d,\"gas\":%0.2f,\"light\":%0.2f,\"fan\":%d,\"window\":%d }",
	   	bed_LED_status,temp,humi,liv_LED_status,gas_value,illumination,fan_status,window_status);
			OneNet_Publish(devPubTopic, PUB_BUF);
//			DEBUG_LOG("==================================================================================");
			ESP8266_Clear();
}

void alarm_set(void)//���ñ�������
{
		int key;
		
	//���Ƶķ�Χ
		x%=3;
		alarm[0]%=100;
		alarm[1]%=1000;
		alarm[2]%=100;
	
		if(clear_control==0)
		{
			   LCD_Clear(WHITE);       // ����
		}
		clear_control=1;//�������˳�ȥ��Ҫ����һ��
		
		sprintf(oledBuf,"��������");
    Draw_Font16B(32,0,BLACK,oledBuf);
		sprintf(oledBuf,"�¶�:%d",alarm[0]);
		Draw_Font16B(0,30,BLACK,oledBuf);
		sprintf(oledBuf,"����ǿ��:%d",alarm[1]); 
		Draw_Font16B(0,60,BLACK,oledBuf);
		sprintf(oledBuf,"����Ũ��:%d",alarm[2]);
		Draw_Font16B(0,90,BLACK,oledBuf);

		key=KEY_Scan(0);		//�õ���ֵ

	  if(key)
		{
			switch(key)
			{				 
				case K3_PRES:	//����right����
						++x;
					break;
				
				case K0_PRES:	//����dowm����
						--alarm[x];
					break;
				
				case K1_PRES:	//����left����
							--x;
					break;
				
				case K2_PRES:	//����up����
					++alarm[x];
					break;
				
			}
		}

} 

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //����ж�
	{                                    
	
		time ++;

		/* ռ�ձ����� */
		pwmset = time/500;

		/* 5ms ���� */
		if(timecount > 50) timecount = 0;
			else timecount ++;

		if(timecount >= pwmset ) 
			GPIO_SetBits(LED_PORT,LED_PIN);
		else 
			GPIO_ResetBits(LED_PORT,LED_PIN);
		
		if(time>16000)
		{
			printf("TIM5,DISABLE\r\n");
			TIM_Cmd(TIM5,DISABLE);//�ض�ʱ��
			GPIO_ResetBits(LED_PORT,LED_PIN);
		}

	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //����жϱ�־λ
}
void ESP8266_init()
{
	USARTx_Config();//stm32-8266ͨѶ����
	if(!ESP8266_INIT_OK){
		sprintf(oledBuf,"Waiting For");
		Draw_Font16B(0,64,RED,oledBuf);
		sprintf(oledBuf,"WiFi");
		Draw_Font16B(0,80,RED,oledBuf);
		sprintf(oledBuf,"Connection");
		Draw_Font16B(0,96,RED,oledBuf);
	}
	ESP8266_Init();					//��ʼ��ESP8266
	LCD_Clear(WHITE);       // ����
	
	sprintf(oledBuf,"Waiting For");
	Draw_Font16B(0,64,RED,oledBuf);
	sprintf(oledBuf,"MQTT Server");
	Draw_Font16B(0,80,RED,oledBuf);
	sprintf(oledBuf,"Connection");
	Draw_Font16B(0,96,RED,oledBuf);
	while(OneNet_DevLink()){//����OneNET
	delay_ms(500);
	}	
	BEEP_ON;//������ʾ����ɹ�
	delay_ms(250);
	BEEP_OFF;
	OneNet_Subscribe(devSubTopic, 1);//����mqtt����
	LCD_Clear(WHITE);       // ����
}

void breathing_lamp()//�򿪺�����
{
		TIM_Cmd(TIM5,ENABLE);//����ʱ��
		pwmset = 0;
		time = 0;
		timecount = 0;
}

void Fan_speed_set(int speed_level)//���ȵ��� 0,1,2,3
{
	if(speed_level > 3)//��������ٶ�
	{
		speed_level = 3;
	}
		switch(speed_level) {
        case 0:
            TIM_SetCompare1(TIM3, 0);
            break;
        case 1:
            TIM_SetCompare1(TIM3, 40);
            break;
        case 2:
            TIM_SetCompare1(TIM3, 45);
            break;
        case 3:
						TIM_SetCompare1(TIM3, 50);
            break;
        default:
						TIM_SetCompare1(TIM3, 0);
            break;
    }
}
