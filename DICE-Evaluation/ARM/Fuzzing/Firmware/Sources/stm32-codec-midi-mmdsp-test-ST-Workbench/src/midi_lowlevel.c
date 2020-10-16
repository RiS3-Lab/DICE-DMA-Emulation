#include "midi_lowlevel.h"
#include "leds.h" 

char canary1[128];
char midiBuffer[MIDI_BUF_SIZE];
char canary2[128];

int MIDIlastIndex = 0;
int MIDITimeToProcessBuffer = 0;

static void USART2_Enable_Rx(void)
{
    GPIO_InitTypeDef  GPIO_USART_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    DMA_InitTypeDef   DMA_InitStruct;

    /* Enable Clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    /* Set up GPIO for alternate function */
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);

    /* Configure GPIO to transmit */
    GPIO_USART_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_USART_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_USART_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_USART_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_USART_InitStruct);

    /* Configure USART */
    USART_InitStruct.USART_BaudRate = MIDI_BAUD_RATE;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStruct);

    /* Enable UART */
    USART_Cmd(USART2, ENABLE);

    /* no DMA interrupts */
//    DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);
//    NVIC_EnableIRQ(DMA1_Stream0_IRQn);


    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_Channel = DMA_Channel_4;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR));
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)midiBuffer;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_BufferSize = MIDI_BUF_SIZE;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream5, &DMA_InitStruct);
    DMA_Cmd(DMA1_Stream5, ENABLE);

    /* Connect UART to DMA */
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

    /* wait for DMA to be enabled */
    while (DMA_GetCmdStatus(DMA1_Stream5) != ENABLE);

}

static void Timer_setup(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimerInitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimerInitStruct.TIM_Prescaler = 0;
    TIM_TimerInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimerInitStruct.TIM_Period = MIDI_TIMER_PERIOD_TICKS; /* Tick once per millisecond */
    TIM_TimerInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_TimerInitStruct);
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void MIDI_low_level_setup(void)
{
    USART2_Enable_Rx();
    Timer_setup();
}
    
void MIDI_process_buffer(void)
{
    if (!MIDITimeToProcessBuffer) {
        return;
    }
    MIDITimeToProcessBuffer = 0;
    int length = MIDI_DMA_STRUCT->NDTR;
    int topIndex = MIDI_BUF_SIZE - length;
    if (topIndex < MIDIlastIndex) {
        topIndex += MIDI_BUF_SIZE;
    }
    int numItems = topIndex - MIDIlastIndex;
    while (numItems--) {
        MIDI_process_byte(midiBuffer[MIDIlastIndex]);
        /* increase last index so the function starts at the correct point next interrupt */
        MIDIlastIndex = (MIDIlastIndex + 1) % MIDI_BUF_SIZE; 
    }
}

/* Not even called */
void DMA1_Stream5_IRQHandler (void)
{
    if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5) 
            && DMA_GetFlagStatus(DMA1_Stream5, DMA_FLAG_TCIF5)) {
        LEDs_greenToggle();
        DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
        DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
    }
    NVIC_ClearPendingIRQ(DMA1_Stream5_IRQn);
}

void TIM2_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(TIM2_IRQn);
    //if (TIM_GetITStatus(TIM2, TIM_IT_Update)) { P2IM is not solving this SRR site
        MIDI_TIMER_INTERRUPT();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
   // }
}

static void USART2_Enable_Rx_nolib(void)
{
    /* Enable clocks */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_DMA1EN;
    /* Setup GPIOD6
     * Alternate function = UASRT2 (AF7),
     * no pull
     * fast speed
     *  */
    GPIOD->AFR[0] &= ~(0xf << (4*6));
    GPIOD->AFR[0] |= (0x7 << (4*6));
    GPIOD->MODER &= ~GPIO_MODER_MODER6;
    GPIOD->MODER |= 0x2 << (2*6);
    GPIOD->PUPDR &= ~GPIO_PUPDR_PUPDR6;
    GPIOD->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;
    GPIOD->OSPEEDR |= 0x3 << (2*6);
    /* Setup USART2
     * baudrate = MIDI_BAUD_RATE
     * DIV = (CPU_FREQ / AHB_PRESCALAR / APB1_PRESCALAR)/(MIDI_BAUD_RATE)
     * setup to receive and enable
     * enable DMA mode for reception */
    USART2->BRR = 180000000 / 1 / 4 / MIDI_BAUD_RATE;
    USART2->CR1 = 0x2004;
    USART2->CR3 = 0x40;
    /* Setup DMA
     * Channel 4
     * peripheral = USART2->DR
     * memory = midiBuffer
     * dir = peripheral to memory
     * buffer size is MIDI_BUF_SIZE
     * no peripheral inc
     * enable memory inc
     * peripheral size is byte
     * memory size is byte
     * circular mode
     * high priority
     * no FIFO (triggers when 1/4 full)
     * single memory burst 
     * single peripheral burst
     */
    DMA1_Stream5->CR = (0x4 << 25) 
        | (0x2 << 16) 
        | (0x1 << 10)
        | (0x1 << 8);
    DMA1_Stream5->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream5->M0AR = (uint32_t)midiBuffer;
    DMA1_Stream5->NDTR = (uint16_t)MIDI_BUF_SIZE;
    DMA1_Stream5->FCR &= ~0x3;
    /* Enable DMA */
    DMA1_Stream5->CR |= 0x1;
}

void MIDI_low_level_setup_nolib(void)
{
    //USART2_Enable_Rx_nolib();
	USART2_Enable_Rx();
    Timer_setup();
}
