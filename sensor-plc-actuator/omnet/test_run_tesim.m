%
% Author: Rick Candell
% Organization: National Institute of Standards and Technology
%               U.S. Department of Commerce
% License: Public Domain

%% clear out memory
clear;

%% run the initialization routine
%   the base case starts off in a steady state condition
[tstart, tstop, xmeas_ii, xmv_ii] = run_tesim_init();
disp(['start: ' num2str(tstart) ' stop: ' num2str(tstop)]);
lvals = [tstart tstop xmv_ii xmeas_ii];
save('tesim_fullout.txt','lvals','-ascii','-tabs')

%% incrementaly run the simulation
tstep = 1/3600;
N = 10/tstep; % 10 hours
for ii = 2:N
    
    % set new start time
    tstart = tstop;
    
    % call the omnet++ mock routine
    omnetpp()
    
    % todo: insert the distributance vector
    %  read disturbance vector from next row in idv file
    
    % call the next iteration of tesim
    [tstart, tstop, xmeas_ii, xmv_ii] = run_tesim_next(tstart, tstep);
    disp(['start: ' num2str(tstart) ' stop: ' num2str(tstop)]);
    
    % log the outputof the simulation
    lvals = [tstart tstop xmv_ii xmeas_ii];
    save('tesim_fullout.txt','lvals','-ascii','-tabs','-append') 

end

%% post processing 
n_time = 2;
n_xmv = 9;
reactor_press_i = 7 + n_time + n_xmv;
X = dlmread('tesim_fullout.txt','\t');
plot(X(:,reactor_press_i))


