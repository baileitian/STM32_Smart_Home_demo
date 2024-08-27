

//��Ƭ��ͷ�ļ�
#include "stm32f4xx.h"                  // Device header


//�����豸����
#include "esp8266.h"

//Ӳ������
#include "delay.h"
#include "usart.h"

//C��
#include <string.h>
#include <stdio.h>


#define WIFI_SSID 						"xiaomi"					//	WIFI������ ������2.4G��wifi������5G�ģ��Ҳ��������ġ��ո�
#define WIFI_PSWD 						"123456789"				//	WIFI����

#define SERVER_HOST						"120.79.100.106"			//	MQTT������������IP
#define SERVER_PORT						"1883"									//	MQTT�������˿ڣ�һ��Ϊ1883���øģ�


#define ESP8266_WIFI_INFO			"AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PSWD "\"\r\n"
#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"" SERVER_HOST "\"," SERVER_PORT "\r\n"

unsigned char esp8266_buf[128];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;

extern u8 ESP8266_INIT_OK;

//==========================================================
//	�������ƣ�	ESP8266_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

//==========================================================
//	�������ƣ�	ESP8266_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		esp8266_cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	esp8266_cntPre = esp8266_cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;
	Usart2_SendString((unsigned char *)cmd, strlen((const char *)cmd));
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//����յ�����
		{
			
			if(strstr((const char *)esp8266_buf, res) != NULL)		//����������ؼ���
			{
				ESP8266_Clear();									//��ջ���
				
				return 0;
			}
		}
		
		delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	�������ƣ�	ESP8266_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
	{
		Usart2_SendString(data, len);		//�����豸������������
	}

}

//==========================================================
//	�������ƣ�	ESP8266_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//����������
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//������IPD��ͷ
			if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				DEBUG_LOG("\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					DEBUG_LOG("\"IPD\" found\r\n");
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		delay_ms(5);
		timeOut--;	//��ʱ�ȴ�
	} while(timeOut > 0);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

}

//==========================================================
//	�������ƣ�	ESP8266_Init
//
//	�������ܣ�	��ʼ��ESP8266
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Init(void)
{
	
	GPIO_InitTypeDef GPIO_Initure;
	
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//ESP8266��λ����
	GPIO_Initure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Initure.GPIO_Pin = GPIO_Pin_5;					//GPIOC14-��λ
	GPIO_Initure.GPIO_OType = GPIO_OType_PP;
	
	GPIO_Initure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Initure);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
	delay_ms(250);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
	delay_ms(500);
	
	ESP8266_Clear();
	
	DEBUG_LOG("0. AT - ����MCU-8266ͨѶ");
	while(ESP8266_SendCmd("AT\r\n","OK"))
		delay_ms(500);
	
	DEBUG_LOG("1. AT+RST - ��λ8266");
	ESP8266_SendCmd("AT+RST\r\n", "");
		delay_ms(500);
	ESP8266_SendCmd("AT+CIPCLOSE\r\n", "");
		delay_ms(500);
	DEBUG_LOG("2. AT+CWMODE=1,1 - ����8266����ģʽΪSTA");
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
		delay_ms(500);
	
	DEBUG_LOG("3. AT+CWDHCP=1,1 - ʹ��STAģʽ��DHCP");
	while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
		delay_ms(500);
	
	DEBUG_LOG("4. AT+CWJAP - ����WIFI");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
		delay_ms(500);
	
	DEBUG_LOG("5. AT+CIPSTART - ���ӷ�����");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);
	ESP8266_INIT_OK = 1;
	DEBUG_LOG("6. ESP8266 Init OK - ESP8266��ʼ���ɹ�");
	DEBUG_LOG("ESP8266��ʼ��			[OK]");
}

//==========================================================
//	�������ƣ�	USART2_IRQHandler
//
//	�������ܣ�	����2�շ��ж�
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void USART2_IRQHandler(void)
{
//DEBUG_LOG("asd\n");
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //�����ж�
	{
		if(esp8266_cnt >= sizeof(esp8266_buf))	
		{
			esp8266_cnt = 0; //��ֹ���ڱ�ˢ��
		}
		esp8266_buf[esp8266_cnt++] = USART_ReceiveData(USART2);
		//DEBUG_LOG("%c\n",esp8266_buf[esp8266_cnt++]); 
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}

}



