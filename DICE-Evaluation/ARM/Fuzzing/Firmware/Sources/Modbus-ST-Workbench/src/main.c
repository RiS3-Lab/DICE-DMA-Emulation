/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f3xx.h"
#include "stm32f3xx_nucleo.h"
#include "afl_call.h"

/*
int main(void)
{

	for(;;);
}*/

#include "FreeRTOS.h"
#include "task.h"

#include "modbus_rtu.h"

int main(void)
{
    SystemInit();
    startForkserver(0);
    modbusSlaveStartTread();
    vTaskStartScheduler();
    return 0;
}
