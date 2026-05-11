#include "dht11.h"
#include "Delay.h"

/************************************************************************
* 函数: void DHT11_IO_IN(void)
* 描述: 设置DQ引脚为输入模式
************************************************************************/
void DHT11_IO_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/************************************************************************
* 函数: void DHT11_IO_OUT(void)
* 描述: 设置DQ引脚为输出模式
************************************************************************/
void DHT11_IO_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/************************************************************************
* 函数: void DHT11_Rst(void)
* 描述: 复位DHT11
************************************************************************/
void DHT11_Rst(void)
{
	DHT11_IO_OUT();
    GPIO_ResetBits(GPIOB, GPIO_Pin_15);
    Delay_ms(20);
    GPIO_SetBits(GPIOB, GPIO_Pin_15);
	Delay_us(30);
}

/************************************************************************
* 函数: uint8_t DHT11_Check(void)
* 描述: 等待DHT11的回应
************************************************************************/
uint8_t DHT11_Check(void)
{
	uint8_t retry=0;
	DHT11_IO_IN();

    while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) && retry<100)
	{
		retry++;
		Delay_us(1);
	};

	if(retry>=100)return 1;
	else retry=0;

    while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) && retry<100)
	{
		retry++;
		Delay_us(1);
	};

	if(retry>=100)return 1;
	return 0;
}

/************************************************************************
* 函数: uint8_t DHT11_Read_Bit(void)
* 描述: 从DHT11读取一个位的数据
************************************************************************/
uint8_t DHT11_Read_Bit(void)
{
 	uint8_t retry=0;

	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) && retry<100)
	{
		retry++;
		Delay_us(1);
	}

	retry=0;
	while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) && retry<100)
	{
		retry++;
		Delay_us(1);
	}

	Delay_us(40);
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15))return 1;
	else return 0;
}

/************************************************************************
* 函数: uint8_t DHT11_Read_Byte(void)
* 描述: 从DHT11读取一个字节
************************************************************************/
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i,dat;
    dat=0;

	for (i=0;i<8;i++)
	{
   		dat<<=1;
	    dat|=DHT11_Read_Bit();
    }

    return dat;
}

/************************************************************************
* 函数: uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)
* 描述: 从DHT11读取一次数据
************************************************************************/
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)
{
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();

	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;

	return 0;
}

/************************************************************************
* 函数: uint8_t DHT11_Init(void)
* 描述: 初始化DHT11
************************************************************************/
uint8_t DHT11_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_15);

	DHT11_Rst();
	return DHT11_Check();
}
