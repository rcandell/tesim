%
% Author: Rick Candell
% Organization: National Institute of Standards and Technology
%               U.S. Department of Commerce
% License: Public Domain

%% clear out memory
clear;

%% Sample times
% TODO: Setup sample times structure and propagate through the model from top
%   or use a class to store and retrieve all of the information instead
%   ???? Why in the world didn't I think of that before ????

%% run the initialization routine
%   the base case starts off in a steady state condition
[tstart, tstop, xmeas_ii, xmv_ii] = run_tesim_init();
disp(['start: ' num2str(tstart) ' stop: ' num2str(tstop)]);
lvals = [tstart tstop xmv_ii xmeas_ii];
save('tesim_fullout.txt','lvals','-ascii','-tabs')

%% incrementally run the simulation
tstep = 1/3600;
N = 48/tstep; % 10 hours
rp = nan(N,2);
rp(1,:) = [tstop, xmeas_ii(7)];
for ii = 2:N
    
    % set new start time
    tstart = tstop;
    
    % call the omnet++ mock routine
    omnetpp()
    
    % TODO: insert the disturbance vector
    %  read disturbance vector from next row in idv file
    
    % call the next iteration of tesim
    [tstart, tstop, xmeas_ii, xmv_ii] = run_tesim_next(tstart, tstep);
    disp(['start: ' num2str(tstart) ' stop: ' num2str(tstop)]);
    disp(['Reactor pressure: ' num2str(xmeas_ii(7))])
    
    % log the outputof the simulation
    lvals = [tstart tstop xmv_ii xmeas_ii];
    save('tesim_fullout.txt','lvals','-ascii','-tabs','-append') 
    
    rp(ii,:) = [tstop, xmeas_ii(7)];
    plot(rp(:,1),rp(:,2))
    drawnow

end

%% post processing 
n_time = 2;
n_xmv = 12;
reactor_press_i = 7 + n_time + n_xmv;
X = dlmread('tesim_fullout.txt','\t');
plot(X(:,reactor_press_i))


