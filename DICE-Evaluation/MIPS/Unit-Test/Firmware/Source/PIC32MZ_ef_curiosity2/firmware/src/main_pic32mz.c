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
#include "definitions.h"                // SYS function prototypes
#include "device_cache.h"


static volatile int counter=0;
static volatile bool isTmr1Expired = false;

static volatile bool isKeyreceived = false;

//static volatile bool changeTempSamplingRate = false;
static volatile bool isUARTTxComplete = true;
//static volatile bool isTemperatureRead = false;
//static uint8_t temperatureVal;
static uint8_t __attribute__ ((aligned (16))) uartTxBuffer[100] = {0};
static uint8_t __attribute__ ((aligned (16))) uartRxBuffer[100];

/*
static void SW1_User_Handler(GPIO_PIN pin, uintptr_t context)
{
    if(S1_Get() )
    {
        LED1_Toggle();     
    }
}*/

static void tmr1EventHandler (uint32_t intCause, uintptr_t context)
{
    isTmr1Expired = true;  
    LED2_Toggle(); 
    /*
    counter++;
    if (counter>5)
    {
        if(isUARTTxComplete)  isUARTTxComplete =  false;        
        counter=0;
    }*/
    
}


static void UARTDmaChannelHandlerTX(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        isUARTTxComplete = true;
        LED3_Set();
    }
}


static void UARTDmaChannelHandlerRX(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
    {
        isKeyreceived = true;       
    }
}



static void printDMA()
{
   
 if(isUARTTxComplete)
        {
            isUARTTxComplete = false;
            
            //  Write back and invalidate an address range in the data cache.
            DCACHE_CLEAN_BY_ADDR((uint32_t)uartTxBuffer, sizeof(uartTxBuffer)); 
            int len;
            len=strlen((const char*)uartTxBuffer);
            DMAC_ChannelTransfer(DMAC_CHANNEL_0, (const void *)uartTxBuffer, \
                    len, \
                    (const void *)&U1TXREG, 1, 1);
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
    
    
    TMR1_CallbackRegister(tmr1EventHandler, 0);
    //GPIO_PinInterruptCallbackRegister(S1_PIN, SW1_User_Handler, 0);
    //GPIO_PinInterruptEnable(S1_PIN);

    /* Start the timer 1 */
    TMR1_Start(); 
    
    
    LED1_Clear();
    LED2_Clear();
    LED3_Clear();
    
    sprintf((char*)uartTxBuffer, "\n\n\rDMA USART RX/TX sample push S1 or type 10 characters\n\r");
    printDMA();
    
    //  Write back and invalidate an address range in the data cache.
    DCACHE_CLEAN_BY_ADDR((uint32_t)uartRxBuffer, sizeof(uartRxBuffer));         
    DMAC_ChannelTransfer(DMAC_CHANNEL_1, (const void *)&U1RXREG, \
           1, (const void *)uartRxBuffer, 10, 1);
        
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
       SYS_Tasks ( );
       
       // callbacks for DMA 
       if(DCH0INTbits.CHBCIF)DMA0_InterruptHandler();
       if(DCH1INTbits.CHBCIF)DMA1_InterruptHandler();
       
       
       if(!S1_Get() )
       {
        LED1_Toggle();  
        LED3_Clear();
        while(!S1_Get());
        
        counter++;
        sprintf((char*)uartTxBuffer, "Printing! DMA sample, counter:%d \n\r", counter);
        printDMA();        
       
       }       
  
       if(isKeyreceived)
       {
           isKeyreceived = false;
                 
           sprintf((char*)uartTxBuffer, "DMA Data Received!!: %s\n\r", uartRxBuffer);
           printDMA();
             
           //  Write back and invalidate an address range in the data cache.
           DCACHE_CLEAN_BY_ADDR((uint32_t)uartRxBuffer, sizeof(uartRxBuffer));
           DMAC_ChannelTransfer(DMAC_CHANNEL_1, (const void *)&U1RXREG, \
           1, (const void *)uartRxBuffer, 10, 1);
                 
       }
                                                            
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

