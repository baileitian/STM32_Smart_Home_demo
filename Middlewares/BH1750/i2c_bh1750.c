#include "i2c_bh1750.h"

//BH1750дһ���ֽ�
//����ֵ	�ɹ���0		ʧ�ܣ���0 
uint8_t BH1750_Byte_Write(uint8_t data)
{
	i2c_Start();
	//����д��ַ
	i2c_SendByte(BH1750_Addr|0);
	if(i2c_WaitAck()==1)
		return 1;
	//���Ϳ�������
	i2c_SendByte(data);
	if(i2c_WaitAck()==1)
		return 2;
	i2c_Stop();
	return 0;
}

//BH1750��ȡ��������
//����ֵ �ɹ������ع���ǿ�� 	ʧ�ܣ�����0
uint16_t BH1750_Read_Measure(void)
{
	uint16_t receive_data=0; 
	i2c_Start();
	//���Ͷ���ַ
	i2c_SendByte(BH1750_Addr|1);
	if(i2c_WaitAck()==1)
		return 0;
	//��ȡ�߰�λ
	receive_data=i2c_ReadByte();
	i2c_Ack();
	//��ȡ�Ͱ�λ
	receive_data=(receive_data<<8)+i2c_ReadByte();
	i2c_NAck();
	i2c_Stop();
	return receive_data;	//���ض�ȡ��������
}


//BH1750s�ϵ�
void BH1750_Power_ON(void)
{
	BH1750_Byte_Write(POWER_ON);
}

//BH1750s�ϵ�
void BH1750_Power_OFF(void)
{
	BH1750_Byte_Write(POWER_OFF);
}

//BH1750��λ	�����ϵ�ʱ��Ч
void BH1750_RESET(void)
{
	BH1750_Byte_Write(MODULE_RESET);
}

//BH1750��ʼ��
void BH1750_Init(void)
{
	BH1750_Power_ON();	//BH1750s�ϵ�
	//BH1750_RESET();			//BH1750��λ
	BH1750_Byte_Write(Measure_Mode);
	delay_ms(120);
}

//��ȡ����ǿ��
float LIght_Intensity(void)
{
	return (float)(BH1750_Read_Measure()/1.2f*Resolurtion);
}
