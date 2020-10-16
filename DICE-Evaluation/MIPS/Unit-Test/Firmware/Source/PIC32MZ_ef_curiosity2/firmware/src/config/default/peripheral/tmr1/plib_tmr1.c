/*******************************************************************************
  TMR1 Peripheral Library Interface Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_tmr1.c

  Summary
    TMR1 peripheral library source file.

  Description
    This file implements the interface to the TMR1 peripheral library.  This
    library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_tmr1.h"

static TMR1_TIMER_OBJECT tmr1Obj;

void TMR1_Initialize(void)
{
    /* Disable Timer */
    T1CONCLR = _T1CON_ON_MASK;

    /*
    SIDL = 0
    TWDIS = 0
    TGATE = 0
    TCKPS = 3
    TSYNC = 0
    TCS = 0
    */
    T1CONSET = 0x30;

    /* Clear counter */
    TMR1 = 0x0;

    /*Set period */
    PR1 = 65535;

    /* Setup TMR1 Interrupt */
    TMR1_InterruptEnable();  /* Enable interrupt on the way out */
}


void TMR1_Start (void)
{
    T1CONSET = _T1CON_ON_MASK;
}


void TMR1_Stop (void)
{
    T1CONCLR = _T1CON_ON_MASK;
}


void TMR1_PeriodSet(uint16_t period)
{
    PR1 = period;
}


uint16_t TMR1_PeriodGet(void)
{
    return (uint16_t)PR1;
}


uint16_t TMR1_CounterGet(void)
{
    return(TMR1);
}

uint32_t TMR1_FrequencyGet(void)
{
    return (390625);
}

void TIMER_1_InterruptHandler (void)
{
    uint32_t status = IFS0bits.T1IF;
    IFS0CLR = _IFS0_T1IF_MASK;

    if((tmr1Obj.callback_fn != NULL))
    {
        tmr1Obj.callback_fn(status, tmr1Obj.context);
    }
}


void TMR1_InterruptEnable(void)
{
    IEC0SET = _IEC0_T1IE_MASK;
}


void TMR1_InterruptDisable(void)
{
    IEC0CLR = _IEC0_T1IE_MASK;
}


void TMR1_CallbackRegister( TMR1_CALLBACK callback_fn, uintptr_t context )
{
    /* - Save callback_fn and context in local memory */
    tmr1Obj.callback_fn = callback_fn;
    tmr1Obj.context = context;
}
