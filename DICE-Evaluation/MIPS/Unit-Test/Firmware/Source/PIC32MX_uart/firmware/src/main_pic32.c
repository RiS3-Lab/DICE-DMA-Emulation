/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include <stdint.h>
#include "definitions.h"                // SYS function prototypes              // SYS function prototypes


/* Timer Counter Time period match values for input clock of 48 MHz */
#define PERIOD_500MS                            24000000U
#define PERIOD_1S                               48000000U
#define PERIOD_2S                               96000000U
#define PERIOD_4S                               192000000U

#define SWITCH_PRESSED_STATE                    0   // Active LOW switch

/*
typedef enum
{
    TEMP_SAMPLING_RATE_500MS = 0,
    TEMP_SAMPLING_RATE_1S = 1,
    TEMP_SAMPLING_RATE_2S
            = 2,
    TEMP_SAMPLING_RATE_4S = 3,
} TEMP_SAMPLING_RATE;
static TEMP_SAMPLING_RATE tempSampleRate = TEMP_SAMPLING_RATE_500MS;
*/
static volatile int counter=0;
static volatile bool isTmr2Expired = false;
static volatile bool changeTempSamplingRate = false;
static volatile bool isUARTTxComplete = false;
static uint8_t __attribute__ ((aligned (16))) uartTxBuffer[100] = {0};
static uint8_t __attribute__ ((aligned (16))) uartRxBuffer[100] = {0};
static volatile bool isKeyreceived = false;

//static void S1_User_Handler(GPIO_PIN pin, uintptr_t context)

static void  S1_User_Handler (CN_PIN pin, uintptr_t context)
{
    /*if(S1_Get() == SWITCH_PRESSED_STATE)
    {
        changeTempSamplingRate = true;      
    }*/
      LD4_Toggle();
}

static void tmr2EventHandler (uint32_t intCause, uintptr_t context)
{
    isTmr2Expired = true;                              
}


static void printDMA()
{
   
 if(isUARTTxComplete)
        {
            isUARTTxComplete = false;
                    
            int len;
            len=strlen((const char*)uartTxBuffer);
            DMAC_ChannelTransfer(DMAC_CHANNEL_0, (const void *)uartTxBuffer, \
                    len, \
                    (const void *)&U1TXREG, 1, 1);
        }

}



static void UARTDmaChannelHandlerTX(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        isUARTTxComplete = true;
    }
}

static void UARTDmaChannelHandlerRX(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        isKeyreceived = true;       
    }
}
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_0, UARTDmaChannelHandlerTX, 0);
    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_1, UARTDmaChannelHandlerRX, 0);
    TMR2_CallbackRegister(tmr2EventHandler, 0);
    GPIO_PinInterruptCallbackRegister(CN15_PIN, S1_User_Handler, 0);
    GPIO_PinInterruptEnable(CN15_PIN);
    
    TMR2_Start();
    
    isUARTTxComplete = true;
    sprintf((char*)uartTxBuffer, "\n\n\rDMA USART RX/TX sample push S1 or type 10 characters\n\r");
    printDMA();
    
    DMAC_ChannelTransfer(DMAC_CHANNEL_1, (const void *)&U1RXREG, \
           1, (const void *)uartRxBuffer, 10, 1);
    
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        // callbacks for DMA 
        if(DCH0INTbits.CHBCIF)DMA_0_InterruptHandler();
        if(DCH1INTbits.CHBCIF)DMA_1_InterruptHandler();
        
        if (true == isTmr2Expired)
        {
           counter++;
           sprintf((char*)uartTxBuffer, "Temperature = %02d F\r\n", counter);
           printDMA();
           
           isTmr2Expired = false;
           
           /*DMAC_ChannelTransfer(DMAC_CHANNEL_0, (const void *)uartTxBuffer, \
                    strlen((const char*)uartTxBuffer), \
                    (const void *)&U1TXREG, 1, 1);*/
          
           LD5_Toggle();
           TMR2_Start();
                      
        }
        
      
       if(isKeyreceived)
       {
           isKeyreceived = false;
                 
           sprintf((char*)uartTxBuffer, "DMA Data Received!!: %s\n\r", uartRxBuffer);
           printDMA();
         
           DMAC_ChannelTransfer(DMAC_CHANNEL_1, (const void *)&U1RXREG, \
           1, (const void *)uartRxBuffer, 10, 1);
                 
       }
        
        
        
        /*
        if(S1_Get())
        {
            LD4_Set();        
        }
        else{
            LD4_Clear();
        }*/
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

