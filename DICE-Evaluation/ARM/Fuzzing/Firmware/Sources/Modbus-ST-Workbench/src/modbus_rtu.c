#include "modbus_rtu.h"
#include "modbus_rtu_conf.h"
#include "stm32f3xx.h"
#include "string.h"
#include "stm32f3xx_ll_rcc.h"
#include <stdint.h>

uint16_t canary1[64];
uint16_t modbusMemory[MODBUS_SLAVE_REGISTERS_NUM];
uint16_t canary2[64];

#define MODBUS_INTERRUPT_PRIORITY configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2

#define MODBUS_MIN_FRAME_SIZE   5
#define MODBUS_MAX_FRAME_SIZE   256

static uint8_t canary3[128];
static uint8_t modbusRxTxBuffer[MODBUS_MAX_FRAME_SIZE];
static uint8_t canary4[128];

volatile uint16_t modbusRxCount = 0;
volatile uint8_t modbusSlaveFrameReadyFlag = 0;
static xSemaphoreHandle xFrameReadySemaphore;
static xTaskHandle xModbusSlaveTask;

static void modbusSlaveHandler(void * pvParameters);
static uint8_t modbusSlaveHardwareInit(void);
static void modbusSlaveStartTransmitting(uint16_t bytesNum);
static void modbusSlaveStartReceiving(void);

static uint8_t modbusSlaveCheckFrameSize(void);
static uint8_t modbusSlaveCheckID(void);
static uint8_t modbusSlaveCheckCRC(void);
static uint16_t modbusSlaveGetCRC(uint8_t * buffer, uint16_t len);
static uint16_t modbusSlaveErrorSet(uint8_t error);
static uint16_t modbusSlaveParseFrame(void);

static uint16_t aux; //P2IM

void modbusSlaveStartTread(void)
{
    xTaskCreate(modbusSlaveHandler, "MBHandler", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xModbusSlaveTask);
}

xTaskHandle modbusSlaveGetTaskHandke(void)
{
    return xModbusSlaveTask;
}

void * modbusSlaveGetGata(uint16_t address)
{
    return (void*)&modbusMemory[address];
}

void modbusSlavePutData(uint16_t address, void * value, uint16_t len)
{
    memcpy(&modbusMemory[address], value, len);
}


static void modbusSlaveHandler(void * pvParameters)
{
    modbusSlaveHardwareInit();
    while(1)
    {
        //xSemaphoreTake(xFrameReadySemaphore, portMAX_DELAY);
    	xSemaphoreTake(xFrameReadySemaphore, 1);
        if(!modbusSlaveCheckFrameSize())
        {
            modbusSlaveStartReceiving();
            continue;
        }
        if(!modbusSlaveCheckID())
        {
            modbusSlaveStartReceiving();
            continue;
        }
        if(!modbusSlaveCheckCRC())
        {
            modbusSlaveStartReceiving();
            continue;
        }
        uint16_t answerLen = modbusSlaveParseFrame();
        modbusSlaveStartTransmitting(answerLen);
    }
}

static uint8_t modbusSlaveHardwareInit(void)
{
    uint8_t status = 0;
    xFrameReadySemaphore = xSemaphoreCreateBinary();
    if(xFrameReadySemaphore == NULL)
        return status;
    /*Port: GPIOA
          Tx:   GPIO_Pin_9
          Rx:   GPIO_Pin_10
        */
    /*configure in alternate function Pull UP*/
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    GPIOA->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER9);
    GPIOA->MODER |= (GPIO_MODER_MODER10_1 | GPIO_MODER_MODER9_1);
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_10 | GPIO_OTYPER_OT_9);
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR9);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR10 | GPIO_PUPDR_PUPDR9);
    GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR9_0);
    GPIOA->AFR[1] &= ~(GPIO_AFRH_AFRH1 | GPIO_AFRH_AFRH2);
    GPIOA->AFR[1] |= ((0x07 << 8) | (0x07 << 4));

    //RCC_ClocksTypeDef RCC_ClocksStatus;
    //RCC_GetClocksFreq(&RCC_ClocksStatus);
    //uint32_t clock = RCC_ClocksStatus.USART1CLK_Frequency;
    uint32_t clock = LL_RCC_GetUSARTClockFreq(LL_RCC_USART1_CLKSOURCE);

    uint32_t divider = (uint32_t)((clock) / (MODBUS_SLAVE_USART_BAUDRATE));
    uint32_t tmpreg  = (uint32_t)((clock) % (MODBUS_SLAVE_USART_BAUDRATE));
    if(tmpreg > MODBUS_SLAVE_USART_BAUDRATE / 2)
        divider++;

    MODBUS_SLAVE_USART->CR1 &= ~0xFFFFFFFF;
    MODBUS_SLAVE_USART->CR2 &= ~0xFFFFFFFF;
    MODBUS_SLAVE_USART->CR3 &= ~0xFFFFFFFF;
    MODBUS_SLAVE_USART->BRR = divider;
    MODBUS_SLAVE_USART->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_RTOIE);
    MODBUS_SLAVE_USART->CR2 |= (USART_CR2_RTOEN);
    MODBUS_SLAVE_USART->CR3 |= (USART_CR3_DMAR | USART_CR3_DMAT);
    MODBUS_SLAVE_USART->RTOR = MODBUS_SLAVE_TIMEOUT;

    NVIC_SetPriority(USART1_IRQn, MODBUS_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(USART1_IRQn);

    /*Recieve channel*/
    DMA1_Channel5->CCR &= ~0xFFFFFFFF;
    DMA1_Channel5->CCR |= (DMA_CCR_PL | DMA_CCR_MINC | DMA_CCR_TCIE);
    DMA1_Channel5->CNDTR = MODBUS_MAX_FRAME_SIZE;
    DMA1_Channel5->CPAR = (uint32_t)&MODBUS_SLAVE_USART->RDR;
    DMA1_Channel5->CMAR = (uint32_t)modbusRxTxBuffer;

    NVIC_SetPriority(DMA1_Channel5_IRQn, MODBUS_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);

    /*Transmit channel*/
    DMA1_Channel4->CCR &= ~0xFFFFFFFF;
    DMA1_Channel4->CCR |= (DMA_CCR_PL | DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE);
    DMA1_Channel4->CPAR = (uint32_t)&MODBUS_SLAVE_USART->TDR;
    DMA1_Channel4->CMAR = (uint32_t)modbusRxTxBuffer;

    NVIC_SetPriority(DMA1_Channel4_IRQn, MODBUS_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    MODBUS_SLAVE_USART->CR1 |= USART_CR1_UE;
    DMA1_Channel5->CCR |= DMA_CCR_EN;

    status = 1;
    return status;
}

void DMA1_Channel4_IRQHandler(void)
{
    DMA1->IFCR |= DMA_IFCR_CTCIF4;
    DMA1_Channel4->CCR &= ~DMA_CCR_EN;
    modbusSlaveStartReceiving();
}

void DMA1_Channel5_IRQHandler(void)
{
    DMA1->IFCR |= DMA_IFCR_CTCIF5;
    DMA1_Channel5->CCR &= ~DMA_CCR_EN;
    MODBUS_SLAVE_USART->CR1 &= ~USART_CR1_RTOIE;
    modbusRxCount = MODBUS_MAX_FRAME_SIZE;
    xSemaphoreGiveFromISR(xFrameReadySemaphore, NULL);
}

void USART1_IRQHandler(void)
{
	aux=0;
    if(MODBUS_SLAVE_USART->ISR & USART_ISR_RTOF)
    {
        MODBUS_SLAVE_USART->ICR |= USART_ICR_RTOCF;
        MODBUS_SLAVE_USART->CR1 &= ~USART_CR1_RTOIE;
        modbusRxCount = MODBUS_MAX_FRAME_SIZE - DMA1_Channel5->CNDTR;
        DMA1_Channel5->CCR &= ~DMA_CCR_EN;
        aux=aux+MODBUS_SLAVE_USART->RDR;
        xSemaphoreGiveFromISR(xFrameReadySemaphore, NULL);
    }
}
static void modbusSlaveStartReceiving(void)
{
     DMA1_Channel5->CNDTR = MODBUS_MAX_FRAME_SIZE;
     DMA1_Channel5->CCR |= DMA_CCR_EN;
     MODBUS_SLAVE_USART->CR1 |= USART_CR1_RTOIE;
     modbusRxCount = 0;
}

static void modbusSlaveStartTransmitting(uint16_t bytesNum)
{
    DMA1_Channel4->CNDTR = bytesNum;
    DMA1_Channel4->CCR |= DMA_CCR_EN;
}

static uint8_t modbusSlaveCheckFrameSize(void)
{
    if(modbusRxCount > MODBUS_MIN_FRAME_SIZE)
        return 1;
    return 0;
}

static uint8_t modbusSlaveCheckID(void)
{
    if(modbusRxTxBuffer[0] == MODBUS_SLAVE_ADDRESS)
        return 1;
    return 0;
}

static uint8_t modbusSlaveCheckCRC(void)
{
    uint16_t recievedCRC = modbusRxTxBuffer[modbusRxCount - 1] << 8 | modbusRxTxBuffer[modbusRxCount - 2];
    uint16_t calculatedCRC = modbusSlaveGetCRC(modbusRxTxBuffer, modbusRxCount - 2);
    if(recievedCRC == calculatedCRC)
        return 1;
    return 1; //P2IM
}

static uint16_t modbusSlaveGetCRC(uint8_t * buffer, uint16_t len)
{
	uint16_t crc = 0xFFFF;
	for (uint16_t pos = 0; pos < len; pos++)
	{
		crc ^= (uint16_t)buffer[pos];

		for (int i = 8; i != 0; i--)
		{
			if ((crc & 0x0001) != 0)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
				crc >>= 1;
		}
	}
	return crc;
}

static uint16_t modbusSlaveErrorSet(uint8_t error)
{
    modbusRxTxBuffer[1] |= 0x80;
    modbusRxTxBuffer[2] = error;
    return MODBUS_MIN_FRAME_SIZE;
}

static uint16_t modbusSlaveParseFrame(void)
{
    uint8_t command = modbusRxTxBuffer[1];
    uint16_t answerLen = 0;
    if(command == 0x03)
    {
        uint16_t startAddr = modbusRxTxBuffer[2] << 8 | modbusRxTxBuffer[3];
        uint16_t regNum = modbusRxTxBuffer[4] << 8 | modbusRxTxBuffer[5];
        if(((startAddr + regNum) > MODBUS_SLAVE_REGISTERS_NUM) || (regNum > 126))
            answerLen = modbusSlaveErrorSet(0x02);
        else
        {
            for(uint16_t reg = 0; reg < regNum; reg++)
            {
                modbusRxTxBuffer[3 + (reg << 1)] = (uint8_t)(modbusMemory[startAddr + reg] >> 8);
                modbusRxTxBuffer[4 + (reg << 1)] = (uint8_t)(modbusMemory[startAddr + reg]);
            }
            modbusRxTxBuffer[2] = regNum << 1;
            answerLen =  MODBUS_MIN_FRAME_SIZE + modbusRxTxBuffer[2];
        }
    }
    else if (command == 0x06)
    {
        uint16_t startAddr = modbusRxTxBuffer[2] << 8 | modbusRxTxBuffer[3];
        if(startAddr > MODBUS_SLAVE_REGISTERS_NUM)
            answerLen = modbusSlaveErrorSet(0x02);
        else
        {
            modbusMemory[startAddr] = modbusRxTxBuffer[4] << 8 | modbusRxTxBuffer[5];
            answerLen = modbusRxCount;
        }
    }
    else if(command == 0x10)
    {
        uint16_t startAddr = modbusRxTxBuffer[2] << 8 | modbusRxTxBuffer[3];
        uint16_t regNum = modbusRxTxBuffer[4] << 8 | modbusRxTxBuffer[5];
        if(((startAddr + regNum) > MODBUS_SLAVE_REGISTERS_NUM) || (regNum > 126))
            answerLen = modbusSlaveErrorSet(0x02);
        else
        {
            for(uint16_t reg = 0; reg < regNum; reg++)
            {
                modbusMemory[startAddr + reg] = modbusRxTxBuffer[7 + (reg << 1)] << 8 | modbusRxTxBuffer[8 + (reg << 1)];
            }
            answerLen = 8;
        }
    }
    else
    {
        answerLen = modbusSlaveErrorSet(0x01);
    }
    uint16_t crc = modbusSlaveGetCRC(modbusRxTxBuffer, answerLen - 2);
    modbusRxTxBuffer[answerLen - 2] = crc;
    modbusRxTxBuffer[answerLen - 1] = crc >> 8;
    return answerLen;
}
