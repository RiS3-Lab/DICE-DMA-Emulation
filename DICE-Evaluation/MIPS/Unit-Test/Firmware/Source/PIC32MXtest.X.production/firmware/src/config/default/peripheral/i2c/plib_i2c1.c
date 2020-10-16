/*******************************************************************************
  Inter-Integrated Circuit (I2C) Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_i2c1.c

  Summary:
    I2C PLIB Implementation file

  Description:
    This file defines the interface to the I2C peripheral library.
    This library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018-2019 Microchip Technology Inc. and its subsidiaries.
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
#include "plib_i2c1.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


static I2C_OBJ i2c1Obj;

void I2C1_Initialize(void)
{
    /* Disable the I2C Master interrupt */
    IEC0CLR = _IEC0_I2C1MIE_MASK;

    /* Disable the I2C Bus collision interrupt */
    IEC0CLR = _IEC0_I2C1BIE_MASK;

    I2C1BRG = 475;

    I2C1CONCLR = _I2C1CON_SIDL_MASK;
    I2C1CONCLR = _I2C1CON_DISSLW_MASK;
    I2C1CONCLR = _I2C1CON_SMEN_MASK;

    /* Clear master interrupt flag */
    IFS0CLR = _IFS0_I2C1MIF_MASK;

    /* Clear fault interrupt flag */
    IFS0CLR = _IFS0_I2C1BIF_MASK;

    /* Turn on the I2C module */
    I2C1CONSET = _I2C1CON_ON_MASK;

    /* Set the initial state of the I2C state machine */
    i2c1Obj.state = I2C_STATE_IDLE;
}

/* I2C state machine */
static void I2C1_TransferSM(void)
{
    IFS0CLR = _IFS0_I2C1MIF_MASK;

    switch (i2c1Obj.state)
    {
        case I2C_STATE_START_CONDITION:
            /* Generate Start Condition */
            I2C1CONSET = _I2C1CON_SEN_MASK;
            IEC0SET = _IEC0_I2C1MIE_MASK;
            IEC0SET = _IEC0_I2C1BIE_MASK;
            i2c1Obj.state = I2C_STATE_ADDR_BYTE_1_SEND;
            break;

        case I2C_STATE_ADDR_BYTE_1_SEND:
            /* Is transmit buffer full? */
            if (!(I2C1STAT & _I2C1STAT_TBF_MASK))
            {
                if (i2c1Obj.address > 0x007F)
                {
                    /* Transmit the MSB 2 bits of the 10-bit slave address, with R/W = 0 */
                    I2C1TRN = ( 0xF0 | (((uint8_t*)&i2c1Obj.address)[1] << 1));

                    i2c1Obj.state = I2C_STATE_ADDR_BYTE_2_SEND;
                }
                else
                {
                    /* 8-bit addressing mode */
                    I2C1TRN = ((i2c1Obj.address << 1) | i2c1Obj.transferType);

                    if (i2c1Obj.transferType == I2C_TRANSFER_TYPE_WRITE)
                    {
                        i2c1Obj.state = I2C_STATE_WRITE;
                    }
                    else
                    {
                        i2c1Obj.state = I2C_STATE_READ;
                    }
                }
            }
            break;

        case I2C_STATE_ADDR_BYTE_2_SEND:
            /* Transmit the 2nd byte of the 10-bit slave address */
            if (!(I2C1STAT & _I2C1STAT_ACKSTAT_MASK))
            {
                if (!(I2C1STAT & _I2C1STAT_TBF_MASK))
                {
                    /* Transmit the remaining 8-bits of the 10-bit address */
                    I2C1TRN = i2c1Obj.address;

                    if (i2c1Obj.transferType == I2C_TRANSFER_TYPE_WRITE)
                    {
                        i2c1Obj.state = I2C_STATE_WRITE;
                    }
                    else
                    {
                        i2c1Obj.state = I2C_STATE_READ_10BIT_MODE;
                    }
                }
            }
            else
            {
                /* NAK received. Generate Stop Condition. */
                i2c1Obj.error = I2C_ERROR_NACK;
                I2C1CONSET = _I2C1CON_PEN_MASK;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_READ_10BIT_MODE:
            if (!(I2C1STAT & _I2C1STAT_ACKSTAT_MASK))
            {
                /* Generate repeated start condition */
                I2C1CONSET = _I2C1CON_RSEN_MASK;
                i2c1Obj.state = I2C_STATE_ADDR_BYTE_1_SEND_10BIT_ONLY;
            }
            else
            {
                /* NAK received. Generate Stop Condition. */
                i2c1Obj.error = I2C_ERROR_NACK;
                I2C1CONSET = _I2C1CON_PEN_MASK;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_ADDR_BYTE_1_SEND_10BIT_ONLY:
            /* Is transmit buffer full? */
            if (!(I2C1STAT & _I2C1STAT_TBF_MASK))
            {
                /* Transmit the first byte of the 10-bit slave address, with R/W = 1 */
                I2C1TRN = ( 0xF1 | ((((uint8_t*)&i2c1Obj.address)[1] << 1)));
                i2c1Obj.state = I2C_STATE_READ;
            }
            else
            {
                /* NAK received. Generate Stop Condition. */
                i2c1Obj.error = I2C_ERROR_NACK;
                I2C1CONSET = _I2C1CON_PEN_MASK;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_WRITE:
            if (!(I2C1STAT & _I2C1STAT_ACKSTAT_MASK))
            {
                /* ACK received */
                if (i2c1Obj.writeCount < i2c1Obj.writeSize)
                {
                    if (!(I2C1STAT & _I2C1STAT_TBF_MASK))
                    {
                        /* Transmit the data from writeBuffer[] */
                        I2C1TRN = i2c1Obj.writeBuffer[i2c1Obj.writeCount++];
                    }
                }
                else
                {
                    if (i2c1Obj.readCount < i2c1Obj.readSize)
                    {
                        /* Generate repeated start condition */
                        I2C1CONSET = _I2C1CON_RSEN_MASK;

                        i2c1Obj.transferType = I2C_TRANSFER_TYPE_READ;

                        if (i2c1Obj.address > 0x007F)
                        {
                            /* Send the I2C slave address with R/W = 1 */
                            i2c1Obj.state = I2C_STATE_ADDR_BYTE_1_SEND_10BIT_ONLY;
                        }
                        else
                        {
                            /* Send the I2C slave address with R/W = 1 */
                            i2c1Obj.state = I2C_STATE_ADDR_BYTE_1_SEND;
                        }

                    }
                    else
                    {
                        /* Transfer Complete. Generate Stop Condition */
                        I2C1CONSET = _I2C1CON_PEN_MASK;
                        i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
                    }
                }
            }
            else
            {
                /* NAK received. Generate Stop Condition. */
                i2c1Obj.error = I2C_ERROR_NACK;
                I2C1CONSET = _I2C1CON_PEN_MASK;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_READ:
            if (!(I2C1STAT & _I2C1STAT_ACKSTAT_MASK))
            {
                /* Slave ACK'd the device address. Enable receiver. */
                I2C1CONSET = _I2C1CON_RCEN_MASK;
                i2c1Obj.state = I2C_STATE_READ_BYTE;
            }
            else
            {
                /* NAK received. Generate Stop Condition. */
                i2c1Obj.error = I2C_ERROR_NACK;
                I2C1CONSET = _I2C1CON_PEN_MASK;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_READ_BYTE:
            /* Data received from the slave */
            if (I2C1STAT & _I2C1STAT_RBF_MASK)
            {
                i2c1Obj.readBuffer[i2c1Obj.readCount++] = I2C1RCV;
                if (i2c1Obj.readCount == i2c1Obj.readSize)
                {
                    /* Send NAK */
                    I2C1CONSET = _I2C1CON_ACKDT_MASK;
                    I2C1CONSET = _I2C1CON_ACKEN_MASK;
                }
                else
                {
                    /* Send ACK */
                    I2C1CONCLR = _I2C1CON_ACKDT_MASK;
                    I2C1CONSET = _I2C1CON_ACKEN_MASK;
                }
                i2c1Obj.state = I2C_STATE_WAIT_ACK_COMPLETE;
            }
            break;

        case I2C_STATE_WAIT_ACK_COMPLETE:
            /* ACK or NAK sent to the I2C slave */
            if (i2c1Obj.readCount < i2c1Obj.readSize)
            {
                /* Enable receiver */
                I2C1CONSET = _I2C1CON_RCEN_MASK;
                i2c1Obj.state = I2C_STATE_READ_BYTE;
            }
            else
            {
                /* Generate Stop Condition */
                I2C1CONSET = _I2C1CON_PEN_MASK;
                i2c1Obj.state = I2C_STATE_WAIT_STOP_CONDITION_COMPLETE;
            }
            break;

        case I2C_STATE_WAIT_STOP_CONDITION_COMPLETE:
            i2c1Obj.state = I2C_STATE_IDLE;
            IEC0CLR = _IEC0_I2C1MIE_MASK;
            IEC0CLR = _IEC0_I2C1BIE_MASK;
            if (i2c1Obj.callback != NULL)
            {
                i2c1Obj.callback(i2c1Obj.context);
            }
            break;

        default:
            break;
    }

}


void I2C1_CallbackRegister(I2C_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    i2c1Obj.callback = callback;
    i2c1Obj.context = contextHandle;
}

bool I2C1_IsBusy(void)
{
    if( (i2c1Obj.state != I2C_STATE_IDLE ) || (I2C1CON & 0x0000001F) ||
        (I2C1STAT & _I2C1STAT_TRSTAT_MASK) || (I2C1STAT & _I2C1STAT_S_MASK) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool I2C1_Read(uint16_t address, uint8_t* rdata, size_t rlength)
{
    /* State machine must be idle and I2C module should not have detected a start bit on the bus */
    if((i2c1Obj.state != I2C_STATE_IDLE) || (I2C1STAT & _I2C1STAT_S_MASK))
    {
        return false;
    }

    i2c1Obj.address             = address;
    i2c1Obj.readBuffer          = rdata;
    i2c1Obj.readSize            = rlength;
    i2c1Obj.writeBuffer         = NULL;
    i2c1Obj.writeSize           = 0;
    i2c1Obj.writeCount          = 0;
    i2c1Obj.readCount           = 0;
    i2c1Obj.transferType        = I2C_TRANSFER_TYPE_READ;
    i2c1Obj.error               = I2C_ERROR_NONE;
    i2c1Obj.state               = I2C_STATE_ADDR_BYTE_1_SEND;

    I2C1CONSET                  = _I2C1CON_SEN_MASK;
    IEC0SET                     = _IEC0_I2C1MIE_MASK;
    IEC0SET                     = _IEC0_I2C1BIE_MASK;

    return true;
}


bool I2C1_Write(uint16_t address, uint8_t* wdata, size_t wlength)
{
    /* State machine must be idle and I2C module should not have detected a start bit on the bus */
    if((i2c1Obj.state != I2C_STATE_IDLE) || (I2C1STAT & _I2C1STAT_S_MASK))
    {
        return false;
    }

    i2c1Obj.address             = address;
    i2c1Obj.readBuffer          = NULL;
    i2c1Obj.readSize            = 0;
    i2c1Obj.writeBuffer         = wdata;
    i2c1Obj.writeSize           = wlength;
    i2c1Obj.writeCount          = 0;
    i2c1Obj.readCount           = 0;
    i2c1Obj.transferType        = I2C_TRANSFER_TYPE_WRITE;
    i2c1Obj.error               = I2C_ERROR_NONE;
    i2c1Obj.state               = I2C_STATE_ADDR_BYTE_1_SEND;

    I2C1CONSET                  = _I2C1CON_SEN_MASK;
    IEC0SET                     = _IEC0_I2C1MIE_MASK;
    IEC0SET                     = _IEC0_I2C1BIE_MASK;

    return true;
}


bool I2C1_WriteRead(uint16_t address, uint8_t* wdata, size_t wlength, uint8_t* rdata, size_t rlength)
{
    /* State machine must be idle and I2C module should not have detected a start bit on the bus */
    if((i2c1Obj.state != I2C_STATE_IDLE) || (I2C1STAT & _I2C1STAT_S_MASK))
    {
        return false;
    }

    i2c1Obj.address             = address;
    i2c1Obj.readBuffer          = rdata;
    i2c1Obj.readSize            = rlength;
    i2c1Obj.writeBuffer         = wdata;
    i2c1Obj.writeSize           = wlength;
    i2c1Obj.writeCount          = 0;
    i2c1Obj.readCount           = 0;
    i2c1Obj.transferType        = I2C_TRANSFER_TYPE_WRITE;
    i2c1Obj.error               = I2C_ERROR_NONE;
    i2c1Obj.state               = I2C_STATE_ADDR_BYTE_1_SEND;

    I2C1CONSET                  = _I2C1CON_SEN_MASK;
    IEC0SET                     = _IEC0_I2C1MIE_MASK;
    IEC0SET                     = _IEC0_I2C1BIE_MASK;

    return true;
}

I2C_ERROR I2C1_ErrorGet(void)
{
    I2C_ERROR error;

    error = i2c1Obj.error;
    i2c1Obj.error = I2C_ERROR_NONE;

    return error;
}

bool I2C1_TransferSetup(I2C_TRANSFER_SETUP* setup, uint32_t srcClkFreq )
{
    uint32_t baudValue;
    uint32_t i2cClkSpeed;

    if (setup == NULL)
    {
        return false;
    }

    i2cClkSpeed = setup->clkSpeed;

    /* Maximum I2C clock speed cannot be greater than 1 MHz */
    if (i2cClkSpeed > 1000000)
    {
        return false;
    }

    if( srcClkFreq == 0)
    {
        srcClkFreq = 48000000UL;
    }

    baudValue = ((float)((float)srcClkFreq/2.0) * (1/(float)i2cClkSpeed - 0.000000130)) - 1;

    /* I2CxBRG value cannot be from 0 to 3 or more than the size of the baud rate register */
    if ((baudValue < 4) || (baudValue > 4095))
    {
        return false;
    }

    I2C1BRG = baudValue;

    /* Enable slew rate for 400 kHz clock speed; disable for all other speeds */

    if (i2cClkSpeed == 400000)
    {
        I2C1CONCLR = _I2C1CON_DISSLW_MASK;;
    }
    else
    {
        I2C1CONSET = _I2C1CON_DISSLW_MASK;;
    }

    return true;
}

static void I2C1_BUS_InterruptHandler(void)
{
    /* Clear the bus collision error status bit */
    I2C1STATCLR = _I2C1STAT_BCL_MASK;

    /* ACK the bus interrupt */
    IFS0CLR = _IFS0_I2C1BIF_MASK;

    i2c1Obj.state = I2C_STATE_IDLE;

    i2c1Obj.error = I2C_ERROR_BUS_COLLISION;

    if (i2c1Obj.callback != NULL)
    {
        i2c1Obj.callback(i2c1Obj.context);
    }
}

static void I2C1_MASTER_InterruptHandler(void)
{
    I2C1_TransferSM();
}

void I2C_1_InterruptHandler(void)
{
    if ((IFS0 & _IFS0_I2C1BIF_MASK) && (IEC0 & _IEC0_I2C1BIE_MASK))
    {
        I2C1_BUS_InterruptHandler();
    }
    else if ((IFS0 & _IFS0_I2C1MIF_MASK) && (IEC0 & _IEC0_I2C1MIE_MASK))
    {
        I2C1_MASTER_InterruptHandler();
    }
}