/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    bme280_driver.c

  Summary:
    This file contains the source code for BME280 driver.

  Description:
    This file implements the BME280 driver functionality. It provides APIs to 
    access the BME280 sensor.
    Please refer to the BME280 data-sheet for more information.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#include "bme280_definitions.h"
#include "bme280_driver.h"

static BME280_DRIVER_DATA  BME280DriverData;

void BME280_RegisterDrvReadReg(READ_REG drvReadReg)
{
    if (drvReadReg)
    {
        BME280DriverData.readRegister = drvReadReg;
    }
}
void BME280_RegisterDrvWriteReg(WRITE_REG drvWriteReg)
{
    if (drvWriteReg)
    {
        BME280DriverData.writeRegister = drvWriteReg;
    }
}
void BME280_RegisterDrvRead(READ drvRead)
{
    if (drvRead)
    {
        BME280DriverData.read = drvRead;
    }
}

void BME280_SoftReset() 
{
  BME280DriverData.writeRegister(BME280_REG_RESET, BME280_SOFT_RESET);
}

uint8_t BME280_ID_Get() 
{
    return BME280DriverData.readRegister(BME280_REG_CHIP_ID);
}

void BME280_ReadRawWeatherData() 
{  
    uint8_t readBuffer[8] = {0};
    
    if (true == BME280DriverData.read(BME280_REG_PRESSURE_MSB, readBuffer, 8))
    {                           
        BME280DriverData.adc_p  = (unsigned long)readBuffer[2] >> 4;
        BME280DriverData.adc_p |= (unsigned long)readBuffer[1] << 4;
        BME280DriverData.adc_p |= (unsigned long)readBuffer[0] << 12;   
        
        BME280DriverData.adc_t  = (unsigned long)readBuffer[5] >> 4;
        BME280DriverData.adc_t |= (unsigned long)readBuffer[4] << 4;
        BME280DriverData.adc_t |= (unsigned long)readBuffer[3] << 12;
        
        BME280DriverData.adc_h = readBuffer[7];
        BME280DriverData.adc_h |= (unsigned long)readBuffer[6] << 8;                  
    }
}

void BME280_CalibParams_Get() 
{    
    BME280DriverData.calib_val.dig_T1 = ((uint16_t)BME280DriverData.readRegister(BME280_CALIB_TEMP_DIG_T1_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_T1 |= BME280DriverData.readRegister(BME280_CALIB_TEMP_DIG_T1_LSB_REG);    

    BME280DriverData.calib_val.dig_T2 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_TEMP_DIG_T2_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_T2 |= BME280DriverData.readRegister(BME280_CALIB_TEMP_DIG_T2_LSB_REG);    

    BME280DriverData.calib_val.dig_T3 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_TEMP_DIG_T3_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_T3 |= BME280DriverData.readRegister(BME280_CALIB_TEMP_DIG_T3_LSB_REG);    

    BME280DriverData.calib_val.dig_P1 = ((uint16_t)BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P1_MSB_REG) << 8);     
    BME280DriverData.calib_val.dig_P1 |= BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P1_LSB_REG);    

    BME280DriverData.calib_val.dig_P2 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P2_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_P2 |= BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P2_LSB_REG);    

    BME280DriverData.calib_val.dig_P3 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P3_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_P3 |= BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P3_LSB_REG);    

    BME280DriverData.calib_val.dig_P4 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P4_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_P4 |= BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P4_LSB_REG);    

    BME280DriverData.calib_val.dig_P5 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P5_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_P5 |= BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P5_LSB_REG);    

    BME280DriverData.calib_val.dig_P6 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P6_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_P6 |= BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P6_LSB_REG);    

    BME280DriverData.calib_val.dig_P7 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P7_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_P7 |= BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P7_LSB_REG);
    
    BME280DriverData.calib_val.dig_P8 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P8_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_P8 |= BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P8_LSB_REG);
   
    BME280DriverData.calib_val.dig_P9 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P9_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_P9 |= BME280DriverData.readRegister(BME280_CALIB_PRESS_DIG_P9_LSB_REG);    

    BME280DriverData.calib_val.dig_H1 = BME280DriverData.readRegister(BME280_CALIB_HUM_DIG_H1_REG);
    
    BME280DriverData.calib_val.dig_H2 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_HUM_DIG_H2_MSB_REG) << 8);    
    BME280DriverData.calib_val.dig_H2 |= BME280DriverData.readRegister(BME280_CALIB_HUM_DIG_H2_LSB_REG);    

    BME280DriverData.calib_val.dig_H3 = BME280DriverData.readRegister(BME280_CALIB_HUM_DIG_H3_REG);
    
    BME280DriverData.calib_val.dig_H4 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_HUM_DIG_H4_MSB_REG) << 4);    
    BME280DriverData.calib_val.dig_H4 |= (BME280DriverData.readRegister(BME280_CALIB_HUM_DIG_H4_LSB_REG) & 0x0F);    

    BME280DriverData.calib_val.dig_H5 = ((int16_t)BME280DriverData.readRegister(BME280_CALIB_HUM_DIG_H5_MSB_REG) << 4);
    BME280DriverData.calib_val.dig_H5 |= (BME280DriverData.readRegister(BME280_CALIB_HUM_DIG_H4_LSB_REG) >> 4); 

    BME280DriverData.calib_val.dig_H6 = BME280DriverData.readRegister(BME280_CALIB_HUM_DIG_H6_REG);     
}

void BME280_SetOversampling(uint8_t param, uint8_t val)
{
    uint8_t control_meas = 0;
    
    switch(param)
    {
        case BME280_PARAM_TEMP:
            control_meas = BME280DriverData.readRegister(BME280_REG_CTRL_MEAS);
            control_meas &= ~0xE0;
            control_meas |= val << 5;
            BME280DriverData.writeRegister(BME280_REG_CTRL_MEAS, control_meas);
            break;
        case BME280_PARAM_PRESS:
            control_meas = BME280DriverData.readRegister(BME280_REG_CTRL_MEAS);
            control_meas &= ~0x1C;
            control_meas |= val << 2;
            BME280DriverData.writeRegister(BME280_REG_CTRL_MEAS, control_meas);
            break;
        case BME280_PARAM_HUM:
            BME280DriverData.writeRegister(BME280_REG_CTRL_HUMIDITY, val );
            break;
        default:
            break;
    }
    
}

void BME280_PowerMode_Set(uint8_t val) 
{
    uint8_t control_meas = 0;
    control_meas = BME280DriverData.readRegister(BME280_REG_CTRL_MEAS);
    control_meas |= val;

    BME280DriverData.writeRegister(BME280_REG_CTRL_MEAS, control_meas);
}

static int32_t BME280_Compensate_T()
{
    int32_t var1, var2, T;

    var1 = ((((BME280DriverData.adc_t>>3) -((int32_t)BME280DriverData.calib_val.dig_T1<<1))) * ((int32_t)BME280DriverData.calib_val.dig_T2)) >> 11;
    var2 = (((((BME280DriverData.adc_t>>4) - ((int32_t)BME280DriverData.calib_val.dig_T1)) * ((BME280DriverData.adc_t>>4) - ((int32_t)BME280DriverData.calib_val.dig_T1))) >> 12) * ((int32_t)BME280DriverData.calib_val.dig_T3)) >> 14;
    BME280DriverData.t_fine = var1 + var2;
    T = (BME280DriverData.t_fine * 5 + 128) >> 8;
    return T;
}

static uint32_t BME280_Compensate_P()
{
  int64_t var1, var2, p;
  
  var1 = ((int64_t)BME280DriverData.t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)BME280DriverData.calib_val.dig_P6;
  var2 = var2 + ((var1*(int64_t)BME280DriverData.calib_val.dig_P5)<<17);
  var2 = var2 + (((int64_t)BME280DriverData.calib_val.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)BME280DriverData.calib_val.dig_P3)>>8) + ((var1 * (int64_t)BME280DriverData.calib_val.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)BME280DriverData.calib_val.dig_P1)>>33;
  if(var1 == 0)
  {
        return 0;
  }
  p = 1048576 - BME280DriverData.adc_p;
  p = (((p<<31) - var2)*3125)/var1;
  var1 = (((int64_t)BME280DriverData.calib_val.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)BME280DriverData.calib_val.dig_P8) * p)>> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)BME280DriverData.calib_val.dig_P7)<<4);
  return (uint32_t)p/256;
}

static uint32_t BME280_Compensate_H() 
{
    int32_t h1;
    h1 = (BME280DriverData.t_fine - ((int32_t)76800));
    h1 = (((((BME280DriverData.adc_h << 14) - (((int32_t)BME280DriverData.calib_val.dig_H4) << 20) - (((int32_t)BME280DriverData.calib_val.dig_H5) * h1)) +
      ((int32_t)16384)) >> 15) * (((((((h1 * ((int32_t)BME280DriverData.calib_val.dig_H6)) >> 10) * (((h1 *
      ((int32_t)BME280DriverData.calib_val.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
      ((int32_t)BME280DriverData.calib_val.dig_H2) + 8192) >> 14));
    h1 = (h1 - (((((h1 >> 15) * (h1 >> 15)) >> 7) * ((int32_t)BME280DriverData.calib_val.dig_H1)) >> 4));
    h1 = (h1 < 0 ? 0 : h1);
    h1 = (h1 > 419430400 ? 419430400 : h1);
    return (uint32_t)(h1>>12);
}

int16_t BME280_GetTempReading() 
{
    return (int16_t)BME280_Compensate_T();
}

uint16_t BME280_GetPressReading() 
{
    return (uint16_t) (BME280_Compensate_P() / 100);    
}

float BME280_GetHumReading() 
{
    return (float)BME280_Compensate_H() / 1024;
}



