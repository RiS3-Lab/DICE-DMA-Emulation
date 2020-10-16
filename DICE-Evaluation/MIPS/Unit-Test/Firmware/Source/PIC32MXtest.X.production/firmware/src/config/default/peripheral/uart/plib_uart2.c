/*******************************************************************************
  UART2 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_uart2.c

  Summary:
    UART2 PLIB Implementation File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#include "device.h"
#include "plib_uart2.h"

// *****************************************************************************
// *****************************************************************************
// Section: UART2 Implementation
// *****************************************************************************
// *****************************************************************************

UART_OBJECT uart2Obj;

void static UART2_ErrorClear( void )
{
    /* rxBufferLen = (FIFO level + RX register) */
    uint8_t rxBufferLen = UART_RXFIFO_DEPTH;
    uint8_t dummyData = 0u;

    /* If it's a overrun error then clear it to flush FIFO */
    if(U2STA & _U2STA_OERR_MASK)
    {
        U2STACLR = _U2STA_OERR_MASK;
    }

    /* Read existing error bytes from FIFO to clear parity and framing error flags */
    while(U2STA & (_U2STA_FERR_MASK | _U2STA_PERR_MASK))
    {
        dummyData = (uint8_t )(U2RXREG );
        rxBufferLen--;

        /* Try to flush error bytes for one full FIFO and exit instead of
         * blocking here if more error bytes are received */
        if(rxBufferLen == 0u)
        {
            break;
        }
    }

    // Ignore the warning
    (void)dummyData;

    /* Clear error interrupt flag */
    IFS1CLR = _IFS1_U2EIF_MASK;

    /* Clear up the receive interrupt flag so that RX interrupt is not
     * triggered for error bytes */
    IFS1CLR = _IFS1_U2RXIF_MASK;

    return;
}

void UART2_Initialize( void )
{
    /* Set up UxMODE bits */
    /* STSEL  = 0 */
    /* PDSEL = 0 */

    U2MODE = 0x0;

    /* Enable UART2 Receiver and Transmitter */
    U2STASET = (_U2STA_UTXEN_MASK | _U2STA_URXEN_MASK);

    /* BAUD Rate register Setup */
    U2BRG = 25;

    /* Disable Interrupts */
    IEC1CLR = _IEC1_U2EIE_MASK;

    IEC1CLR = _IEC1_U2RXIE_MASK;

    IEC1CLR = _IEC1_U2TXIE_MASK;

    /* Initialize instance object */
    uart2Obj.rxBuffer = NULL;
    uart2Obj.rxSize = 0;
    uart2Obj.rxProcessedSize = 0;
    uart2Obj.rxBusyStatus = false;
    uart2Obj.rxCallback = NULL;
    uart2Obj.txBuffer = NULL;
    uart2Obj.txSize = 0;
    uart2Obj.txProcessedSize = 0;
    uart2Obj.txBusyStatus = false;
    uart2Obj.txCallback = NULL;

    /* Turn ON UART2 */
    U2MODESET = _U2MODE_ON_MASK;
}

bool UART2_SerialSetup( UART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    bool status = false;
    uint32_t baud = setup->baudRate;
    uint32_t brgValHigh = 0;
    uint32_t brgValLow = 0;
    uint32_t brgVal = 0;
    uint32_t uartMode;

    if((uart2Obj.rxBusyStatus == true) || (uart2Obj.txBusyStatus == true))
    {
        /* Transaction is in progress, so return without updating settings */
        return status;
    }

    if (setup != NULL)
    {
        if(srcClkFreq == 0)
        {
            srcClkFreq = UART2_FrequencyGet();
        }

        /* Calculate BRG value */
        brgValLow = ((srcClkFreq / baud) >> 4) - 1;
        brgValHigh = ((srcClkFreq / baud) >> 2) - 1;

        /* Check if the baud value can be set with low baud settings */
        if((brgValHigh >= 0) && (brgValHigh <= UINT16_MAX))
        {
            brgVal =  (((srcClkFreq >> 2) + (baud >> 1)) / baud ) - 1;
            U2MODESET = _U2MODE_BRGH_MASK;
        }
        else if ((brgValLow >= 0) && (brgValLow <= UINT16_MAX))
        {
            brgVal = ( ((srcClkFreq >> 4) + (baud >> 1)) / baud ) - 1;
            U2MODECLR = _U2MODE_BRGH_MASK;
        }
        else
        {
            return status;
        }

        if(setup->dataWidth == UART_DATA_9_BIT)
        {
            if(setup->parity != UART_PARITY_NONE)
            {
               return status;
            }
            else
            {
               /* Configure UART2 mode */
               uartMode = U2MODE;
               uartMode &= ~_U2MODE_PDSEL_MASK;
               U2MODE = uartMode | setup->dataWidth;
            }
        }
        else
        {
            /* Configure UART2 mode */
            uartMode = U2MODE;
            uartMode &= ~_U2MODE_PDSEL_MASK;
            U2MODE = uartMode | setup->parity ;
        }

        /* Configure UART2 mode */
        uartMode = U2MODE;
        uartMode &= ~_U2MODE_STSEL_MASK;
        U2MODE = uartMode | setup->stopBits ;

        /* Configure UART2 Baud Rate */
        U2BRG = brgVal;

        status = true;
    }

    return status;
}

bool UART2_Read(void* buffer, const size_t size )
{
    bool status = false;
    uint8_t* lBuffer = (uint8_t* )buffer;

    if(lBuffer != NULL)
    {
        /* Check if receive request is in progress */
        if(uart2Obj.rxBusyStatus == false)
        {
            /* Clear errors before submitting the request.
             * ErrorGet clears errors internally. */
            UART2_ErrorGet();

            uart2Obj.rxBuffer = lBuffer;
            uart2Obj.rxSize = size;
            uart2Obj.rxProcessedSize = 0;
            uart2Obj.rxBusyStatus = true;
            status = true;

            /* Enable UART2_FAULT Interrupt */
            IEC1SET = _IEC1_U2EIE_MASK;

            /* Enable UART2_RX Interrupt */
            IEC1SET = _IEC1_U2RXIE_MASK;
        }
    }

    return status;
}

bool UART2_Write( void* buffer, const size_t size )
{
    bool status = false;
    uint8_t* lBuffer = (uint8_t*)buffer;

    if(lBuffer != NULL)
    {
        /* Check if transmit request is in progress */
        if(uart2Obj.txBusyStatus == false)
        {
            uart2Obj.txBuffer = lBuffer;
            uart2Obj.txSize = size;
            uart2Obj.txProcessedSize = 0;
            uart2Obj.txBusyStatus = true;
            status = true;

            /* Initiate the transfer by sending first byte */
            if(!(U2STA & _U2STA_UTXBF_MASK))
            {
                U2TXREG = *lBuffer;
                uart2Obj.txProcessedSize++;
            }

            IEC1SET = _IEC1_U2TXIE_MASK;
        }
    }

    return status;
}

UART_ERROR UART2_ErrorGet( void )
{
    UART_ERROR errors = UART_ERROR_NONE;
    uint32_t status = U2STA;

    errors = (UART_ERROR)(status & (_U2STA_OERR_MASK | _U2STA_FERR_MASK | _U2STA_PERR_MASK));

    if(errors != UART_ERROR_NONE)
    {
        UART2_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

void UART2_ReadCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart2Obj.rxCallback = callback;

    uart2Obj.rxContext = context;
}

bool UART2_ReadIsBusy( void )
{
    return uart2Obj.rxBusyStatus;
}

size_t UART2_ReadCountGet( void )
{
    return uart2Obj.rxProcessedSize;
}

void UART2_WriteCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart2Obj.txCallback = callback;

    uart2Obj.txContext = context;
}

bool UART2_WriteIsBusy( void )
{
    return uart2Obj.txBusyStatus;
}

size_t UART2_WriteCountGet( void )
{
    return uart2Obj.txProcessedSize;
}

static void UART2_FAULT_InterruptHandler (void)
{
    /* Disable the fault interrupt */
    IEC1CLR = _IEC1_U2EIE_MASK;
    /* Disable the receive interrupt */
    IEC1CLR = _IEC1_U2RXIE_MASK;

    /* Clear rx status */
    uart2Obj.rxBusyStatus = false;

    /* Client must call UARTx_ErrorGet() function to clear the errors */
    if( uart2Obj.rxCallback != NULL )
    {
        uart2Obj.rxCallback(uart2Obj.rxContext);
    }
}

static void UART2_RX_InterruptHandler (void)
{
    if(uart2Obj.rxBusyStatus == true)
    {
        while((_U2STA_URXDA_MASK == (U2STA & _U2STA_URXDA_MASK)) && (uart2Obj.rxSize > uart2Obj.rxProcessedSize) )
        {
            uart2Obj.rxBuffer[uart2Obj.rxProcessedSize++] = (uint8_t )(U2RXREG);
        }

        /* Clear UART2 RX Interrupt flag after reading data buffer */
        IFS1CLR = _IFS1_U2RXIF_MASK;

        /* Check if the buffer is done */
        if(uart2Obj.rxProcessedSize >= uart2Obj.rxSize)
        {
            uart2Obj.rxBusyStatus = false;

            /* Disable the receive interrupt */
            IEC1CLR = _IEC1_U2RXIE_MASK;

            if(uart2Obj.rxCallback != NULL)
            {
                uart2Obj.rxCallback(uart2Obj.rxContext);
            }
        }
    }
    else
    {
        // Nothing to process
        ;
    }
}

static void UART2_TX_InterruptHandler (void)
{
    if(uart2Obj.txBusyStatus == true)
    {
        while((!(U2STA & _U2STA_UTXBF_MASK)) && (uart2Obj.txSize > uart2Obj.txProcessedSize) )
        {
            U2TXREG = uart2Obj.txBuffer[uart2Obj.txProcessedSize++];
        }

        /* Clear UART2TX Interrupt flag after writing to buffer */
        IFS1CLR = _IFS1_U2TXIF_MASK;

        /* Check if the buffer is done */
        if(uart2Obj.txProcessedSize >= uart2Obj.txSize)
        {
            uart2Obj.txBusyStatus = false;

            /* Disable the transmit interrupt, to avoid calling ISR continuously */
            IEC1CLR = _IEC1_U2TXIE_MASK;

            if(uart2Obj.txCallback != NULL)
            {
                uart2Obj.txCallback(uart2Obj.txContext);
            }
        }
    }
    else
    {
        // Nothing to process
        ;
    }
}

void UART_2_InterruptHandler (void)
{
    /* Call RX handler if RX interrupt flag is set */
    if ((IFS1 & _IFS1_U2RXIF_MASK) && (IEC1 & _IEC1_U2RXIE_MASK))
    {
        UART2_RX_InterruptHandler();
    }
    /* Call TX handler if TX interrupt flag is set */
    else if ((IFS1 & _IFS1_U2TXIF_MASK) && (IEC1 & _IEC1_U2TXIE_MASK))
    {
        UART2_TX_InterruptHandler();
    }
    /* Call Error handler if Error interrupt flag is set */
    else if ((IFS1 & _IFS1_U2EIF_MASK) && (IEC1 & _IEC1_U2EIE_MASK))
    {
        UART2_FAULT_InterruptHandler();
    }
}

