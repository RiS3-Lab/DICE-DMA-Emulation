#!/usr/bin/env python3

'''
   DICE - script to create a working base folder for fuzzing
   ------------------------------------------------------

   Copyright (C) 2019-2020 RiS3 Lab

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at:

     http://www.apache.org/licenses/LICENSE-2.0

'''

import subprocess,sys,os,json,logging,shutil,signal,time

import configparser
import argparse
from argparse import Namespace
from CreateSeedInput import Create

dirs =["GPSReceiver",
"GuitarPedal",
"MIDISynthetizer",
"Modbus",
"ModbusRedZones",
"Oscilloscope",
"SolderingStation",
"StepperMotor"]




if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Helper script to launch fuzzer")
    parser.add_argument("-R","--run-num", dest="run", default="1.0",
        help="fuzzing run number")
    parser.add_argument("-B", "--base", dest="base", default="FuzzDir",
        help="base dir for fuzzing ")

    args = parser.parse_args()

    base = args.base
    run_num = args.run

    os.chdir(os.getenv("HOME"))

    for d in dirs:
        os.makedirs(base + "/" + d + "/" + run_num + "/inputs")
        os.makedirs(base + "/" + d + "/" + run_num + "/outputs")
        Create(base + "/" + d + "/" + run_num + "/inputs/input.data")