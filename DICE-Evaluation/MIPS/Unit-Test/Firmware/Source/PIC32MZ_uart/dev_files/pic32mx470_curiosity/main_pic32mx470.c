/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include <stdint.h>
#include "definitions.h"                // SYS function prototypes
#include "bme280_definitions.h"
#include "bme280_driver.h"

/* Timer Counter Time period match values for input clock of 48 MHz */
#define PERIOD_500MS                            24000000U
#define PERIOD_1S                               48000000U
#define PERIOD_2S                               96000000U
#define PERIOD_4S                               192000000U

#define SWITCH_PRESSED_STATE                    0   // Active LOW switch

#define MAX_I2C_TX_BUFFER_SIZE                  100
#define MAX_I2C_RX_BUFFER_SIZE                  100

typedef struct
{
    volatile bool                   isBufferCompleteEvent;
    uint8_t                         slaveID;
    int16_t                         temperature;
    uint8_t                         txBuffer[MAX_I2C_TX_BUFFER_SIZE];
    uint8_t                         rxBuffer[MAX_I2C_RX_BUFFER_SIZE];
}BME_SENSOR_DATA;    
static BME_SENSOR_DATA BME280SensorData;

typedef enum
{
    TEMP_SAMPLING_RATE_500MS = 0,
    TEMP_SAMPLING_RATE_1S = 1,
    TEMP_SAMPLING_RATE_2S = 2,
    TEMP_SAMPLING_RATE_4S = 3,
} TEMP_SAMPLING_RATE;
static TEMP_SAMPLING_RATE tempSampleRate = TEMP_SAMPLING_RATE_500MS;

static volatile bool isTmr2Expired = false;
static volatile bool changeTempSamplingRate = false;
static volatile bool isUARTTxComplete = false;
static uint16_t      temperatureVal;
static uint8_t __attribute__ ((aligned (16))) uartTxBuffer[100] = {0};





// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );


    /* Start the timer 2 */

    while ( true )
    {
        if (true == isTmr2Expired)
        {

            /* The getting temperature is in DegC and resolution is 0.01 DegC. 
             * It means, if the output value is 5123 equals to 51.23 DegC */
            if(changeTempSamplingRate == false)
            {
                /* Celcius to Fahrenheit (°C to °F) Conversion (°F = (°C × 9/5) + 32) */
				
            }
            else
            {
				
            }
        }
    }
    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

