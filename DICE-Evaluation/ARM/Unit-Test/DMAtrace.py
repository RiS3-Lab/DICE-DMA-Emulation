#!/usr/bin/env python3

'''
   DICE - script to get the DMA traces summary for the complete unit test
   ------------------------------------------------------

   Copyright (C) 2019-2020 RiS3 Lab

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at:

     http://www.apache.org/licenses/LICENSE-2.0

'''

import sys, os
from os import listdir
from os.path import isfile, join
import subprocess


if len(sys.argv) == 3:

    mypath = sys.argv[1]
    outfile = sys.argv[2]
    
    files = [f for f in listdir(mypath) if isfile(join(mypath, f))]
    
    files.sort()
    
    if os.path.exists(outfile):
        os.remove(outfile)

        
    for file in files:
        print(file)
        command = ["grep", "DMA", mypath + "/" + file]
        with open(outfile,'a') as f:
            f.write("        \n")
            f.write("START **********  "+ file + "  *****************\n")
        with open(outfile,'a') as f:
            subprocess.call(command, stdout=f, stderr=f)
        with open(outfile,'a') as f:
            f.write("FINISH **********  "+ file + "  *****************\n")
            f.write("        \n")

else:
    print("Usage: %s <dma_trace_folder> <output_file>" % sys.argv[0])

