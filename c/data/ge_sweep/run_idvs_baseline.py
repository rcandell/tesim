# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import subprocess 
import numpy as np
import matplotlib as mp

EXE = "C:/git/tesim/c/Debug/tesim.exe"
fh = open("NUL","w")

# first run the baseline simulations for all disturbance cases
DURATION = 72
TS = 0.0005
TC = 0.0005
TSV = 1
PREFIX = "lossless"
IDV_VEC = range(1, 16)
# no disturbance
call_str = [EXE, 
            "-s", str(DURATION),
            "-t", str(TS),
            "-c", str(TC),
            "-k", str(TSV),
            "-p", PREFIX]
print(call_str)            
#call(call_str)
subprocess.Popen(call_str, stdout = fh, stderr = fh)

# all disturbances individually
for IDV in IDV_VEC:
    call_str = [EXE, 
                "-s", str(DURATION),
                "-t", str(TS),
                "-c", str(TC),
                "-k", str(TSV),
                "-p", PREFIX + "_IDV_" + str(IDV),
                "-i", str(IDV)]
    print(call_str)
    #subprocess.call(call_str)
    subprocess.Popen(call_str, stdout = fh, stderr = fh)

# now run the GE channel over steady state
# the P vector
#N = 20;
#B = 3
#p = B**(1-np.logspace(0.1, 0.9, N,base=B))
#mp.pyplot.plot(p, np.zeros(N), "o")
#
#
## the Q vector
#N = 20;
#B = 3;
#q = B**(1-np.logspace(0.1, 0.9, N,base=B))
#mp.pyplot.plot(q, np.ones(N), "o")


fh.close()
