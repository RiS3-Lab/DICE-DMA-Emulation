To compile the code you have to download MPLAB-X IDE and the corresponding  MPLAB XC compiler from Microchip's website.

https://www.microchip.com/mplab/mplab-x-ide

https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers


## Note:
The QEMU Mips emulator does not support FPU. You have to disable the FPU core feature of the PIC32MZ before compiling the code.
For the version and standard installation of the MPLAB XC, the changes are done on line #70958 of the following file:

/opt/microchip/mplabx/v5.30/packs/Microchip/PIC32MZ-EF_DFP/1.1.45/include/proc/p32mz2048efm100.h
