
clear;

% global Ts_base
% global Ts_scan
% global Ts_save
% global IDVspec
% global xmv_in
% global xmeas_in
% global xInitial
% global model_type

tstart = 0;
tstep = (10E-3)/3600; % 10ms in hours 

% name of the model
mdlname = 'te_plant_controller_for_omnet'

model_type = 0; % plant and controller
init_model_omnet;
xmeas_in = [0.270700856883759,3664.69063568155,4425.81806289222,9.28480585069764,32.1900221885694,47.1917509274582,2798.76379680602,63.8910533761394,122.910026775311,0.198219923127051,91.9874459238251,50.9488299638826,2704.04100119615,25.1009447388150,48.4018265276201,3331.20881059281,23.1886690470323,66.6270819394961,-1.91085306778550,272.695609000679,102.468192217436,92.2852078244913,32.1496517328533,14.8784917049868,18.8516948805069,5.99875776257265,16.6686702756563,4.05437841297992,32.6296761633793,21.7035622719829,13.2975011616905,0.895835584752024,16.1374011792979,5.39613141410103,6.67536112633276,3.26453109846362,0.0107148709581566,0.574717471829245,0.186135857878832,53.7630976903944,43.9857233442386];
xmv_in = [...
            62.8066628843803,...
            53.2853166090127,...
            26.6622397978940,...
            60.4826070428628,...
            24.2240443611559,...
            37.2084224738049,...
            46.4297284526017,...
            35.9192771543255,...
            12.3742025071079];
open(mdlname);
mdl = bdroot;

% number of iterations to run
N = 10;

% run the simulation for the first time step
set_param(mdl, 'StartTime', num2str(0));
set_param(mdl, 'StopTime', num2str(0));
set_param(mdl, 'SaveFinalState', 'on');
set_param(mdl, 'FinalStateName', 'xFinal');
set_param(mdl, 'SaveCompleteFinalSimState', 'on');
set_param(mdl, 'LoadInitialState', 'on');
%set_param(mdl, 'SaveCompleteFinalSimState', 'on');
set_param(mdl, 'InitialState', 'xInitial');

for ii = 1:N

    % increment stop time
    disp(['Start time: '    num2str(tstart)]);
    disp(['Old Stop Time: ' get_param(mdl, 'StopTime')]);   
    tstop = tstart + ii*tstep;  
    disp(['New Stop time: ' num2str(tstop)]);
    set_param(mdl, 'StartTime', num2str(tstart));
    set_param(mdl, 'StopTime', num2str(tstop));   
    
    % run the simulation again
    simout = sim(mdl);
    
    % fetch the manipulated variables from OMNET++
    xmv_in = xmv_out;        % OMNET++ may delay some elements
    xmeas_in = xmeas_out;    % OMNET++ may delay some elements
 
    % propagate the state of the model
    xInitial = xFinal; 

end





