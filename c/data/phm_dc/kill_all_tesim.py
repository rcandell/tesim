import psutil

PROCNAME = "tesim.exe"

for proc in psutil.process_iter():
    # check whether the process name matches
    try:
        #print(proc.pid, proc.name())
        if proc.name() == PROCNAME:
            print("KILLING " + str(proc.pid))
            proc.kill()
    except Exception as e:
        None
        #print(e)
    


