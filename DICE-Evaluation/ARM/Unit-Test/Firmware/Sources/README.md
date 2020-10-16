## P2IM unit test source code (Non-DMA-Enabled)
Please check the P2IM unit test website:

https://github.com/RiS3-Lab/p2im-unit_tests/tree/a6a29d5d02f702a72360e38d1ec65c92ff0e3bcc

## DICE unit test source code (DMA-Enabled)

To get and compile the source code, you have to download the corresponding SDK/OS or examples and agree with the vendor/developer's usage and distribution license.
The links to get source code are as follows:

|Firmware | MCU | OS/SDK |  Source Code   |
|---------|-----|--------|--------------  |      
|ADC PDC               |  SAM3x          |  Arduino              |  [here](./ADC_PDC.ino)  (the original website not working anymore) |     
|||||      
|SPI DMAC Slave        | SAM3x           |  Atmel ASF            |  https://www.microchip.com/mymicrochip/filehandler.aspx?ddocname=en1001769  |
|USART DMAC Example    |                 |                       |                                                                             |
|||||      
|ADC slider             |  STM32F103      |     ChibiOS           |  https://osdn.net/projects/chibios/downloads/70739/ChibiOS_19.1.0.7z/ |
|I2C accelerometer      |  STM32F103      |                       |                                                                       |
|SPI                    |  STM32F103      |                       |                                                                       |
|UART                   |  STM32F103      |                       |                                                                       |
|ADC slider             |  STM32F429      |                       |                                                                       |
|SPI                    |  STM32L152      |                       |                                                                       |
|UART                   |  STM32L152      |                       |                                                                       |   
||||| 
| ADC SW DMA           |  STM32F103      | STM32CubeF1           | https://www.st.com/en/embedded-software/stm32cubef1.html              |
| ADC Timer DMA         | |                                 |   |
| I2C DMA IT            | |                                 |   |
| I2C DMA Adv IT        | |                                 |   |
| I2C TxRx DMA         | |                                 |   |
| SPI Half DMA         | |                                 |   |
| SPI Half DMA Init     | |                                 |   |
| SPI Full DMA         | |                                 |   |
| USART TxRx DMA       | |                                 |   |
| USART Full DMA       | |                                 |   |
| UART H.Term. DMA      | |                                 |   |
| UART 2Boards DMA      | |                                 |   |
| SPI Full EX. DMA     | |                                 |   |
| I2C 2Boards DMA       | |                                 |   |
|||||     
| PDMA M-M                |  LPC1837              |  LPC Open             | https://www.nxp.com/downloads/en/software/lpcopen_3_02_lpcxpresso_mcb1857.zip |
| Serial console          |  NRF52832             |  Mynewt               | https://mynewt.apache.org/download/ |
|                         |  NRF51822             |                       |                     |
|||||     
| SPI slave              |  NRF51822             | Nordic SDK                     | https://www.nordicsemi.com/Software-and-tools/Software/nRF5-SDK/Download |
| SPI master             |  NRF52832             |                                |  |
| SPI slave              |  NRF52832             |                                |  |
| Serial DMA             |  NRF52832             |                                |  |
|||||     
| PDMA USART            | NUC123    | OpenNuvoton     | https://github.com/OpenNuvoton/NUC123BSP/tree/master/SampleCode/StdDriver/UART_PDMA |
| PDMA M-M              |           |                 | https://github.com/OpenNuvoton/NUC123BSP/tree/master/SampleCode/StdDriver/PDMA |


## NOTE:

The QEMU emulator used by P2IM does not support FPU for the Cortex-M. If the MCU that you are testing supports
FPU you must disable the hardware FPU and compile your code using the software implementation 
of the floating-point ABI (-mfloat-abi=soft). 



