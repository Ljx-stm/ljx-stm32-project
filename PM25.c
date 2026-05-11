#include "PM25.h"
#include "adc.h"
#include "stm32f10x.h"
#include "Delay.h"

// 取消LED引脚、取消时序，纯读取ADC
uint16_t PM25_GetDATA(void)
{
    // 直接读PB1 ADC9
    uint16_t adc_val = Get_Adc_Average(ADC_Channel_9, 5);
    
    // 3.3V换算
    float voltage = (float)adc_val * 3.3f / 4096.0f;
    
    // 弱化基线，强制有数值，不会卡死0
    float concentration = voltage * 500.0f;

    if(concentration < 0)   concentration = 0;
    if(concentration > 500) concentration = 500;

    return (uint16_t)concentration;
}
