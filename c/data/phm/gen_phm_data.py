# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import subprocess 
import numpy as np
import matplotlib as mp
import time
import atexit

P_list = []
# setup parameters for simulations
DURATION = 72
TS = 0.0005
TC = 0.0005
TSV = 1
PREFIX = "PHM"
IDV_VEC = range(0, 16)
PER = 0.65

def cleanup():
     timeout_sec = 5
     for p in P_list: # list of your processes
         p_sec = 0
         for second in range(timeout_sec):
             if p.poll() == None:
                 time.sleep(1)
                 p_sec += 1
         if p_sec >= timeout_sec:
             p.kill() # supported from python 2.6
     print('cleaned up!')


def main():
    EXE = 'C:/git/tesim/c/Debug/tesim.exe'
    fh = open("simlog.dat", "w")

    # all disturbances individually
    for IDV in IDV_VEC:
        for run in range(10):
            for per in [0, PER]:
                prefix_str = PREFIX + "_IDV_" + str(IDV) + "_run_" + str(run)
                call_str = [EXE,
                            "-s", str(DURATION),
                            "-t", str(TS),
                            "-c", str(TC),
                            "--per", str(PER),
                            "-k", str(TSV),
                            "-p", prefix_str,
                            "-i", str(IDV)]
                print(call_str)
                p = subprocess.Popen(call_str, stdout = fh, stderr = fh)
                P_list.append(p)

    # reactor pressure sensor comms. link going bad
    for run in range(10):
        prefix_str = PREFIX + "_reactor_pressure_sensor_comms_failure" + "_run_" + str(run)
        call_str = [EXE,
            "-s", str(DURATION),
            "-t", str(TS),
            "-c", str(TC),
            "-g",
            "--xmeas-pq", "0.8:0.3",
            "--xmeas-ge-chan-id","7",
            "-k", str(TSV),
            "-p", prefix_str]
        print(call_str)
        p = subprocess.Popen(call_str, stdout=fh, stderr=fh)
        P_list.append(p)

    # reactor cooling water sensor comms. link going bad
    for run in range(10):
        prefix_str = PREFIX + "_reactor_cooling_water_sensor_comms_failure" + "_run_" + str(run)
        call_str = [EXE,
            "-s", str(DURATION),
            "-t", str(TS),
            "-c", str(TC),
            "-g",
            "--xmeas-pq", "0.8:0.3",
            "--xmeas-ge-chan-id","21",
            "-k", str(TSV),
            "-p", prefix_str]
        print(call_str)
        p = subprocess.Popen(call_str, stdout = fh, stderr = fh)
        P_list.append(p)

    # purge valve actuator comms. link going bad
    for run in range(10):
        prefix_str = PREFIX + "_purge_valve_comms_failure" + "_run_" + str(run)
        call_str = [EXE,
            "-s", str(DURATION),
            "-t", str(TS),
            "-c", str(TC),
            "-g",
            "--xmv-pq", "0.8:0.3",
            "--xmv-ge-chan-id","6",
            "-k", str(TSV),
            "-p", prefix_str]
        print(call_str)
        p = subprocess.Popen(call_str, stdout = fh, stderr = fh)
        P_list.append(p)

    exit_codes = [pi.wait() for pi in P_list]
    print(exit_codes)

    fh.close()


if __name__ == "__main__":
    atexit.register(cleanup)
    main()