#ifndef MODBUS_RTU_H_INCLUDED
#define MODBUS_RTU_H_INCLUDED

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void modbusSlaveStartTread(void);
xTaskHandle modbusSlaveGetTaskHandle(void);
void * modbusSlaveGetGata(uint16_t address);
void modbusSlavePutData(uint16_t address, void * value, uint16_t len);


#endif /* MODBUS_RTU_H_INCLUDED */
