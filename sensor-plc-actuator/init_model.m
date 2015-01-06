% Base case initialization

global Ts_base
global Ts_scan
global Ts_save
global IDVspec

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
xInitial = initial_conditions_combined('te_plant_controller')

% TS_base is the integration step size in hours.  The simulink model
% is using a fixed step size (Euler) method.  Variable step-size methods
% don't work very well (high noise level).  TS_base is also the sampling
% period of most discrete PI controllers used in the simulation.
% Ts_base=0.0005/2;  % in hours (original setting)
%Ts_base=1/3600;  % seconds seconds converted hours
Ts_base=1/1800;  % seconds seconds converted hours
Ts_scan=Ts_base*10;   

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
if license('checkout','instrument_control_toolbox')
    echoudp('off')
    % measurement ports  - for the controller listener
    for port = 9901:9941
        echoudp('on', port)
    end
    % manipulated variables ports - for the plant listener
    for port = 9951:9962
        echoudp('on', port)
    end
end

% % If p is the probability of going from good to bad, and if r is the
% % probability of going from bad to good, then this code will generate a
% % packet loss pattern with burst losses. 
% 
% gtob=[0.8 ,1];
% btog=[0,.5];
% xmv_vec_length=12;
% xmeas_vec_length=41;
% xmv_init = [62.807, 53.287, 26.662, 60.483, 0, 24.229, 37.208, 46.431, 0, 35.865, 12.931, 100]';
% 
% xmeas_init = [0.25052, 3664, 4509.3, 9.3477, 26.902, 42.339, 2705, 75, 120.4, 0.33712, 80.109,... 
%     50, 2633.7, 25.16, 50, 3102.2, 22.949, 65.731, 230.31, 341.43, 94.599, 77.297, 32.188,... 
%     8.8933, 26.383,  6.882, 18.776, 1.6567, 32.958, 13.823, 23.978, 1.2565, 18.579, 2.2633,... 
%     4.8436, 2.2986, 0.017866, 0.8357, 0.098577, 53.724, 43.828]';
% 
% % set R & P in the top-level script
% % by default, set to always good state
% if ~exist('P','var') 
%     P=0;
% end
% if ~exist('R','var') 
%     R=1;
% end
% p_xmv = P+zeros(xmv_vec_length,1);
% r_xmv = R+zeros(xmv_vec_length,1);
% p_xmeas = P+zeros(xmeas_vec_length,1);
% r_xmeas = R+zeros(xmeas_vec_length,1);
% % p_xmv = gtob(1) + (gtob(2)-gtob(1)).*rand(xmv_vec_length,1);
% % r_xmv = btog(1) + (btog(2)-btog(1)).*rand(xmv_vec_length,1);
% % p_xmeas = gtob(1) + (gtob(2)-gtob(1)).*rand(xmeas_vec_length,1);
% % r_xmeas = btog(1) + (btog(2)-btog(1)).*rand(xmeas_vec_length,1);
% 
% initialize the IDV condition (disturbances)
if isempty(evalin('base','IDVspec'))
    IDVspec = zeros(1, 20);
%    IDVmat = zeros(1, 20);
%    IDVmat(4) = 1; % Reactor cooling water inlet temperature
end
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





