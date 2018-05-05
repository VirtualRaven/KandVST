import csv
import numpy as np
import numpy.fft as fft
import matplotlib.pyplot as plt
import peakutils
import sys
sampleRate = 44100
exP = [int(220*np.power(2.0,1.0/12.0)),int(880*np.power(2.0,1.0/12.0))]
if len(sys.argv) != 2:
    exit(0)
filePath = sys.argv[1]

def doPeakTest(f,expP):
    with open(f,'r') as cfile:
        dRead = csv.reader(cfile,delimiter=',')
        data = []
        for r in dRead:
            for e in r:
                if e !='':
                    data.append(float(e))
    four = fft.fft(data, norm = 'ortho')
    freq = fft.fftfreq(len(data),d=1/sampleRate)
    freq = (np.where(freq>=0)[0]).tolist()
    f = []
    for c in four:
        f.append(np.linalg.norm(c))
    f=f[0:int(len(f)/2)]
    f =np.array(f)

    peaks =peakutils.indexes(np.array(f), thres=0.2, min_dist=5)
    rPeaks = []
    for p in peaks:
        rPeaks.append(freq[p])
    
    print("Peaks = " + str(rPeaks))

    if set(rPeaks)==set(expP):
        return True
    return False




if doPeakTest(filePath+"//data_1.txt",exP) and doPeakTest(filePath+"//data_2.txt",exP):
    exit(42)
    
exit(1)
