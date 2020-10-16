/*******************************************************************************
  Inter-Integrated Circuit (I2C) Library
  Instance Header File

  Company
    Microchip Technology Inc.

  File Name
    plib_i2c_master.h

  Summary
    I2C peripheral library interface.

  Description
    This file defines the interface to the I2C peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:

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

#ifndef PLIB_I2C_MASTER_H
#define PLIB_I2C_MASTER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* I2C Transfer Status

   Summary:
    I2C Transfer Status data type.

   Description:
    This data type defines the I2C Transfer Status.

   Remarks:
    None.
*/

typedef enum
{
    /* No Error */
    I2C_ERROR_NONE,

    /* Slave returned Nack */
    I2C_ERROR_NACK,

    /* Bus Collision Error */
    I2C_ERROR_BUS_COLLISION,

} I2C_ERROR;

// *****************************************************************************
/* I2C Transfer Type

   Summary:
    I2C Transfer Type.

   Description:
    This data type defines the I2C Transfer Type.

   Remarks:
    None.
*/
typedef enum
{
    I2C_TRANSFER_TYPE_WRITE = 0,

    I2C_TRANSFER_TYPE_READ

}I2C_TRANSFER_TYPE;

// *****************************************************************************
/* I2C State.

   Summary:
    I2C PLib Task State.

   Description:
    This data type defines the I2C PLib Task State.

   Remarks:
    None.

*/

typedef enum
{
    I2C_STATE_START_CONDITION,

    I2C_STATE_ADDR_BYTE_1_SEND,

    I2C_STATE_ADDR_BYTE_2_SEND,

    I2C_STATE_READ_10BIT_MODE,

    I2C_STATE_ADDR_BYTE_1_SEND_10BIT_ONLY,

    I2C_STATE_WRITE,

    I2C_STATE_READ,

    I2C_STATE_READ_BYTE,

    I2C_STATE_WAIT_ACK_COMPLETE,

    I2C_STATE_WAIT_STOP_CONDITION_COMPLETE,

    I2C_STATE_IDLE,

} I2C_STATE;

// *****************************************************************************
/* I2C Callback

   Summary:
    I2C Callback Function Pointer.

   Description:
    This data type defines the I2C Callback Function Pointer.

   Remarks:
    None.
*/

typedef void (*I2C_CALLBACK) (uintptr_t contextHandle);

// *****************************************************************************
/* I2C PLib Instance Object

   Summary:
    I2C PLib Object structure.

   Description:
    This data structure defines the I2C PLib Instance Object.

   Remarks:
    None.
*/

typedef struct
{
    uint16_t                address;
    uint8_t*                writeBuffer;
    uint8_t*                readBuffer;
    size_t                  writeSize;
    size_t                  readSize;
    size_t                  writeCount;
    size_t                  readCount;
    bool                    forcedWrite;
    I2C_TRANSFER_TYPE       transferType;
    I2C_STATE               state;
    I2C_ERROR               error;
    I2C_CALLBACK            callback;
    uintptr_t               context;

} I2C_OBJ;

// *****************************************************************************
/* I2C Transfer Setup Data Structure

   Summary:
    I2C Transfer Setup Data Structure

   Description:
    This data structure defines the I2C Transfer Setup Data

   Remarks:
    None.
*/

typedef struct
{
    /* I2C Clock Speed */
    uint32_t clkSpeed;

} I2C_TRANSFER_SETUP;

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
// DOM-IGNORE-END

#endif /* PLIB_I2C_MASTER_H */














