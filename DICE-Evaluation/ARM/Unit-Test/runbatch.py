#!/usr/bin/env python3

'''
    DICE - Script to run the complete unit test for ARM Cortex-M
   ------------------------------------------------------

   Copyright (C) 2019-2020 RiS3 Lab

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at:

     http://www.apache.org/licenses/LICENSE-2.0

'''


from os import listdir
from os import getcwd
from os.path import isfile, join
import subprocess

firmpath="./firmware"
runpath = getcwd()+"/run.py"
out_path = "./revision2"


files = [f for f in listdir(firmpath) if isfile(join(firmpath, f))]

mcus={"F103":"f103", "F429": "f429", "L1521": "l1521", "LPC1837":"lpc18", \
    "NRF51822": "nrf52","NRF52832": "nrf52", "SAM3": "sam3", "NUC123":"nuc123", \
    "K64F": "k64f" }


for f in files:
    print(files)
    for key,mcu in mcus.items():
        if key in f:
            print("Processing: {}".format(f) )
            subprocess.call([runpath, mcu, firmpath +"/"+f, out_path])
           break
