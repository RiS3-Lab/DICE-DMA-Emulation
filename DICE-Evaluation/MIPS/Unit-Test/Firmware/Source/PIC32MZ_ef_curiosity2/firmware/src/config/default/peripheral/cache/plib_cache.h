/*******************************************************************************
  CACHE PLIB Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    plib_cache.h

  Summary:
    L1 CACHE PLIB external functions declarations

  Description:
    The CACHE PLIB supports the L1 CACHE found on PIC32MZ.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2019 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef PLIB_CACHE_H
#define PLIB_CACHE_H

// *****************************************************************************
// *****************************************************************************
// Section: Type definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* CACHE_COHERENCY enumeration

   Summary:
    L1 cache coherency settings.

   Description:
    This enumeration defines the supported system L1 cache coherency settings.

   Remarks:
    Used to read or write cache coherency policy for kseg0.
*/

typedef enum
{
    CACHE_WRITETHROUGH_NO_WRITEALLOCATE = 0,

    CACHE_WRITETHROUGH_WRITEALLOCATE = 1,

    CACHE_DISABLE = 2,

    CACHE_WRITEBACK_WRITEALLOCATE = 3

} CACHE_COHERENCY;

// *****************************************************************************
// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

/******************************** CACHE API ***********************************/

void CACHE_CacheInit(CACHE_COHERENCY cacheCoherency);

void CACHE_CacheFlush(void);

void CACHE_DataCacheFlush(void);

void CACHE_InstructionCacheFlush(void);

void CACHE_CacheClean(uint32_t addr, size_t len);

void CACHE_DataCacheClean(uint32_t addr, size_t len);

void CACHE_DataCacheInvalidate(uint32_t addr, size_t len);

void CACHE_InstructionCacheInvalidate(uint32_t addr, size_t len);

void CACHE_InstructionCacheLock(uint32_t addr, size_t len);

void CACHE_DataCacheLock(uint32_t addr, size_t len);

void CACHE_CacheSync(uint32_t addr, size_t len);

void CACHE_CacheCoherencySet(CACHE_COHERENCY cacheCoherency);

CACHE_COHERENCY CACHE_CacheCoherencyGet(void);

size_t CACHE_DataCacheAssociativityGet(void);

size_t CACHE_InstructionCacheAssociativityGet(void);

size_t CACHE_DataCacheLineSizeGet(void);

size_t CACHE_InstructionCacheLineSizeGet(void);

size_t CACHE_DataCacheLinesPerWayGet(void);

size_t CACHE_InstructionCacheLinesPerWayGet(void);

size_t CACHE_DataCacheSizeGet(void);

size_t CACHE_InstructionCacheSizeGet(void);

#endif //#ifndef PLIB_CACHE_H
