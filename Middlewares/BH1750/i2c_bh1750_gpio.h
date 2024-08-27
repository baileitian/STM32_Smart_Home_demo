#ifndef __I2C_GPIO_H
#define __I2C_GPIO_H

#include "stm32f4xx.h"

#define BH1750_I2C_WR	0		/* д����bit */
#define BH1750_I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define BH1750_GPIO_PORT_I2C	GPIOB			/* GPIO�˿� */
#define BH1750_RCC_I2C_PORT 	RCC_AHB1Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define BH1750_I2C_SCL_PIN		GPIO_Pin_1			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define BH1750_I2C_SDA_PIN		GPIO_Pin_0			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 1	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define BH1750_I2C_SCL_1()  GPIO_SetBits(BH1750_GPIO_PORT_I2C, BH1750_I2C_SCL_PIN)		/* SCL = 1 */
	#define BH1750_I2C_SCL_0()  GPIO_ResetBits(BH1750_GPIO_PORT_I2C, BH1750_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define BH1750_I2C_SDA_1()  GPIO_SetBits(BH1750_GPIO_PORT_I2C, BH1750_I2C_SDA_PIN)		/* SDA = 1 */
	#define BH1750_I2C_SDA_0()  GPIO_ResetBits(BH1750_GPIO_PORT_I2C, BH1750_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define BH1750_I2C_SDA_READ()  GPIO_ReadInputDataBit(BH1750_GPIO_PORT_I2C, BH1750_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define BH1750_I2C_SCL_1()  BH1750_GPIO_PORT_I2C->BSRR = BH1750_I2C_SCL_PIN				/* SCL = 1 */
	#define BH1750_I2C_SCL_0()  BH1750_GPIO_PORT_I2C->BRR = BH1750_I2C_SCL_PIN				/* SCL = 0 */
	
	#define BH1750_I2C_SDA_1()  BH1750_GPIO_PORT_I2C->BSRR = BH1750_I2C_SDA_PIN				/* SDA = 1 */
	#define BH1750_I2C_SDA_0()  BH1750_GPIO_PORT_I2C->BRR = BH1750_I2C_SDA_PIN				/* SDA = 0 */
	
	#define BH1750_I2C_SDA_READ()  ((BH1750_GPIO_PORT_I2C->IDR & BH1750_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif


void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);


#endif

