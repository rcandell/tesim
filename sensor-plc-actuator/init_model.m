% Base case initialization

global Ts_base
global Ts_scan
global Ts_save
global IDVspec
global model_type

u0=[63.053, 53.98, 24.644, 61.302, 22.21, 40.064, 38.10, 46.534, 47.446, 41.106, 18.114, 50];
    
for i=1:12;
    iChar=int2str(i);
    eval(['xmv',iChar,'_0=u0(',iChar,');']);
end

Fp_0=100;

r1_0=0.251/Fp_0;
r2_0=3664/Fp_0;
r3_0=4509/Fp_0;
r4_0=9.35/Fp_0;
r5_0=0.337/Fp_0;
r6_0=25.16/Fp_0;
r7_0=22.95/Fp_0;

Eadj_0=0;
SP17_0=80.1;

% Note:  The values of xmv_0 and r_0 specified above get overridden
%        by the initial conditions specified in the xInitial variable,
%        loaded in the following statement.  The above statements are
%        only needed when starting from a new condition where xInitial
%        doesn't apply.

%load initial conditions for model signals
disp(['name of model: ' bdroot])
xInitial = initial_conditions(bdroot, model_type)
%xInitial = initial_conditions_combined('te_plant_controller')

% TS_base is the integration step size in hours.  The simulink model
% is using a fixed step size (Euler) method.  Variable step-size methods
% don't work very well (high noise level).  TS_base is also the sampling
% period of most discrete PI controllers used in the simulation.
% Ts_base=0.0005/2;  % in hours (original setting)
Ts_base         = 1/3600;  % 1 second in hours 
Ts_scan         = 60*Ts_base;
% TS_SCAN cannot go longer than 125 times Ts_base

% SENSOR SAMPLE TIMES
Ts_sensor       = repmat(Ts_scan,41,1);
%Ts_sensor(10)   = 1*Ts_sensor(10);

% ACTUATOR SAMPLE TIMES
Ts_actuator       = repmat(Ts_scan,12,1);
%Ts_actuator(3)   = 1*Ts_actuator(3);

% PLC processing delay
Dprocplc = 0; %Ts_base;

% TS_save is the sampling period for saving results.  The following
% variables are saved at the end of a run:
% tout    -  the elapsed time (hrs), length N.
% simout  -  the TE plant outputs, N by 41 matrix
% OpCost  -  the instantaneous operating cost, $/hr, length N
% xmv     -  the TE plant manipulated variables, N by 12 matrix
% idv     -  the TE plant disturbances, N by 20 matrix
% Ts_save=0.01;  % the original setting
Ts_save=Ts_base;

% enable my udp servers
% note: port numbers must be chosen carefully to avoid contention and slow
% down of the simulation
echoudp('off')
remote_port = 25001
echoudp('on',remote_port)
xmeas_local_port_tx = 9601:9699;
xmeas_local_port_rx = xmeas_local_port_tx;%9701:9799;
xmv_local_port_tx = 9801:9899;
xmv_local_port_rx = xmv_local_port_tx;%9901:9999;

if license('checkout','instrument_control_toolbox')
  disp('license available')
else
  disp('no license available')
end

% initialize the IDV condition (disturbances)
IDVspec = zeros(1, 20);
IDVspec(1) = 0; % a/c feed ratio, b composition constant
IDVspec(2) = 0; 
IDVspec(3) = 0;
IDVspec(4) = 0; % Reactor cooling water inlet temperature, step change
IDVspec(5) = 0;
IDVspec(6) = 0;
IDVspec(4) = 0;
IDVspec(8) = 0;
IDVspec(9) = 0;
IDVspec(10) = 0;
IDVspec(11) = 0; % reactor cooling inlet temp, random variation
IDVspec(12) = 0; % condenser cooling inlet temp, random variation
IDVspec(13) = 0; % reaction kinectics, slow drift
IDVspec(14) = 0; % reactor cooling valve sticks
IDVspec(15) = 0; % condenser cooling valve sticks
IDVspec(16) = 0;
IDVspec(17) = 0;
IDVspec(18) = 0;
IDVspec(19) = 0; 
IDVspec(20) = 0; 
IDVspec %#ok<NOPTS>

% 
% % initialize the sensor override (for Override and MITM attacks)
% orv_idx_sensors = [-1];  % disable override
% % orv_idx_sensors = [7];  % reactor pressure
% orv_values_sensors = [1000];
% 
% % initialize the actuator override (for Override and MITM attacks)
% orv_idx_actuators = [-1];  % disable override
% % orv_idx_actuators = [1];  % D feed flow
% % orv_idx_actuators = [6];  % purge valve
% orv_values_actuators = [15]; 
% 
% % Note that setting the purge valve to 15% allows the process to continue
% % operating smoothly and at quality while the reactor temperature builds.
% % A simple MITM attack would run the reactor to an overpressure condition
% % within 15 hours and ultimately cause a safety issue.
% % Also, note that when the purge valve is stuck at one number, the
% % production cost is flat versus oscillatory for the baseline case.  This
% % is due to the fact that the purged materials is a major factor in cost
% % calculation.
% 
% % calculate the seed for the random number generator
% %   for the actuators channel
% rng('shuffle','twister');
% RNGSEED1 = round(rand(1,12)*1000);
% %   for the sensors channel
% RNGSEED2 = round(rand(1,41)*1000);





