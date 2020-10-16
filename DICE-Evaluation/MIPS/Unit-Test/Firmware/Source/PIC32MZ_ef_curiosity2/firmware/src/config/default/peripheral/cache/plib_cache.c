/*******************************************************************************
  PLIB Cache Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    plib_cache_pic32mz.c

  Summary:
    Cache implementation.

  Description:
    The CACHE PLIB provies a simple interface to control Data and Instruction
    caches.
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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <xc.h>
#include <cp0defs.h>
#include "plib_cache_local.h"
#include "peripheral/cache/plib_cache.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Variable Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: SYS DEVCON Client Setup Routines
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Function:
    void CACHE_CacheInit(CACHE_COHERENCY cacheCoherency)

  Summary:
    Initializes the L1 cache.

  Description:
    Initializes both instruction and data caches. Invalidates all entries and
    zeroes all tags. Sets coherency attribute for kseg0.

  Remarks:
    Use with caution. Invalidates all cache lines without writing data
    back to memory. Any dirty data in the cache will be lost.
*/

void CACHE_CacheInit(CACHE_COHERENCY cacheCoherency)
{
    _pic32_init_cache(cacheCoherency);
}


// *****************************************************************************
/* Function:
    void CACHE_CacheFlush(void)

  Summary:
    Flushes the L1 cache.

  Description:
    Flushes both instruction and data caches. Invalidate entire instruction
    cache; write back and invalidate entire data cache.

  Remarks:
    Simplest way to sychnronize caches with memory, but not necessarily
    the most efficient.
*/

void CACHE_CacheFlush(void)
{
    _pic32_flush_cache();
}


// *****************************************************************************
/* Function:
    void CACHE_DataCacheFlush(void)

  Summary:
    Flushes the L1 data cache.

  Description:
    Write back and invalidate the entire data cache.

  Remarks:
    None.
*/

void CACHE_DataCacheFlush(void)
{
    _pic32_flush_dcache();
}


// *****************************************************************************
/* Function:
    void CACHE_InstructionCacheFlush(void)

  Summary:
    Flushes (invalidates) the L1 instruction cache.

  Description:
    Invalidate entire instruction cache.

  Remarks:
    None.
*/

void CACHE_InstructionCacheFlush(void)
{
    _pic32_flush_icache();
}


// *****************************************************************************
/* Function:
    void CACHE_CacheClean(uint32_t addr, size_t len)

  Summary:
    Write back and invalidate an address range in either cache.

  Description:
    Write back (data) and invalidate (data and address) an address range
    in either cache.

  Remarks:
    None.
*/

void CACHE_CacheClean(uint32_t addr, size_t len)
{
    _pic32_clean_cache(addr, len);
}


// *****************************************************************************
/* Function:
    void CACHE_DataCacheClean(uint32_t addr, size_t len)

  Summary:
    Write back and invalidate an address range in the data cache.

  Description:
    Write back and invalidate an address range in the data cache.

  Remarks:
    None.
*/

void CACHE_DataCacheClean(uint32_t addr, size_t len)
{
    _pic32_clean_dcache(addr, len);
}


// *****************************************************************************
/* Function:
    void CACHE_DataCacheInvalidate(uint32_t addr, size_t len)

  Summary:
    Invalidate an address range in the data cache.

  Description:
    Invalidate an address range in the data cache.

  Remarks:
    None.
*/

void CACHE_DataCacheInvalidate(uint32_t addr, size_t len)
{
    _pic32_clean_dcache_nowrite(addr, len);
}


// *****************************************************************************
/* Function:
    void CACHE_InstructionCacheInvalidate(uint32_t addr, size_t len)

  Summary:
    Invalidate an address range in the instruction cache.

  Description:
    Invalidate an address range in the instruction cache.

  Remarks:
    None.
*/

void CACHE_InstructionCacheInvalidate(uint32_t addr, size_t len)
{
    _pic32_clean_icache(addr, len);
}


// *****************************************************************************
/* Function:
    void CACHE_InstructionCacheLock(uint32_t addr, size_t len)

  Summary:
    Fetch and lock a block of instructions in the instruction cache.

  Description:
    Fetch and lock a block of instructions in the instruction cache.

  Remarks:
    Global flush functions will invalidate and unlock any cache lines
    locked with this function.
*/

void CACHE_InstructionCacheLock(uint32_t addr, size_t len)
{
    _pic32_lock_icache(addr, len);
}


// *****************************************************************************
/* Function:
    void CACHE_DataCacheLock(uint32_t addr, size_t len)

  Summary:
    Fetch and lock a block of data in the data cache.

  Description:
    Fetch and lock a block of data in the data cache.

  Remarks:
    Global flush functions will invalidate and unlock any cache lines
    locked with this function.
*/

void CACHE_DataCacheLock(uint32_t addr, size_t len)
{
    _pic32_lock_dcache(addr, len);
}


// *****************************************************************************
/* Function:
    void CACHE_CacheSync(uint32_t addr, size_t len)

  Summary:
    Synchronize the instruction and data caches.

  Description:
    Synchronize the instruction and data caches. Used when modifying
    the instruction stream (breakpoints, self-modifying code, relocating
    executable code to RAM). Flushes an address range from the data cache
    and invalidates that same range from the instruction cache.

  Remarks:
    None.
*/

void CACHE_CacheSync(uint32_t addr, size_t len)
{
    _pic32_sync_icache(addr, len);
}


// *****************************************************************************
/* Function:
    void CACHE_CacheCoherencySet(CACHE_COHERENCY cacheCoherency)

  Summary:
    Set the cache coherency attribute for kseg0.

  Description:
    Set the cache coherency attribute for kseg0.

  Remarks:
    Use with caution. May cause unpredictable behavior in a running
    system.
*/

void CACHE_CacheCoherencySet(CACHE_COHERENCY cacheCoherency)
{
    uint32_t cp0;
    /*LDRA_INSPECTED 496 S */ /*Deviation Reference: MH-4490*/
    cp0 = _mfc0(16, 0);
    cp0 &= ~0x07;
    cp0 |= cacheCoherency;
    /*LDRA_INSPECTED 496 S */ /*Deviation Reference: MH-4491*/
    _mtc0(16, 0, cp0);
}


// *****************************************************************************
/* Function:
    CACHE_COHERENCY CACHE_CacheCoherencyGet(void)

  Summary:
    Returns the current cache coherency attribute for kseg0.

  Description:
    Returns the current cache coherency attribute for kseg0.

  Remarks:
    None.
*/

CACHE_COHERENCY CACHE_CacheCoherencyGet(void)
{
    /*LDRA_INSPECTED 496 S */ /*Deviation Reference: MH-4492*/
    return _mfc0(16, 0) & 0x03;
}


// *****************************************************************************
/* Function:
    size_t CACHE_DataCacheAssociativityGet(void)

  Summary:
    Returns the number of ways in the data cache.

  Description:
    Returns the number of ways in the data cache.

  Remarks:
    None.
*/

size_t CACHE_DataCacheAssociativityGet(void)
{
    return _pic32_get_dcache_associativity();
}


// *****************************************************************************
/* Function:
    size_t CACHE_InstructionCacheAssociativityGet(void)

  Summary:
    Returns the number of ways in the instruction cache.

  Description:
    Returns the number of ways in the instruction cache.

  Remarks:
    None.
*/

size_t CACHE_InstructionCacheAssociativityGet(void)
{
    return _pic32_get_icache_associativity();
}


// *****************************************************************************
/* Function:
    size_t CACHE_DataCacheLineSizeGet(void)

  Summary:
    Returns the data cache line size.

  Description:
    Returns the data cache line size.

  Remarks:
    None.
*/

size_t CACHE_DataCacheLineSizeGet(void)
{
    return _pic32_get_dcache_linesize();
}


// *****************************************************************************
/* Function:
    size_t CACHE_InstructionCacheLineSizeGet(void)

  Summary:
    Returns the instruction cache line size.

  Description:
    Returns the instruction cache line size.

  Remarks:
    None.
*/

size_t CACHE_InstructionCacheLineSizeGet(void)
{
    return _pic32_get_icache_linesize();
}


// *****************************************************************************
/* Function:
    size_t CACHE_DataCacheLinesPerWayGet(void)

  Summary:
    Returns the number of lines per way in the data cache.

  Description:
    Returns the number of lines per way in the data cache.

  Remarks:
    None.
*/

size_t CACHE_DataCacheLinesPerWayGet(void)
{
    return _pic32_get_dcache_lines_per_way();
}


// *****************************************************************************
/* Function:
    size_t CACHE_InstructionCacheLinesPerWayGet(void)

  Summary:
    Returns the number of lines per way in the instruction cache.

  Description:
    Returns the number of lines per way in the instruction cache.

  Remarks:
    None.
*/

size_t CACHE_InstructionCacheLinesPerWayGet(void)
{
    return _pic32_get_icache_lines_per_way();
}


// *****************************************************************************
/* Function:
    size_t CACHE_DataCacheSizeGet(void)

  Summary:
    Returns the total number of bytes in the data cache.

  Description:
    Returns the total number of bytes in the data cache.

  Remarks:
    None.
*/

size_t CACHE_DataCacheSizeGet(void)
{
    return _pic32_get_dcache_size();
}


// *****************************************************************************
/* Function:
    size_t CACHE_InstructionCacheSizeGet(void)

  Summary:
    Returns the total number of bytes in the instruction cache.

  Description:
    Returns the total number of bytes in the instruction cache.

  Remarks:
    None.
*/

size_t CACHE_InstructionCacheSizeGet(void)
{
    return _pic32_get_icache_size();
}

/*******************************************************************************
 End of File
*/

