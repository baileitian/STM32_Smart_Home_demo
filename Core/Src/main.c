#include "main.h"  //系统头文件
#include "dht11.h"
#include "adc.h"
#include "esp8266.h"
#include "onenet.h"
#include "i2c_bh1750_gpio.h"
#include "i2c_bh1750.h"
#include "gpio.h"
#include "tim.h"

void access_control(int mode); //0关门,1开门
void publish_tip();//发布主题
void alarm_set(void);//设置报警上限
void ESP8266_init();//
void breathing_lamp();//呼吸灯
void Fan_speed_set(int speed_level);//风扇调速 0,1,2,3
	
char PUB_BUF[256];//上传数据的buf
const char *devSubTopic[] = {"/smart/sub"};//订阅的MQTT主题
const char devPubTopic[] = "/smart/pub";//发布的MQTT主题
u8 ESP8266_INIT_OK = 0;//esp8266初始化完成标志

u8 clear_control=0;//控制屏幕是否清屏
u8 x=0,y=0;//用于定位按键位置，方便设置定时时间
int alarm[3]={30,200,40};//温度、光照强度和气体浓度的最大值
int light_limit = 20; 

/*控制呼吸灯*/
static uint8_t pwmset; 
static uint16_t time;
static uint8_t timecount;

char oledBuf[100];//oled 缓存区
DHT11_Data_TypeDef DHT11_Data;//温湿度传感器数据
float gas_value = 0.0;//气体浓度
float illumination = 0.0;//光照强度
int body_status = 0;//人体感应，1代表有人、0代表没人
int key_number = 0;//获取按键返回值
int liv_LED_flag = 1; //卧室灯标志位 1代表关 0代表开
int bed_LED_flag = 1;//客厅灯标志位 1代表关 0代表开
int illumination_flag = 1; //窗帘标志位 1代表窗帘开 0代表关
int fanflag = 0; //风扇标志位 1代表开 0代表关
int time_count = 0; //控制数据上传时间
int main(void)
{
	unsigned char *dataPtr = NULL;

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  delay_ms(50);           // 延时函数--50mS
	uart_init(115200);		//初始化串口	
	
	GPIO_Config();          // 初始化 对应的引脚
	
	LCD_Init();             // 初始化 1.8寸LCD液晶屏 SPI 接口
	GBK_Lib_Init();         // 硬件GBK字库初始化--(如果使用不带字库的液晶屏版本，此处可以屏蔽，不做字库初始化）
	LCD_Clear(WHITE);       // 清屏

	KEY_Init();             //初始化与按键连接的硬件接口
  boma_Init(); //初始化拨码
	DHT11_GPIO_Config();
	Adc_Init();         //初始化ADC-- ( PA1-ADC1_CH1 )

	TIM4_PWM_Init(200-1,8400-1);//舵机
	TIM3_PWM_Init(200-1,8400-1);//风扇
	TIM_SetCompare1(TIM3,0);
	TIM5_Int_Init(20-1,840-1);
	TIM_Cmd(TIM5,DISABLE);//关定时器
	if(i2c_CheckDevice(BH1750_Addr)==0)
		printf("BH1750设备检测成功\r\n");
	else printf("BH1750设备检测失败\r\n");
	BH1750_Init();
	
	bed_LED_OFF;//默认灯关
	liv_LED_OFF;
	
	ESP8266_init();
  while (1)
  { 
			clear_control=0;//用于控制屏幕是否清屏
			while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==1)//拨码1打上，进入设置界面
			{
					delay_ms(1);//消抖
					if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==1)
					{ 
						alarm_set();//设置定时时间
					}
			}
			if(clear_control==1)//设置完时间退出来后
			{
					LCD_Clear(WHITE);       // 清屏
			}
			get_gas_real_value(&gas_value);//气体浓度获取
			Read_DHT11(&DHT11_Data);//温湿度获取
			illumination = LIght_Intensity();//光照强度获取
			body_status =	HC_SR501_READ(); //获取人体感应状态
			key_number = KEY_Scan(0);		//得到键值
		//	printf("temp:%d.%d , humi:%d.%d\r\n",DHT11_Data.temp_int,DHT11_Data.temp_deci,DHT11_Data.humi_int,DHT11_Data.humi_deci);
			//printf("BH1750:%0.2f\r\n",BH1750_GetData());
			//TIM_SetCompare1(TIM3,50);
			//printf("BH1750:%0.2f lx\r\n",LIght_Intensity());
			//printf("HC_SR501:%d\r\n",body_status);
		
			sprintf(oledBuf,"智能家居");
			Draw_Font16B(46,2,BLACK,oledBuf);
			sprintf(oledBuf,"温度:   %d.%d",DHT11_Data.temp_int,DHT11_Data.temp_deci);
			Draw_Font16B(0,20,BLACK,oledBuf);
			sprintf(oledBuf,"湿度:   %d.%d",DHT11_Data.humi_int,DHT11_Data.humi_deci);
			Draw_Font16B(0,37,BLACK,oledBuf);
			sprintf(oledBuf,"光照强度: %0.2f ",illumination);
			Draw_Font16B(0,54,BLACK,oledBuf);
			sprintf(oledBuf,"气体浓度: %0.2f ",gas_value);
			Draw_Font16B(0,71,BLACK,oledBuf);
			sprintf(oledBuf,"人体状态:");
			Draw_Font16B(0,88,BLACK,oledBuf);
			
			switch(body_status)
			{
				case 0:	
					      sprintf(oledBuf,"无人");
								Draw_Font16B(78,88,GREEN,oledBuf);break;
			
				case 1:	sprintf(oledBuf,"有人");
								Draw_Font16B(78,88,RED,oledBuf);break;
			}
			
			sprintf(oledBuf,"窗帘状态:");
			Draw_Font16B(0,105,BLACK,oledBuf);
			
			switch(illumination_flag)
			{
				case 1:	
					      sprintf(oledBuf,"开");
								Draw_Font16B(78,105,GREEN,oledBuf);break;
			
				case 0:	sprintf(oledBuf,"关");
								Draw_Font16B(78,105,RED,oledBuf);break;
			}
			
			/*自动控制*/
			
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)==1)//手动模式
		{
			//printf("manual mode\r\n");
		}
		else//自动模式
		{
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)//场景1
			{
				delay_ms(10);//消抖
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
				{
					if((int)illumination < light_limit)
					{
							if(body_status == 1 && liv_LED_flag == 1)//有人走过去
							{
								printf("liv_LED_ON\r\n");
								liv_LED_ON;
								liv_LED_flag = 0;//这样才能让程序到这里只执行一遍
							}
						}
					}
			}
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)//场景2
			{
				delay_ms(10);//消抖
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)
				{
					if((int)illumination < light_limit)
					{
							if(body_status == 1&& bed_LED_flag == 1 )//有人走过去
							{
									printf("bed_LED_ON\r\n");
									breathing_lamp();
									bed_LED_flag = 0;
							}
					}
				}
			}
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)== 0)//场景3
			{
				delay_ms(10);//消抖
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
				{
						if(illumination >= alarm[1] && illumination_flag == 1)//关窗帘
						{
								access_control(0);//关
								illumination_flag = 0;
												
						}
						else if(illumination < alarm[1] && illumination_flag == 0)//开窗帘
						{
							access_control(1);//开
							illumination_flag = 1;
						}

				}
			}
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)== 1)//场景4
			{
				delay_ms(10);//消抖
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==1 && GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)
				{

						if(illumination < alarm[1] && illumination_flag == 1)//关窗帘，开灯
						{
								access_control(0);//关
								illumination_flag = 0;
						}
						liv_LED_ON;

				}
			}
								/*温度控制风扇*/
			if(DHT11_Data.temp_int >= alarm[0])//开风扇
			{
					Fan_speed_set(1);//开
				  fanflag = 1; 
			}
			else if(DHT11_Data.temp_int < alarm[0])
			{
					Fan_speed_set(0);
					fanflag = 0; 
			}
			
			/*气体浓度控制排风扇和报警*/
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
			

			/*本地按键控制*/
			if(key_number == K1_PRES && illumination_flag == 0)//left 开窗帘
			{
				access_control(1);
				illumination_flag = 1;
			}
			if(key_number == K3_PRES && illumination_flag == 1)//right 关窗帘
			{
				access_control(0);
				illumination_flag = 0;
			}
			if(key_number == K0_PRES)//dowm 关老人房灯
			{
				bed_LED_OFF;
				bed_LED_flag = 1;
			}
			if(key_number == K2_PRES)//up 关大厅灯
			{
				liv_LED_OFF;
				liv_LED_flag = 1;
			}
			
			if(time_count > 3)//200ms发布一次
			{
					publish_tip();//上传数据到云端
					time_count=0;
			}
		
		
			//printf("key:%d\r\n",KEY_Scan(0));
		
			/*解析接收的mqtt信息*/
			dataPtr = ESP8266_GetIPD(3);
			if(dataPtr != NULL)
			 OneNet_RevPro(dataPtr);

			time_count++;
			delay_ms(50);
			
			
	}
}

void access_control(int mode) //0关,1开
{
	if(mode == 1)//开
	{
		TIM_SetCompare4(TIM4,22);
		delay_ms(350);
		TIM_SetCompare4(TIM4,15);// 设置比较寄存器的值
	}
	if(mode == 0)
	{
		TIM_SetCompare4(TIM4,7);
		delay_ms(350);
		TIM_SetCompare4(TIM4,15);// 设置比较寄存器的值
	}
}

void publish_tip()//上传数据函数
{
			float temp = DHT11_Data.temp_int+(DHT11_Data.temp_deci/10.0);//温度
			float humi = DHT11_Data.humi_int+(DHT11_Data.humi_deci/10.0);//湿度
			//客厅灯状态 ，这里取反是因为在APP 1 表示开 0表示关
			int liv_LED_status = !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
			int bed_LED_status = !bed_LED_flag;//卧室灯状态
	    int fan_status = fanflag;//风扇状态
			int window_status = illumination_flag;//窗帘状态
//			DEBUG_LOG("==================================================================================");
//			DEBUG_LOG("发布数据 ----- OneNet_Publish");
			sprintf(PUB_BUF,"{\"bed_LED\":%d,\"Temp\":%0.1f,\"humi\":%0.1f,\"liv_LED\":%d,\"gas\":%0.2f,\"light\":%0.2f,\"fan\":%d,\"window\":%d }",
	   	bed_LED_status,temp,humi,liv_LED_status,gas_value,illumination,fan_status,window_status);
			OneNet_Publish(devPubTopic, PUB_BUF);
//			DEBUG_LOG("==================================================================================");
			ESP8266_Clear();
}

void alarm_set(void)//设置报警上限
{
		int key;
		
	//限制的范围
		x%=3;
		alarm[0]%=100;
		alarm[1]%=1000;
		alarm[2]%=100;
	
		if(clear_control==0)
		{
			   LCD_Clear(WHITE);       // 清屏
		}
		clear_control=1;//设置完退出去需要清屏一次
		
		sprintf(oledBuf,"上限设置");
    Draw_Font16B(32,0,BLACK,oledBuf);
		sprintf(oledBuf,"温度:%d",alarm[0]);
		Draw_Font16B(0,30,BLACK,oledBuf);
		sprintf(oledBuf,"光照强度:%d",alarm[1]); 
		Draw_Font16B(0,60,BLACK,oledBuf);
		sprintf(oledBuf,"气体浓度:%d",alarm[2]);
		Draw_Font16B(0,90,BLACK,oledBuf);

		key=KEY_Scan(0);		//得到键值

	  if(key)
		{
			switch(key)
			{				 
				case K3_PRES:	//按键right按下
						++x;
					break;
				
				case K0_PRES:	//按键dowm按下
						--alarm[x];
					break;
				
				case K1_PRES:	//按键left按下
							--x;
					break;
				
				case K2_PRES:	//按键up按下
					++alarm[x];
					break;
				
			}
		}

} 

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //溢出中断
	{                                    
	
		time ++;

		/* 占空比设置 */
		pwmset = time/500;

		/* 5ms 脉宽 */
		if(timecount > 50) timecount = 0;
			else timecount ++;

		if(timecount >= pwmset ) 
			GPIO_SetBits(LED_PORT,LED_PIN);
		else 
			GPIO_ResetBits(LED_PORT,LED_PIN);
		
		if(time>16000)
		{
			printf("TIM5,DISABLE\r\n");
			TIM_Cmd(TIM5,DISABLE);//关定时器
			GPIO_ResetBits(LED_PORT,LED_PIN);
		}

	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //清除中断标志位
}
void ESP8266_init()
{
	USARTx_Config();//stm32-8266通讯串口
	if(!ESP8266_INIT_OK){
		sprintf(oledBuf,"Waiting For");
		Draw_Font16B(0,64,RED,oledBuf);
		sprintf(oledBuf,"WiFi");
		Draw_Font16B(0,80,RED,oledBuf);
		sprintf(oledBuf,"Connection");
		Draw_Font16B(0,96,RED,oledBuf);
	}
	ESP8266_Init();					//初始化ESP8266
	LCD_Clear(WHITE);       // 清屏
	
	sprintf(oledBuf,"Waiting For");
	Draw_Font16B(0,64,RED,oledBuf);
	sprintf(oledBuf,"MQTT Server");
	Draw_Font16B(0,80,RED,oledBuf);
	sprintf(oledBuf,"Connection");
	Draw_Font16B(0,96,RED,oledBuf);
	while(OneNet_DevLink()){//接入OneNET
	delay_ms(500);
	}	
	BEEP_ON;//鸣叫提示接入成功
	delay_ms(250);
	BEEP_OFF;
	OneNet_Subscribe(devSubTopic, 1);//订阅mqtt主题
	LCD_Clear(WHITE);       // 清屏
}

void breathing_lamp()//打开呼吸灯
{
		TIM_Cmd(TIM5,ENABLE);//开定时器
		pwmset = 0;
		time = 0;
		timecount = 0;
}

void Fan_speed_set(int speed_level)//风扇调速 0,1,2,3
{
	if(speed_level > 3)//限制最大速度
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
