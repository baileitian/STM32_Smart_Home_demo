#include "i2c_bh1750.h"

//BH1750写一个字节
//返回值	成功：0		失败：非0 
uint8_t BH1750_Byte_Write(uint8_t data)
{
	i2c_Start();
	//发送写地址
	i2c_SendByte(BH1750_Addr|0);
	if(i2c_WaitAck()==1)
		return 1;
	//发送控制命令
	i2c_SendByte(data);
	if(i2c_WaitAck()==1)
		return 2;
	i2c_Stop();
	return 0;
}

//BH1750读取测量数据
//返回值 成功：返回光照强度 	失败：返回0
uint16_t BH1750_Read_Measure(void)
{
	uint16_t receive_data=0; 
	i2c_Start();
	//发送读地址
	i2c_SendByte(BH1750_Addr|1);
	if(i2c_WaitAck()==1)
		return 0;
	//读取高八位
	receive_data=i2c_ReadByte();
	i2c_Ack();
	//读取低八位
	receive_data=(receive_data<<8)+i2c_ReadByte();
	i2c_NAck();
	i2c_Stop();
	return receive_data;	//返回读取到的数据
}


//BH1750s上电
void BH1750_Power_ON(void)
{
	BH1750_Byte_Write(POWER_ON);
}

//BH1750s断电
void BH1750_Power_OFF(void)
{
	BH1750_Byte_Write(POWER_OFF);
}

//BH1750复位	仅在上电时有效
void BH1750_RESET(void)
{
	BH1750_Byte_Write(MODULE_RESET);
}

//BH1750初始化
void BH1750_Init(void)
{
	BH1750_Power_ON();	//BH1750s上电
	//BH1750_RESET();			//BH1750复位
	BH1750_Byte_Write(Measure_Mode);
	delay_ms(120);
}

//获取光照强度
float LIght_Intensity(void)
{
	return (float)(BH1750_Read_Measure()/1.2f*Resolurtion);
}
