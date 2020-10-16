/*******************************************************************************
  SYS PORTS Static Functions for PORTS System Service

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.c

  Summary:
    GPIO function implementations for the GPIO PLIB.

  Description:
    The GPIO PLIB provides a simple interface to manage peripheral
    input-output controller.

*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#include "plib_gpio.h"

#define TOTAL_NUM_OF_INT_USED 1
/* Array to store pin objects of each configured interrupt */
GPIO_PIN_CALLBACK_OBJ cnPinObj[TOTAL_NUM_OF_INT_USED] =
    {
        {.cnPin = CN15_PIN , .gpioPin = GPIO_PIN_RD6, .callback = NULL },
    };


/******************************************************************************
  Function:
    GPIO_Initialize ( void )

  Summary:
    Initialize the GPIO library.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_Initialize ( void )
{

    /* PORTA Initialization */
    LATA = 0x0; /* Initial Latch Value */
    TRISACLR = 0x8; /* Direction Control */

    /* PORTB Initialization */

    /* PORTC Initialization */
    LATC = 0x0; /* Initial Latch Value */
    TRISCCLR = 0x2; /* Direction Control */

    /* PORTD Initialization */

    /* PORTE Initialization */
    LATE = 0x10; /* Initial Latch Value */
    TRISECLR = 0x10; /* Direction Control */

    /* PORTF Initialization */

    /* PORTG Initialization */


    CNPUESET = 0x8000; /* Pull-Up Enable */
    /* Change Notice Enable */
    CNCONSET = _CNCON_ON_MASK;
    IEC1SET = _IEC1_CNIE_MASK;

    uint8_t i, bitPosition;
    uint32_t latestPortValue, mask;

    /* save the initial pin value for CN pins */
    for(i = 0; i < TOTAL_NUM_OF_INT_USED; i++)
    {
        latestPortValue = *(volatile uint32_t *)(&PORTA + ((cnPinObj[i].gpioPin >> 4) * 0x10));
        bitPosition = cnPinObj[i].gpioPin % 16;
        mask = 1 << bitPosition;
        cnPinObj[i].prevPinValue = (bool)((latestPortValue & mask) >> bitPosition);
    }

}

// *****************************************************************************
// *****************************************************************************
// Section: GPIO APIs which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    uint32_t GPIO_PortRead ( GPIO_PORT port )

  Summary:
    Read all the I/O lines of the selected port.

  Description:
    This function reads the live data values on all the I/O lines of the
    selected port.  Bit values returned in each position indicate corresponding
    pin levels.
    1 = Pin is high.
    0 = Pin is low.

    This function reads the value regardless of pin configuration, whether it is
    set as as an input, driven by the GPIO Controller, or driven by a peripheral.

  Remarks:
    If the port has less than 32-bits, unimplemented pins will read as
    low (0).
    Implemented pins are Right aligned in the 32-bit return value.
*/
uint32_t GPIO_PortRead(GPIO_PORT port)
{
    return (*(volatile uint32_t *)(&PORTA + (port * 0x10)));
}

// *****************************************************************************
/* Function:
    void GPIO_PortWrite (GPIO_PORT port, uint32_t mask, uint32_t value);

  Summary:
    Write the value on the masked I/O lines of the selected port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value)
{
    *(volatile uint32_t *)(&LATA + (port * 0x10)) = (*(volatile uint32_t *)(&LATA + (port * 0x10)) & (~mask)) | (mask & value);
}

// *****************************************************************************
/* Function:
    uint32_t GPIO_PortLatchRead ( GPIO_PORT port )

  Summary:
    Read the latched value on all the I/O lines of the selected port.

  Remarks:
    See plib_gpio.h for more details.
*/
uint32_t GPIO_PortLatchRead(GPIO_PORT port)
{
    return (*(volatile uint32_t *)(&LATA + (port * 0x10)));
}

// *****************************************************************************
/* Function:
    void GPIO_PortSet ( GPIO_PORT port, uint32_t mask )

  Summary:
    Set the selected IO pins of a port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortSet(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&LATASET + (port * 0x10)) = mask;
}

// *****************************************************************************
/* Function:
    void GPIO_PortClear ( GPIO_PORT port, uint32_t mask )

  Summary:
    Clear the selected IO pins of a port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortClear(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&LATACLR + (port * 0x10)) = mask;
}

// *****************************************************************************
/* Function:
    void GPIO_PortToggle ( GPIO_PORT port, uint32_t mask )

  Summary:
    Toggles the selected IO pins of a port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortToggle(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&LATAINV + (port * 0x10))= mask;
}

// *****************************************************************************
/* Function:
    void GPIO_PortInputEnable ( GPIO_PORT port, uint32_t mask )

  Summary:
    Enables selected IO pins of a port as input.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&TRISASET + (port * 0x10)) = mask;
}

// *****************************************************************************
/* Function:
    void GPIO_PortOutputEnable ( GPIO_PORT port, uint32_t mask )

  Summary:
    Enables selected IO pins of a port as output(s).

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&TRISACLR + (port * 0x10)) = mask;
}


void GPIO_PinInterruptEnable(CN_PIN cnPin)
{
    CNENSET = cnPin;
}

void GPIO_PinInterruptDisable(CN_PIN cnPin)
{
    CNENCLR = cnPin;
}

// *****************************************************************************
// *****************************************************************************
// Section: GPIO APIs which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************
bool GPIO_PinInterruptCallbackRegister(
    CN_PIN cnPin,
    const GPIO_PIN_CALLBACK callback,
    uintptr_t context
)
{
    uint8_t i;

    for(i = 0; i < TOTAL_NUM_OF_INT_USED; i++)
    {
        if (cnPinObj[i].cnPin == cnPin)
        {
            cnPinObj[i].callback = callback;
            cnPinObj[i].context  = context;
            return true;
        }
    }
    return false;
}

// *****************************************************************************
// *****************************************************************************
// Section: Local Function Implementation
// *****************************************************************************
// *****************************************************************************

/* Function:
    void CHANGE_NOTICE_InterruptHandler()

  Summary:
    Interrupt Handler for change notice interrupt.

  Remarks:
	It is an internal function called from ISR, user should not call it directly.
*/
void CHANGE_NOTICE_InterruptHandler()
{
    uint8_t i, bitPosition;
    uint32_t latestPortValue, mask;
    bool currPinValue;

    /* Check which CN interrupt has occurred and call callback if registered */
    for(i = 0; i < TOTAL_NUM_OF_INT_USED; i++)
    {
        latestPortValue = *(volatile uint32_t *)(&PORTA + ((cnPinObj[i].gpioPin >> 4) * 0x10));
        bitPosition = cnPinObj[i].gpioPin % 16;
        mask = 1 << bitPosition;
        currPinValue = (bool)((latestPortValue & mask) >> bitPosition);
        if((cnPinObj[i].prevPinValue != currPinValue) && (cnPinObj[i].callback != NULL))
        {
            cnPinObj[i].prevPinValue = currPinValue;
            cnPinObj[i].callback (cnPinObj[i].cnPin, cnPinObj[i].context);
        }
    }
    IFS1CLR = _IFS1_CNIF_MASK;
}

/*******************************************************************************
 End of File
*/
