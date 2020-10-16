#!/usr/bin/env python3
'''
   DICE - script to execute a unit test of DICE with a MIPS emulator
   ------------------------------------------------------

   Copyright (C) 2019-2020 RiS3 Lab

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at:

     http://www.apache.org/licenses/LICENSE-2.0

'''


import configparser
import argparse
from argparse import Namespace
from subprocess import STDOUT, check_output

import subprocess,sys,os,re,signal,atexit,shutil,logging,time,csv
import struct


def color_print(s, color="green"):
    if color == "green":
        print("\033[92m%s\033[0m" % s)
    elif color == "blue":
        print("\033[94m%s\033[0m" % s)
    elif color == "yellow":
        print("\033[93m%s\033[0m" % s)
    elif color == "red":
        print("\033[91m%s\033[0m" % s)
    else:
        print(s)


def sig_handler(signo, stack_frame):
    global oldstdout
    global oldstderr

    subprocess.call(["killall", cfg.qemu_bin])
    sys.stdout = oldstdout
    sys.stderr =oldstderr
    sys.exit("execution finished!")
    
    # fix terminal after killing the emulator
    cmd = ["stty", "sane"]
    subprocess.call(cmd)

    cmd = ["tput", "rs1"]
    subprocess.call(cmd)


def read_config(cfg_f):
  
    
    if not os.path.isfile(cfg_f):
        sys.exit("Cannot find the specified configuration file: %s" % cfg_f)
    parser = configparser.SafeConfigParser()
    parser.read(cfg_f)
    
    return Namespace(
        qemu_bin    = os.path.abspath(parser.get("qemu", "bin")),
        machine     = parser.get("test", "machine"),
        monitor     = parser.get("test", "monitor"),
        serial      = parser.get("test", "serial").split(','),
        bios        = os.path.abspath(parser.get("test", "bios")),
        kernel      = os.path.abspath(parser.get("test", "kernel")),
        sd          = os.path.abspath(parser.get("test", "sd")),
    )


if __name__ == "__main__":
    global  oldstdout
    oldstdout = sys.stdout
    global  oldstderr
    oldstderr = sys.stderr

    start_time = time.time()

    parser = argparse.ArgumentParser(description="MIPS emulator unit tester")
    parser.add_argument("-c", "--config", dest="config", required=True, help="configuration file for unit test")
    parser.add_argument("-o", "--output", dest="out", default="./out.txt", help="output file")
    args = parser.parse_args()
    
    try:
        cfg = read_config(args.config)
    except:
        sys.exit("error parsing config file")
    
    signal.signal(signal.SIGALRM , sig_handler)

    cmd = []
    cmd.append(cfg.qemu_bin)
    cmd.extend(['-machine',cfg.machine])
    cmd.append("-nographic")
    cmd.extend(["-monitor", cfg.monitor])
    
    for s in cfg.serial:
        if not  'none' in s :
            cmd.extend(["-serial", s])
    
    if not 'none' in cfg.bios :
        cmd.extend(["-bios", cfg.bios])
    
    if not 'none' in cfg.kernel :
        cmd.extend(["-kernel", cfg.kernel])
    
    if not 'none' in cfg.sd:
        print(type(cfg.sd))
        print(cfg.sd)
        cmd.extend(["-sd", cfg.sd])

 
    color_print( 'Executing: ' + ' '.join(cmd))

    try:

        with open(args.out, "w") as f:
            subprocess.call(cmd, stdout=f, stderr=f, timeout= 10)
    except:
        cmd = ["stty", "sane"]
        subprocess.call(cmd)
        cmd = ["tput", "rs1"]
        subprocess.call(cmd)
        print("execution finished!")

   

 