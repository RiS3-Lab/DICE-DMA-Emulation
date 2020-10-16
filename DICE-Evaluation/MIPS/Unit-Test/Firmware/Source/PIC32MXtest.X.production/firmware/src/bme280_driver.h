/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    bme280_driver.h

  Summary:
    This header file provides prototypes and definitions for the BME280 sensor
    driver.

  Description:
    This header file provides function prototypes and data type definitions for
    the BLE280 sensor driver. The APIs that are needed by other task routines are
    defined here. Please refer to BME280 sensor data-sheet for more information.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END

#ifndef BME280_DRIVER_H
#define	BME280_DRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef uint8_t (*READ_REG)(uint8_t addr);
typedef bool (*WRITE_REG)(uint8_t addr, uint8_t data);
typedef bool (*READ)(uint8_t addr, uint8_t* const pBuffer, uint8_t nBytes);

typedef struct 
{
  uint16_t      dig_T1;
  int16_t       dig_T2;
  int16_t       dig_T3;
  uint16_t      dig_P1;
  int16_t       dig_P2;
  int16_t       dig_P3;
  int16_t       dig_P4;
  int16_t       dig_P5;
  int16_t       dig_P6;
  int16_t       dig_P7;
  int16_t       dig_P8;
  int16_t       dig_P9;
  uint8_t       dig_H1;
  int16_t       dig_H2;
  uint8_t       dig_H3;
  int16_t       dig_H4;
  int16_t       dig_H5;
  int8_t        dig_H6;
  
}BME280_CALIBRATION_PARAM;
    
    
typedef struct
{
    /* calibration parameters */
    BME280_CALIBRATION_PARAM    calib_val;
    /* raw ADC values */
    int32_t     adc_t;
    int32_t     adc_p;
    int32_t     adc_h;
    int32_t     t_fine;
    /* Driver related call-backs */    
    READ_REG    readRegister;
    WRITE_REG   writeRegister;
    READ        read;
    
}BME280_DRIVER_DATA;

/* Drive call-backs registration*/
void BME280_RegisterDrvReadReg(READ_REG   readReg);
void BME280_RegisterDrvWriteReg(WRITE_REG  writeReg);
void BME280_RegisterDrvRead(READ   read);

uint8_t BME280_ID_Get();
void BME280_SoftReset();
void BME280_ReadRawWeatherData();
void BME280_CalibParams_Get();
void BME280_SetOversampling(uint8_t param, uint8_t val);
void BME280_PowerMode_Set(uint8_t Value);
int16_t BME280_GetTempReading();
uint16_t BME280_GetPressReading();
float BME280_GetHumReading();

#ifdef	__cplusplus
}
#endif

#endif	/* BME280_DRIVER_H */

