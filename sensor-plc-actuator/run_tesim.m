function y = run_tesim( tstart, tstop, tstep )
%RUN_MODEL Summary of this function goes here
%   Detailed explanation goes here

if nargin < 3
    tstep = (10E-3)/3600; % 10ms in hours 
end

if nargin < 2
    tstop = 10;
end

if nargin < 1
    tstart = 0;
end

global Ts_base
global Ts_scan
global Ts_save
global IDVspec
global xmv_in
global xmeas_in
global xInitial
global model_type

% name of the model
mdlname = 'te_plant_controller_for_omnet'

model_type = 0; % plant and controller
init_model_omnet;
xmeas_in = [0 zeros(1,41)];
xmv_in = [0 zeros(1,9)];
open(mdlname);
mdl = bdroot;

% number of iterations to run
N = 10;

% run the simulation for the first time step
% simOut = sim(mdl, [0 tstep]);
out3 = sim(mdl, 'StopTime', '5', 'SaveFinalState', 'on', ...
          'LoadInitialState', 'off', 'SaveCompleteFinalSimState', 'on',...
          'FinalStateName', 'xFinal');

for ii = 1:N
    % increment clock
    
    % run the simulation for some time
    simOut = sim(mdl,  [tstart tstop]);
    
    % save the states
 
%     set_param(mdl, 'SaveFinalState', 'off')
    
    % update the next sim times
    TStart = TFinal;
    TFinal = TFinal + tstep;
    
    % save the 

end

end

function save_mdl_state(mdl, fname)
end

function load_mdl_state(mdl, fname)
end

function run_model(mdl, tstart, tstep, fname_states)
    % open controller and initialize states
    open(mdlname_plant);
    mdl = bdroot;
    
    % run controller for incremental time step
    
    % save the controller states
    
end


