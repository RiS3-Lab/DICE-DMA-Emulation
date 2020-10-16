/*******************************************************************************
 System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt.c

  Summary:
    Interrupt vectors mapping

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


void TIMER_3_InterruptHandler( void );
void I2C_1_InterruptHandler( void );
void CHANGE_NOTICE_InterruptHandler( void );
void UART_2_InterruptHandler( void );
void DMA_0_InterruptHandler( void );



/* All the handlers are defined here.  Each will call its PLIB-specific function. */
void __ISR(_TIMER_3_VECTOR, ipl1AUTO) TIMER_3_Handler (void)
{
    TIMER_3_InterruptHandler();
}

void __ISR(_I2C_1_VECTOR, ipl1AUTO) I2C_1_Handler (void)
{
    I2C_1_InterruptHandler();
}

void __ISR(_CHANGE_NOTICE_VECTOR, ipl1AUTO) CHANGE_NOTICE_Handler (void)
{
    CHANGE_NOTICE_InterruptHandler();
}

void __ISR(_UART_2_VECTOR, ipl1AUTO) UART_2_Handler (void)
{
    UART_2_InterruptHandler();
}

void __ISR(_DMA_0_VECTOR, ipl1AUTO) DMA_0_Handler (void)
{
    DMA_0_InterruptHandler();
}




/*******************************************************************************
 End of File
*/
