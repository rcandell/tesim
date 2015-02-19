function [tstart, tstop, xmeas, xmv, states] = run_tesim_func(mdlname, tstart, tstop, p_xmv, p_xmeas, p_states)

if nargin < 6
    p_states = 'states_default.txt';
end

if nargin < 5
    p_xmeas = 'xmeas_default.txt';
end

if nargin < 4
    p_xmv = 'xmv_default.txt';
end

if nargin < 3
    tstop = 1;
end

if nargin < 2
    tstart = 0;
end

if nargin < 1
    mdlname = 'te_plant_controller_for_omnet';  
end

% load the last values
xmeas_in = load_file(p_xmeas);
xmv_in = load_file(p_xmv);
tesim_ic = load_file(p_states);

% load the initial conditions
        
open(mdlname);
mdl = bdroot;

% initialize model
% init_model_omnet;
xInitial = initial_conditions(bdroot, model_type);
assignin('base','xInitial',xInitial);

% run the simulation for the first time step
set_param(mdl, 'StartTime', num2str(0));
set_param(mdl, 'StopTime', num2str(0));
set_param(mdl, 'SaveFinalState', 'on');
set_param(mdl, 'FinalStateName', 'xFinal');
set_param(mdl, 'SaveFormat', 'Structure');
set_param(mdl, 'SaveCompleteFinalSimState', 'off');  % this is must for the approach to work
set_param(mdl, 'LoadInitialState', 'on');
set_param(mdl, 'InitialState', 'xInitial');

tstart = 0;
tstep = (10E-3)/3600; % 10ms in hours 
tstop = tstep;

tesim_ic = [];


% set new start time
set_param(mdl, 'StartTime', num2str(tstart));

% set new stop time
set_param(mdl, 'StopTime', num2str(tstop));   

% run the simulation again
disp(['tstart: ' num2str(tstart)])
disp(['tstop: ' num2str(tstop)])
simout = sim(mdl);

% fetch the manipulated variables from OMNET++
% fetch from file later
xmv_out = simout.get('xmv_out');
xmv_in = xmv_out;        % OMNET++ may delay some elements

% fetch the measured variables from OMNET++
% fetch from file later
xmeas_out = simout.get('xmeas_out')
xmeas_in = xmeas_out;    % OMNET++ may delay some elements

% propagate the state of the model
xInitial = simout.get('xFinal'); 
% explicitly load the plant initial conditions
tesim_ic = simout.get('xFinal').signals(1).values;

% update start and stop times
tstart = tstop;
tstop = tstop + tstep;  


end  


function M = load_file(fname)

%     fd = fopen(fname,'r');
%     if fd == -1
%         error([fname 'not found'])
%     end
    
    M = dlmread(fname, '\t');

end



