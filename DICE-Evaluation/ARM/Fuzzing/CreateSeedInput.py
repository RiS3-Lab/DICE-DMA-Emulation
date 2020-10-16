#!/usr/bin/python3


import struct
import random
import sys



def Create(file):
    fout = open(file, 'wb')
    #random data
    # #for x in range(10000):
    # #   a=random.randint(1,254)
    # #   fout.write(struct.pack('B',a))
    # #pattern data
  
    for x in range(10000):
        fout.write(struct.pack('I',0xefbeadde))
    fout.close()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit("usage: " + sys.argv[0] + "  <file>" )
    Create(sys.argv[1])