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





