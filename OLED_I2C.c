#include "stm32f10x.h"
#include "OLED_I2C.h"
#include "Delay.h"
#include "codetab.h"
#include "stdint.h"

// 固定引脚：SCL=PB6  SDA=PB7（必须严格按这个接线）
#define OLED_SCL_PIN GPIO_Pin_6
#define OLED_SDA_PIN GPIO_Pin_7
#define OLED_GPIO_PORT GPIOB
#define OLED_GPIO_CLK RCC_APB2Periph_GPIOB

#define OLED_SCL_H GPIO_SetBits(OLED_GPIO_PORT, OLED_SCL_PIN)
#define OLED_SCL_L GPIO_ResetBits(OLED_GPIO_PORT, OLED_SCL_PIN)
#define OLED_SDA_H GPIO_SetBits(OLED_GPIO_PORT, OLED_SDA_PIN)
#define OLED_SDA_L GPIO_ResetBits(OLED_GPIO_PORT, OLED_SDA_PIN)

// I2C GPIO初始化
static void IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(OLED_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN | OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStructure);

    OLED_SCL_H;
    OLED_SDA_H;
}

// I2C起始信号
static void IIC_Start(void)
{
    OLED_SDA_H;
    OLED_SCL_H;
    Delay_us(5);
    OLED_SDA_L;
    Delay_us(5);
    OLED_SCL_L;
}

// I2C停止信号
static void IIC_Stop(void)
{
    OLED_SDA_L;
    OLED_SCL_H;
    Delay_us(5);
    OLED_SDA_H;
    Delay_us(5);
}

// I2C等待应答（修复闪烁的核心！）
static uint8_t IIC_Wait_Ack(void)
{
    uint8_t ErrTime = 0;
    OLED_SDA_H; Delay_us(5);
    OLED_SCL_H; Delay_us(5);
    while(GPIO_ReadInputDataBit(OLED_GPIO_PORT, OLED_SDA_PIN))
    {
        ErrTime++;
        if(ErrTime > 250) { IIC_Stop(); return 1; }
    }
    OLED_SCL_L;
    return 0;
}

// I2C发送一个字节
static void IIC_Send_Byte(uint8_t byte)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        OLED_SCL_L; Delay_us(5);
        if(byte & 0x80) OLED_SDA_H; else OLED_SDA_L;
        byte <<= 1;
        OLED_SCL_H; Delay_us(5);
    }
    OLED_SCL_L;
    IIC_Wait_Ack(); // 必须加应答！否则通信乱码/闪烁
}

// 写命令
void OLED_WriteCmd(unsigned char I2C_Command)
{
    IIC_Start();
    IIC_Send_Byte(OLED_ADDRESS);
    IIC_Send_Byte(0x00);
    IIC_Send_Byte(I2C_Command);
    IIC_Stop();
}

// 写数据
void OLED_WriteDat(unsigned char I2C_Data)
{
    IIC_Start();
    IIC_Send_Byte(OLED_ADDRESS);
    IIC_Send_Byte(0x40);
    IIC_Send_Byte(I2C_Data);
    IIC_Stop();
}

// 优化初始化（彻底稳定，无闪烁）
void OLED_Init(void)
{
    IIC_GPIO_Init();
    Delay_ms(100);

    OLED_WriteCmd(0xAE);	// 关显示
    OLED_WriteCmd(0xD5);	// 时钟分频
    OLED_WriteCmd(0x80);
    OLED_WriteCmd(0xA8);	// 驱动路数
    OLED_WriteCmd(0x3F);
    OLED_WriteCmd(0xD3);	// 显示偏移
    OLED_WriteCmd(0x00);
    OLED_WriteCmd(0x40);	// 起始行
    OLED_WriteCmd(0x8D);	// 电荷泵（必须开！）
    OLED_WriteCmd(0x14);
    OLED_WriteCmd(0x20);	// 地址模式
    OLED_WriteCmd(0x02);
    OLED_WriteCmd(0xA1);	// 左右镜像
    OLED_WriteCmd(0xC8);	// 上下镜像
    OLED_WriteCmd(0xDA);	// 引脚配置
    OLED_WriteCmd(0x12);
    OLED_WriteCmd(0x81);	// 对比度
    OLED_WriteCmd(0xCF);
    OLED_WriteCmd(0xD9);	// 预充电
    OLED_WriteCmd(0xF1);
    OLED_WriteCmd(0xDB);	// VCOMH
    OLED_WriteCmd(0x30);
    OLED_WriteCmd(0xA4);	// 全局显示
    OLED_WriteCmd(0xA6);	// 正常显示
    OLED_WriteCmd(0xAF);	// 开显示

    Delay_ms(20);
    OLED_CLS();
}

// 设置光标
void OLED_SetPos(unsigned char x, unsigned char y)
{
    OLED_WriteCmd(0xb0+y);
    OLED_WriteCmd(((x&0xF0)>>4)|0x10);
    OLED_WriteCmd(x&0x0F);
}

// 全屏填充
void OLED_Fill(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        OLED_WriteCmd(0xb0+m);
        OLED_WriteCmd(0x00);
        OLED_WriteCmd(0x10);
        for(n=0;n<128;n++) OLED_WriteDat(fill_Data);
    }
}

// 清屏
void OLED_CLS(void)
{
    OLED_Fill(0x00);
}

// 开启/关闭OLED
void OLED_ON(void)  { OLED_WriteCmd(0XAF); }
void OLED_OFF(void) { OLED_WriteCmd(0XAE); }

// 显示字符串
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0,i = 0,j = 0;
    switch(TextSize)
    {
        case 1:
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x>126){x=0;y++;}
                OLED_SetPos(x,y);
                for(i=0;i<6;i++) OLED_WriteDat(F6x8[c][i]);
                x += 6; j++;
            }
            break;
        case 2:
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x>120){x=0;y++;}
                OLED_SetPos(x,y);
                for(i=0;i<8;i++) OLED_WriteDat(F8X16[c*16+i]);
                OLED_SetPos(x,y+1);
                for(i=0;i<8;i++) OLED_WriteDat(F8X16[c*16+i+8]);
                x += 8; j++;
            }
            break;
    }
}

// 显示16*16中文（调用你的字库F16x16）
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm=0;
    unsigned int adder=32*N;
    OLED_SetPos(x,y);
    for(wm=0;wm<16;wm++) OLED_WriteDat(F16x16[adder++]);
    OLED_SetPos(x,y+1);
    for(wm=0;wm<16;wm++) OLED_WriteDat(F16x16[adder++]);
}

// 显示图片
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0; unsigned char x,y;
    if(y1%8==0) y=y1/8; else y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        OLED_SetPos(x0,y);
        for(x=x0;x<x1;x++) OLED_WriteDat(BMP[j++]);
    }
}
