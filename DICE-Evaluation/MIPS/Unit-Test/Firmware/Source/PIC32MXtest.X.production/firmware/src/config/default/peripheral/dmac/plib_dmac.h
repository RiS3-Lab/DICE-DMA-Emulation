/*******************************************************************************
  DMAC Peripheral Library Interface Header File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_dmac.h

  Summary:
    DMAC peripheral library interface.

  Description:
    This file defines the interface to the DMAC peripheral library. This
    library provides access to and control of the DMAC controller.

  Remarks:
    None.

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

#ifndef PLIB_DMAC_H    // Guards against multiple inclusion
#define PLIB_DMAC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <device.h>
#include <string.h>
#include <stdbool.h>
#include <sys/kmem.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: type definitions
// *****************************************************************************
// *****************************************************************************

typedef enum
{
    /* Data was transferred successfully. */
    DMAC_ERROR_NONE = 1,

    /* DMA address error. */
    DMAC_ERROR_ADDRESS_ERROR = 2

} DMAC_ERROR;

typedef enum
{
    /* Data was transferred successfully. */
    DMAC_TRANSFER_EVENT_COMPLETE,

    /* Error while processing the request */
    DMAC_TRANSFER_EVENT_ERROR,

    /* No events yet. */
    DMAC_TRANSFER_EVENT_NONE

} DMAC_TRANSFER_EVENT;

typedef void (*DMAC_CHANNEL_CALLBACK) (DMAC_TRANSFER_EVENT status, uintptr_t contextHandle);

typedef struct
{
    bool inUse;

    /* Inidcates the error information for
       the last DMA operation on this channel */
    DMAC_ERROR errorInfo;

    /* Call back function for this DMA channel */
    DMAC_CHANNEL_CALLBACK  pEventCallBack;

    /* Client data(Event Context) that will be returned at callback */
    uintptr_t hClientArg;

} DMAC_CHANNEL_OBJECT;

typedef enum
{
    DMAC_CHANNEL_0 = 0x0,

    DMAC_CHANNEL_1 = 0x1,

    DMAC_CHANNEL_2 = 0x2,

    DMAC_CHANNEL_3 = 0x3,

    DMAC_CHANNEL_4 = 0x4,

    DMAC_CHANNEL_5 = 0x5,

    DMAC_CHANNEL_6 = 0x6,

    DMAC_CHANNEL_7 = 0x7,

    DMAC_NUMBER_OF_CHANNELS = 0x8

} DMAC_CHANNEL;

// *****************************************************************************
// *****************************************************************************
// Section: DMAC API
// *****************************************************************************
// *****************************************************************************

void DMAC_Initialize( void );

void DMAC_ChannelCallbackRegister(DMAC_CHANNEL channel, const DMAC_CHANNEL_CALLBACK eventHandler, const uintptr_t contextHandle );

bool DMAC_ChannelTransfer( DMAC_CHANNEL channel, const void *srcAddr, size_t srcSize, const void *destAddr, size_t destSize, size_t cellSize);

void DMAC_ChannelDisable(DMAC_CHANNEL channel);

bool DMAC_ChannelIsBusy(DMAC_CHANNEL channel);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif //PLIB_DMAC_H
