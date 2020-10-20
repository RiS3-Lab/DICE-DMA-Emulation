
# DICE: Automatic Emulation of DMA Input Channels for Dynamic Firmware Analysis

This is the repository of our paper DICE to appear at Proceedings of the 42nd IEEE Symposium on Security and Privacy (Oakland'21).


# Citing our paper

If your research find one or several components of DICE useful, please use the following citation:

```bibtex
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
DICE requires Ubuntu 16.04 or 18.04 64Bit LTS. We are aware of some issues with Ubuntu 20.04, therefore it is not supported.

DICE is based in many open source projects (check references in the paper), particularly it requires the [P<sup>2</sup>IM framework](https://github.com/RiS3-Lab/p2im) 
for Fuzz testing and benchmarks of the ARM Cortex-M.  Use the following commands to clone this repository and initialize all the required sub-modules:

```bash
git clone https://github.com/alejoseb/DICEscrathcpad.git
cd DICE-DMA-Emulation
git submodule update --init --recursive

```

## DICE Directory structure
After cloning and initializing all sub-modules the directory structure will be as follows:

```
.
├── DICE-Evaluation
│   ├── ARM                         
│   │   ├── DICE-P2IM-Utilities     # DICE-P2IM scripts for model instantiation 
│   │   ├── Fuzzing                 # Firmware binaries, source code and scripts for fuzz testing with DICE-P2IM
│   │   └── Unit-Test               # Firmware binaries, source code and scripts for unit test with DICE-P2IM
│   └── MIPS
│       └── Unit-Test               # Firmware binaries, source code and scripts for unit test with DICE-MIPS-emulator 
├── DICE-Patches                    # DICE patches (Add-ons) for P2IM and MIPS-emulator
├── DICE-precompiled
│   ├── ARM_bin                     # Precompiled QEMU-DICE-P2IM binaries with DMA input channels identification and emulation for the ARM Cortex-M
│   ├── ARM_bin_Disabled            # Precompiled QEMU-DICE-P2IM binaries with DMA input channels identification for the ARM Cortex-M
│   └── MIPS_bin                    # Precompiled QEMU-DICE-MIPS-emulator with DMA input channels identification for the MIPS M4K/M-class
├── mips-emulator                   # Vanilla MIPS emulator submodule
└── p2im                            # Vanilla P2IM framework submodule

```


## Applying DICE patch (Add-on) to QEMU P<sup>2</sup>IM
Compile QEMU using the source code in the "p2im" directory of DICE
and the instructions provided [here](https://github.com/RiS3-Lab/p2im/blob/master/docs/build_qemu.md).
Do not proceed if the compilation of vanilla QEMU P<sup>2</sup>IM fails. You must solve any dependencies
or compilation issues before proceeding. 

After compilation, from the root of the DICE directory structure, execute the following command ignoring any warning messages:

```bash
git apply ./DICE-Patches/DICE-P2IM.patch --unsafe-paths --directory ./p2im/qemu/src/qemu.git/

```

Compile QEMU again to finish the integration of DICE and QEMU P<sup>2</sup>IM.

The compiled QEMU binaries will be located at: 
`./p2im/qemu/src/install/debian64/qemu/bin`


## Compiling AFL 
From the root of the DICE directory execute the following commands:

```bash
cd p2im
make -C afl/

```

The compiled AFL binaries will be located at: 
`./p2im/afl`

##  Applying DICE patch (Add-on) to QEMU MIPS-emulator 
From the root of the DICE directory execute the following commands ignoring any warning messages:

```bash
git apply ./DICE-Patches/DICE-MIPS-EMULATOR.patch --unsafe-paths --directory ./mips-emulator
cd mips-emulator 
./configure --prefix=/usr/local/qemu-mips --target-list=mipsel-softmmu --disable-werror --disable-xen
make
```

# DICE: unit test of ARM Cortex-M firmware
We provide a helping [script](./DICE-Evaluation/ARM/Unit-Test/run.py) to automatically prepare the [configuration file](https://github.com/RiS3-Lab/p2im#preparing-the-configuration-file) required by P<sup>2</sup>IM.
This script launches DICE-P<sup>2</sup>IM to identify DMA input channels and instantiate peripheral models until firmware execution does not access new unknown interfaces. 

```bash
./run.py f103 ./Firmware/Binaries-DICE/F103_ADC_SingleConversion_TriggerTimer_DMA.elf ./output
```
To verify the automatically identified DMA input channels and the access to the DMA buffers of the
previous example, execute the following commands:

```bash
cd DICE-Evaluation/ARM/Unit-Test
 ./DMAtrace.py ./output/dma_trace ./dma.txt
```

The output file `dma.txt` contains the filtered execution trace for DMA operations of the firmware. 

For example:

```bash
DMA Stream configuration identified: *0x40020000 p_A:*0x40020010->*0x4001244c p_B:*0x40020014->*0x200000a4 

```

Where: 
`0x40020000` is the mapped base address of the DMA controller in the STM32F103 MCU

`p_A:*0x40020010->*0x4001244c` is a pointer to a register in a peripheral (Source), and

`p_B:*0x40020014->*0x200000a4` is a pointer to a buffer in RAM (Destinantion)



Besides this example, the filtered trace file contains more self-explanatory messages related to the DMA buffer size inference and data consumption through DMA input channels.

We also provide the [runbatch.py](DICE-Evaluation/ARM/Unit-Test/runbatch.py) script to execute the whole unit test as described on the DICE paper. This script might require several minutes or a few hours to complete.


# DICE: Fuzzing real-world ARM Cortex-M firmware
Fuzzing with DICE-P<sup>2</sup>IM follows the same [workflow](https://github.com/RiS3-Lab/p2im/blob/master/README.md#fuzzing) described for the P<sup>2</sup>IM framework. Therefore, the P<sup>2</sup>IM 
documentation is still relevant for the DICE-P<sup>2</sup>IM integration and you should check it for further details.
Besides the similarities, we provide in this repository a set of slightly modified scripts in a specific directory structure to easily reproduce the experiments presented in our paper.

To fuzz the real firmware follow these steps:

1. Create a base directory to store the fuzzing working files. We provide a handy [script](DICE-Evaluation/ARM/Fuzzing/CreateBaseDir.py) that will
do it for you. We recommend to create the base directory in the root of your home directory, otherwise
QEMU might complain about long paths and stop execution.

For example:

```bash
./DICE-Evaluation/ARM/Fuzzing/CreateBaseDir.py  -B ~/FuzzBase -R 1.0

```

This script creates a complete working tree and default seed input for the 7 tested firmware as follows:

```
FuzzBase
├── GPSReceiver
│   └── 1.0
│       ├── inputs
│       │   └── input.data
│       └── outputs
├── GuitarPedal
│   └── 1.0
│       ├── inputs
│       │   └── input.data
│       └── outputs
:
:
└── StepperMotor
    └── 1.0
        ├── inputs
        │   └── input.data
        └── outputs

```


2. Configure your Linux kernel according to AFL requirements.  Execute the following commands as root:

```bash
su -
echo core >/proc/sys/kernel/core_pattern
cd /sys/devices/system/cpu
echo performance | tee cpu*/cpufreq/scaling_governor
exit
```


3. Launch the fuzzing campaign using the provided configuration files and the previously created base directory.

For example:

```bash
cd ./DICE-Evaluation/ARM/Fuzzing
export FUZZDIR=/home/$USER/FuzzBase
./fuzz.py -c ./Configs/Modbus.cfg
```

4. Analyze the fuzzing results using the same [instructions](https://github.com/RiS3-Lab/p2im/blob/master/README.md#analyzing-fuzzing-results) provided for P<sup>2</sup>IM.



# DICE: Unit test for MIPS M4K/M-Class
Before launching the unit test you need to decompress the file system images of the BSD distribution

```bash
cd ./DICE-Evaluation/MIPS/Unit-Test/Unix/Filesystem
unzip ./filesystem.zip
cd ../../
```

To run the unit test of MIPS execute the [script](DICE-Evaluation/MIPS/Unit-Test/run.py) passing the corresponding configuration file.

For example:

```bash
cd ./DICE-Evaluation/MIPS/Unit-Test
./run.py -c ./Configs/runBSD-Lite-PIC32MZ.cfg -o ./dma.txt

```

This script will execute the QEMU DICE-Mips-emulator for 10 seconds and then will stop it automatically.  The output `dma.txt` file will contain similar traces as described for the ARM Cortex-M unit test. It is worth noting that some firmware are not DMA enabled and might not produce any DMA related trace, which is an expected result (no false positives).


# More documentation
Please refer to our [paper] (https://www.longlu.org/publication/dice/dice.pdf).

# Issues, questions and support
Please open a new issue in the GitHub repository including the details of your particular inquiry. 








 



