
%% clear out memory
clear;

%% run the initialization routine
[tstart, tstop, xmeas_out, xmv_out] = run_tesim_init();
dlmwrite('tesim_fullout.txt',[tstart tstop xmv_in xmeas_in])

%% incrementaly run the simulation
tstep = 1/3600;
for ii = 1:1000
    
    % set timing
    tstart = tstop;
    tstop = tstart + tstep;
    
    % call the omnet++ mock routine
    omnetpp()
    
    % call the next iteration of tesim
    disp(['start: ' num2str(tstart) ' stop: ' num2str(tstop)]);
    [tstart, tstop, xmeas_out, xmv_out] = run_tesim_next(tstart, tstep);
    
    % log the outputof the simulation
    dlmwrite('tesim_fullout.txt',[tstart tstop xmv_in xmeas_in],'-append')
    
    if ~mod(ii,5)
        disp('waiting for input ....')
        pause(5)
    end
end
