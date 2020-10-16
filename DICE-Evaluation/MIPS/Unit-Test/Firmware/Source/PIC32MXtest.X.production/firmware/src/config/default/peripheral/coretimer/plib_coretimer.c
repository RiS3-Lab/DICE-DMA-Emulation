/*******************************************************************************
  Core Timer Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_coretimer.c

  Summary:
    Core timer Source File

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
#include "peripheral/coretimer/plib_coretimer.h"



static uint32_t compareValue = CORE_TIMER_COMPARE_VALUE;

void CORETIMER_Initialize()
{

    // Clear Core Timer
    _CP0_SET_COUNT(0);
    _CP0_SET_COMPARE(compareValue);

    // Enable Timer by clearing Disable Count (DC) bit
    _CP0_SET_CAUSE(_CP0_GET_CAUSE() & (~_CP0_CAUSE_DC_MASK));
}

void CORETIMER_Start( void )
{
    // Disable Timer by setting Disable Count (DC) bit
    _CP0_SET_CAUSE(_CP0_GET_CAUSE() | _CP0_CAUSE_DC_MASK);

    // Clear Compare Timer Interrupt Flag
    IFS0CLR=0x1;

    // Clear Core Timer
    _CP0_SET_COUNT(0);

    _CP0_SET_COMPARE(compareValue);

    // Enable Timer by clearing Disable Count (DC) bit
    _CP0_SET_CAUSE(_CP0_GET_CAUSE() & (~_CP0_CAUSE_DC_MASK));

}

void CORETIMER_Stop( void )
{
    // Disable Timer by setting Disable Count (DC) bit
    _CP0_SET_CAUSE(_CP0_GET_CAUSE() | _CP0_CAUSE_DC_MASK);
}

uint32_t CORETIMER_FrequencyGet ( void )
{
    return (CORE_TIMER_FREQUENCY);
}

void CORETIMER_CompareSet ( uint32_t compare )
{
    compareValue = compare;
    _CP0_SET_COMPARE(compareValue);
}

uint32_t CORETIMER_CounterGet ( void )
{
    uint32_t count;
    count = _CP0_GET_COUNT();
    return count;
}

bool CORETIMER_CompareHasExpired( void )
{
    if (IFS0bits.CTIF != 0)
    {
        // Clear Compare Timer Interrupt Flag
        IFS0CLR=0x1;

        return true;
    }

    return false;
}

void CORETIMER_DelayMs ( uint32_t delay_ms)
{
    uint32_t startCount, endCount;
    /* Calculate the end count for the given delay */
    endCount=(CORE_TIMER_FREQUENCY/1000)*delay_ms;
    startCount=_CP0_GET_COUNT();
    while((_CP0_GET_COUNT()-startCount)<endCount);
}

void CORETIMER_DelayUs ( uint32_t delay_us)
{
    uint32_t startCount, endCount;
    /* Calculate the end count for the given delay */
    endCount=(CORE_TIMER_FREQUENCY/1000000)*delay_us;
    startCount=_CP0_GET_COUNT();
    while((_CP0_GET_COUNT()-startCount)<endCount);
}
