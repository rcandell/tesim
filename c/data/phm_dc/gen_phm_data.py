# Author:       Richard Candell (rick.candell@nist.gov)
# Organization: National Institute of Standards and Technology
#               U.S. Department of Commerce
# License:      Public Domain

import subprocess 
import numpy as np
import matplotlib as mp
import time
import atexit
import os

P_list = []
# setup parameters for simulations
DURATION = 72
TS = 0.0005
TC = 0.0005
KSV = 20
PREFIX = "PHM"
IDV_VEC = range(0, 16)
PER = 0.65
NRUNS = 10

'''
When the main function exits, this kills all subprocesses
created by the script.
'''
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

'''
main() - Produces the data sets used for training
processes are allocated to CPU's according to current load
'''
def main():
    EXE = 'C:/git/tesim/c/Debug/tesim.exe'
    #fh = open("simlog.dat", "w")
    fh = open(os.devnull,"w")  # faster operation, but no file

    '''
    Training data set 1:
    The chemical plant is run at steady state with physical disturbances.
    The physical disturbances are defined in Down and Vogel, 1993
    Ten (10) runs of each disturbance type are produced as separate files.
    '''
    # all physical disturbances individually
    for IDV in IDV_VEC:
        for run in range(NRUNS):
            for per in [0, PER]:
                prefix_str = PREFIX + "_IDV_" + str(IDV) + "_run_" + str(run)
                call_str = [EXE,
                            "-s", str(DURATION),
                            "-t", str(TS),
                            "-c", str(TC),
                            "--per", str(PER),
                            "-k", str(KSV),
                            "-p", prefix_str,
                            "-i", str(IDV)]
                print(call_str)
                p = subprocess.Popen(call_str, stdout = fh, stderr = fh)
                P_list.append(p)

        # wait for al lthe runs to complete
        exit_codes = [pi.wait() for pi in P_list]
        print(exit_codes)

    '''
    Training data set 2:
    The reactor is run at steady state but with a bad communications link for
    the reactor sensor reading.  A Gilbert Elliot channel model is used to model the link's
    behavior.
    '''
    # reactor pressure sensor comms. link going bad
    for run in range(NRUNS):
        prefix_str = PREFIX + "_reactor_pressure_sensor_comms_failure" + "_run_" + str(run)
        call_str = [EXE,
            "-s", str(DURATION),
            "-t", str(TS),
            "-c", str(TC),
            "-g",
            "--xmeas-pq", "0.8:0.3",
            "--xmeas-ge-chan-id","7",
            "-k", str(KSV),
            "-p", prefix_str]
        print(call_str)
        p = subprocess.Popen(call_str, stdout=fh, stderr=fh)
        P_list.append(p)
        exit_codes = [pi.wait() for pi in P_list]
        print(exit_codes)

    '''
    Training data set 3:
    The reactor is run at steady state but with a bad communications link for
    the reactor cooling water temperature reading.  A Gilbert Elliot channel model
    is used to model the link's behavior.
    '''
    # reactor cooling water sensor comms. link going bad
    for run in range(NRUNS):
        prefix_str = PREFIX + "_reactor_cooling_water_sensor_comms_failure" + "_run_" + str(run)
        call_str = [EXE,
            "-s", str(DURATION),
            "-t", str(TS),
            "-c", str(TC),
            "-g",
            "--xmeas-pq", "0.8:0.3",
            "--xmeas-ge-chan-id","21",
            "-k", str(KSV),
            "-p", prefix_str]
        print(call_str)
        p = subprocess.Popen(call_str, stdout = fh, stderr = fh)
        P_list.append(p)
    exit_codes = [pi.wait() for pi in P_list]
    print(exit_codes)

    '''
    Training data set 4:
    The reactor is run at steady state but with a bad communications link for
    the purge valve command channel.  A Gilbert Elliot channel model is used to model
    the link's behavior.
    '''
    # purge valve actuator comms. link going bad
    for run in range(NRUNS):
        prefix_str = PREFIX + "_purge_valve_comms_failure" + "_run_" + str(run)
        call_str = [EXE,
            "-s", str(DURATION),
            "-t", str(TS),
            "-c", str(TC),
            "-g",
            "--xmv-pq", "0.8:0.3",
            "--xmv-ge-chan-id","6",
            "-k", str(KSV),
            "-p", prefix_str]
        print(call_str)
        p = subprocess.Popen(call_str, stdout = fh, stderr = fh)
        P_list.append(p)
    exit_codes = [pi.wait() for pi in P_list]
    print(exit_codes)

    '''
    Training data set 5:
    The reactor is run at steady state but with a bad communications link for
    the A feed flow command channel.  A Gilbert Elliot channel model is used to model
    the link's behavior.
    '''
    # A feed flow actuator comms. link going bad
    for run in range(NRUNS):
        prefix_str = PREFIX + "_Afeed_valve_comms_failure" + "_run_" + str(run)
        call_str = [EXE,
            "-s", str(DURATION),
            "-t", str(TS),
            "-c", str(TC),
            "-g",
            "--xmv-pq", "0.8:0.3",
            "--xmv-ge-chan-id","3",
            "-k", str(KSV),
            "-p", prefix_str]
        print(call_str)
        p = subprocess.Popen(call_str, stdout = fh, stderr = fh)
        P_list.append(p)
    exit_codes = [pi.wait() for pi in P_list]
    print(exit_codes)

    '''
    Training data set 6:
    The reactor is run at steady state but with a bad communications link for
    the D feed flow command channel.  A Gilbert Elliot channel model is used to model
    the link's behavior.
    '''
    # A feed flow actuator comms. link going bad
    for run in range(NRUNS):
        prefix_str = PREFIX + "_Dfeed_valve_comms_failure" + "_run_" + str(run)
        call_str = [EXE,
            "-s", str(DURATION),
            "-t", str(TS),
            "-c", str(TC),
            "-g",
            "--xmv-pq", "0.8:0.3",
            "--xmv-ge-chan-id","1",
            "-k", str(KSV),
            "-p", prefix_str]
        print(call_str)
        p = subprocess.Popen(call_str, stdout = fh, stderr = fh)
        P_list.append(p)
    exit_codes = [pi.wait() for pi in P_list]
    print(exit_codes)

    # close the simulation log file
    fh.close()


if __name__ == "__main__":
    atexit.register(cleanup)
    main()