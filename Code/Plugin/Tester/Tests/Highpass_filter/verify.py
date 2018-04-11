import csv
import numpy as np
import numpy.fft as fft
import matplotlib.pyplot as plt
import peakutils
import sys
import math
sampleRate = 44100
exP = 2
thr = 0.1
if len(sys.argv) != 2:
    exit(0)
filePath = sys.argv[1]
def findFreqIndex(frq,flist):
    c =0
    for f in flist:
        if f == frq:
            return c
        c+=1
    return -1


def doPeakTest(f):
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

    v1 = findFreqIndex(12544,freq)
    v2 = findFreqIndex(440,freq)
    v1 = f[v1]
    v2 = f[v2]

    
    db = 20*math.log10(v2/v1)
    diff = abs(20*math.log10(1/math.sqrt(2)))-abs(db)

    print ("v1: " + str(20*math.log10(v1)) + " dB")
    print ("v2: " + str(20*math.log10(v2)) + " dB")
    print (str(db)+" db")
    print ("Diff from target: " + str(diff)+" db")
    
    
    #plt.plot(f)
    #plt.show()
    peaks =peakutils.indexes(np.array(f), thres=0.2, min_dist=5)
    rPeaks = []
    for p in peaks:
        rPeaks.append(freq[p])

		
    print("Found: " + str(len(rPeaks)))
    if len(rPeaks)==exP and (diff<thr):
        return True
    return False




if doPeakTest(filePath+"//data_1.txt") and doPeakTest(filePath+"//data_2.txt"):
    exit(42)
    
exit(1)
