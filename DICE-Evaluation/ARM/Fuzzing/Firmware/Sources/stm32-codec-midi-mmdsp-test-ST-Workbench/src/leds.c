/* Some functions for giving some feedback through the LEDs */
#include "stm32f4xx.h" 

GPIO_InitTypeDef LEDs_GPIO_InitStruct;

void LEDs_Init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    LEDs_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    LEDs_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    LEDs_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    LEDs_GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    LEDs_GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOG, &LEDs_GPIO_InitStruct);
}

void LEDs_Toggle(void)
{
    GPIO_ToggleBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14);
}

void LEDs_Set(void)
{
    GPIO_SetBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14);
}

void LEDs_Reset(void)
{
    GPIO_ResetBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14);
}

void LEDs_redToggle(void)
{
    GPIO_ToggleBits(GPIOG, GPIO_Pin_14);
}

void LEDs_redSet(void)
{
    GPIO_SetBits(GPIOG, GPIO_Pin_14);
}

void LEDs_redReset(void)
{
    GPIO_ResetBits(GPIOG, GPIO_Pin_14);
}
    
void LEDs_greenToggle(void)
{
    GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
}

void LEDs_greenSet(void)
{
    GPIO_SetBits(GPIOG, GPIO_Pin_13);
}

void LEDs_greenReset(void)
{
    GPIO_ResetBits(GPIOG, GPIO_Pin_13);
}
    
