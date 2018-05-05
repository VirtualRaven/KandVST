import csv
import numpy as np
import numpy.fft as fft
import matplotlib.pyplot as plt
import peakutils
import sys
sampleRate = 44100
if len(sys.argv) != 2:
    exit(0)
filePath = sys.argv[1]

def doThresholdTest(f):
    with open(f,'r') as cfile:
        dRead = csv.reader(cfile,delimiter=',')
        data = []
        for r in dRead:
            for e in r:
                if e !='':
                    data.append(float(e))
    i = 0
    for samp in data:
        if(float(samp) < -0.5 or float(samp) > 0.5) :
            print("Test faild! samp = "+str(samp))
            return False

    return True


if doThresholdTest(filePath+"//data_1.txt") and doThresholdTest(filePath+"//data_2.txt"):
    exit(42)
    
exit(1)
