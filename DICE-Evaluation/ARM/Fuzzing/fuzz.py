#!/usr/bin/env python3

'''
   P2IM-DICE - script to orchestrate the fuzzing campaign
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

cfg_board = {"f103": "NUCLEO-F103RB", "f429": "STM32F429I-Discovery", "f303":"NUCLEO_F303RE", \
        "sam3": "Arduino-Due", "k64f": "FRDM-K64F", \
        "nuc123": "NUVOTON_nuc123" , "nrf52": "Nordic_NRF52832" , "lpc18": "NXP_LPC1837", \
        "l1521": "LISA_F105RC" }
cfg_mcu= {"f103": "STM32F103RB", "f429": "STM32F429ZI", "f303":  "STM32F303RE", "sam3": "SAM3X8E", \
        "k64f": "MK64FN1M0VLL12", "nuc123": "NUC123", "nrf52": "NRF52832", "lpc18": "LPC1837", \
        "l1521":"STM32F105RC"}

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

def read_config(cfg_f):
    if not os.path.isfile(cfg_f):
        sys.exit("Cannot find the specified configuration file: %s" % cfg_f)
    parser = configparser.SafeConfigParser(os.environ)
    parser.read(cfg_f)
    

    return Namespace(
        working_dir = os.path.abspath(parser.get("DEFAULT", "working_dir")),
        prog        = parser.get("DEFAULT", "program"),
        run         = parser.get("DEFAULT", "run"),
        afl_bin     = os.path.abspath(parser.get("afl", "bin")),
        afl_timeout = parser.get("afl", "timeout"),
        afl_seed    = os.path.abspath(parser.get("afl", "input")),
        afl_output  = os.path.abspath(parser.get("afl", "output")),
        qemu_bin    = os.path.abspath(parser.get("qemu", "bin")),
        mcu         = parser.get("program", "mcu"),
        img         = os.path.abspath(parser.get("program", "img")),
        log_f       = os.path.abspath(parser.get("model", "log_file")),
        me_bin      = os.path.abspath(parser.get("model", "bin")),
        redzones    = os.path.abspath(parser.get("qemu", "redzones")),
    )

def sigalarm_handler(signum, frame):
    global pid, killed
    killed = True
    os.kill(pid, signal.SIGTERM)
    os.kill(pid, signal.SIGKILL)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Helper script to launch fuzzer")
    parser.add_argument("--model-if", dest="model_if", default=None,
        help="optional peripheral model file")
    parser.add_argument("-c", "--config", dest="config", required=True,
        help="configuration file")
    
    parser.add_argument("--no-fuzzing", dest="no_fuzzing",
        action='store_true', help="don't run fuzzer")
    parser.add_argument("--no-skip-deterministic", dest="no_skip_deterministic",
        action='store_true', help="don't skip deterministic steps of afl")


    args = parser.parse_args()

    if args.config:
      args.config = os.path.abspath(args.config)

    if args.model_if and not os.path.isfile(args.model_if):
      sys.exit("model file provided with --model-if doesn't exist.")


    cfg = read_config(args.config)

    
    color_print("Change working dir to: %s" % cfg.working_dir, "blue")
    os.chdir(cfg.working_dir)
    print("CWD: %s\n" % os.getcwd())
    #shutil.copyfile(args.config, "fuzz.cfg")

    logging.basicConfig(filename=cfg.log_f, level=logging.INFO,
        format='[%(asctime)s] %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p')
    logging.info("fresh fuzzer run")

    #generate cfg file for model instantaion script
    cfg_f = "mi-%s.cfg" % cfg.mcu
 

    try:
       objdump_path =  subprocess.check_output(['which','arm-none-eabi-objdump']).decode()[:-1]
    except:
       sys.exit("arm-none-eabi-objdump not found, verify that the Cortex-M toolchain is installed and added to your PATH")

    
    with open(cfg_f, "w") as f:
        f.write("[qemu]\n")
        f.write("bin         = %s\n" % cfg.qemu_bin)
        f.write("log         = unimp,guest_errors,int\n\n")
        f.write("[program]\n")
        f.write("board       = %s\n" % cfg_board[cfg.mcu])
        f.write("mcu         = %s\n" % cfg_mcu[cfg.mcu])
        f.write("img         = %s\n\n" % cfg.img)
        f.write("[model]\n")
        f.write("retry_num   = 3\n")
        f.write("peri_addr_range = 512\n")
        f.write("objdump     = %s\n" % objdump_path)
        f.write("log_file    = me.log\n")

    # try our best to extract model w/o input
    color_print("try our best to extract model w/o input", "blue")
    run_num = "0"

    if os.path.isdir(run_num):
      color_print("model already extracted! No need to do it again")
    else:
      logging.info("run_num %s, access_to_unmodeled_peri: %s" % (run_num, {}))
      start_time = time.time()

      cmd_me0 = [cfg.me_bin, "-c", cfg_f, "--run-num", run_num,
        "--print-to-file"]
      if args.model_if:
        cmd_me0 += ["-model-input", args.model_if]
      print("cmd_me0: %s\n" % ' '.join(cmd_me0))
      subprocess.call(cmd_me0)

      logging.info("execution time: %f" % (time.time() - start_time))

    args.model_if = os.path.abspath("%s/peripheral_model.json" % run_num)
    print('')


    # extract model for each seed input
    color_print("extract model for each seed input", "blue")
    prev_run_num = run_num

    for seed0 in os.listdir(cfg.afl_seed):
      seed = "%s/%s" % (cfg.afl_seed, seed0)
      seed_run = 1

      while True: # may extract multiple rounds
        run_num = "%s.%s.%d" % (prev_run_num, seed0, seed_run)
        color_print(run_num)


        color_print("run f/w w/ seed input to check if there is aup")
        cmd_qemu = [cfg.qemu_bin, "-nographic", "-aflFile", seed,
          "-board", cfg_board[cfg.mcu], "-mcu", cfg_mcu[cfg.mcu], "-image", cfg.img,
          "-pm-stage", "3", "-model-input", args.model_if, 
          # options below are not used in no forkserver mode
          "-me-bin", cfg.me_bin, "-me-config", cfg_f]
        print("cmd_qemu: %s\n" % ' '.join(cmd_qemu))

        # set timeout for 1s
        signal.signal(signal.SIGALRM, sigalarm_handler)
        signal.alarm(1)

        with open(os.devnull, 'w') as devnull:
          proc = subprocess.Popen(cmd_qemu, stdout=devnull, stderr=devnull)

        global pid, killed
        pid = proc.pid
        killed = False
        proc.wait()
        if not killed:
          # clear timeout value
          signal.alarm(0)
        else:
          color_print("qemu hangs(pid: %d). seed input should not hang!" % pid, "red")
          # qemu killed in sig handler

        # check if there is aup
        if proc.returncode not in [0x40, 0x41]:
          color_print("No aup, don't run ME")
          break


        color_print("There is aup, run ME")
        cmd_me = [cfg.me_bin, "-c", cfg_f, "--run-num", run_num,
          "--print-to-file", "--run-from-forkserver", "--afl-file", seed, 
          "--model-if", args.model_if]
        print("cmd_me: %s" % ' '.join(cmd_me))
        subprocess.call(cmd_me)

        args.model_if = os.path.abspath("%s/peripheral_model.json" % run_num)
        seed_run += 1
        print('')
    print('')


    # launch fuzzer
    color_print("launch fuzzer", "blue")

    cmd_afl = [cfg.afl_bin, "-i", cfg.afl_seed, "-o", cfg.afl_output, 
        "-t", cfg.afl_timeout, "-QQ", 
        # used by only non forkserver mode
        # AFL passes model_if to qemu, so we don't pass it here
        "-a", cfg.me_bin, "-b", cfg_f, "-c", args.model_if, 
        "-T", "%s_%s" % (cfg.prog, cfg.run)]
    if not args.no_skip_deterministic:
        # skip deterministic stage
        cmd_afl += ["-d"]
    # end of afl options 

    if 'none' in cfg.redzones:
       cmd_afl += [cfg.qemu_bin, "-nographic",
              "-board",  cfg_board[cfg.mcu] , "-mcu", cfg_mcu[cfg.mcu], "-image", cfg.img,
              "-pm-stage", "3", "-aflFile", "@@", 
          ]
       
    else:
       cmd_afl += [cfg.qemu_bin, "-nographic",
            "-board",  cfg_board[cfg.mcu] , "-mcu",  cfg_mcu[cfg.mcu], "-image", cfg.img, "--dmaFile", cfg.redzones,
            "-pm-stage", "3", "-aflFile", "@@", 
         
        ]
       


    print("cmd_afl: %s\n" % ' '.join(cmd_afl))

    if not args.no_fuzzing:
      subprocess.call(cmd_afl, env=dict(os.environ, AFL_NO_FORKSRV=''))
