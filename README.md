
# DICE: Automatic Emulation of DMA Input Channels for Dynamic Firmware Analysis

This the repository of our paper DICE to appear at Proceedings of the 42nd IEEE Symposium on Security and Privacy (Oakland'21).


If your research find one or several components of DICE useful, please cite the following paper:

```
@inproceedings{dice,
 abstract = {Microcontroller-based embedded devices are at the core of Internet-of-Things (IoT) and Cyber-Physical Systems (CPS). The security of these devices is of paramount importance. Among the approaches to securing embedded devices, dynamic firmware analysis (e.g., vulnerability detection) gained great attention lately, thanks to its offline nature and low false- positive rates. However, regardless of the analysis and emulation techniques used, existing dynamic firmware analyzers share a major limitation, namely the inability to handle firmware using DMA (Direct Memory Access). It severely limits the types of devices supported and firmware code coverage.
We present DICE, a drop-in solution for firmware analyzers to emulate DMA input channels and generate or manipulate DMA inputs (from peripherals to firmware). DICE is designed to be hardware-independent (i.e., no actual peripherals or DMA controllers needed) and compatible with common MCU firmware (i.e., no firmware-specific DMA usages assumed) and embedded architectures. The high-level idea behind DICE is the identi- fication and emulation of the abstract DMA input channels, rather than the highly diverse peripherals and controllers. DICE identifies DMA input channels as the firmware writes the source and destination DMA transfer pointers into the DMA controller. Then DICE manipulates the input transferred through DMA on behalf of the firmware analyzer. DICE does not require firmware source code or additional features from firmware analyzers. We integrated DICE to the recently proposed firmware an- alyzer P2IM (for ARM Cortex-M architecture) and a PIC32 emulator (for MIPS M4K/M-Class architecture). We evaluated it on 83 benchmarks and sample firmware, representing 9 different DMA controllers from 5 different vendors. DICE detected 33 out of 37 DMA input channels, with 0 false positives. It correctly supplied DMA inputs to 21 out of 22 DMA buffers that firmware actually use, which previous firmware analyzers cannot achieve due to the lack of DMA emulation. DICE’s overhead is fairly low, it adds 3.4% on average to P2IM execution time. We also fuzz-tested 7 real-world firmware using DICE and compared the results with the original P2IM. DICE uncovered tremendously more execution paths (as much as 79X) and found 5 unique previously-unknown bugs that are unreachable without DMA emulation.},
 author = {Mera, Alejandro and Feng, Bo and Lu, Long and Kirda, Engin and Robertson, William},
 booktitle = {Proceedings of the 42nd IEEE Symposium on Security and Privacy},
 month = {May},
 series = {S&P/Oakland'21},
 title = {DICE: Automatic Emulation of DMA Input Channels for Dynamic Firmware Analysis},
 year = {2021}
}

```

# Getting started
DICE has been tested in Ubuntu 16.04 and 18.04 64Bit LTS. We are aware of some issues with Ubuntu 20.04, therefore it is not supported.

DICE requires some other projects, please use the following command to clone this repository and initialize the required sub-modules:

```



```




