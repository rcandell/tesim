function [tstart, tstop, xmeas_out, xmv_out] = run_tesim_init(mdlname, tstart, tstop)
%
% Author: Rick Candell
% Organization: National Institute of Standards and Technology
%               U.S. Department of Commerce
% License: Public Domain

if nargin < 3
    tstop = 10/3600;
end

if nargin < 2
    tstart = 0;
end

if nargin < 1
    mdlname = 'te_plant_controller_for_omnet';  
end

% open the model        
open(mdlname);
mdl = bdroot;

% controller and plant initial conditions
Ts_base = 1/3600;
Ts_scan = 1/3600;
Ts_save = 1/3600;
model_data_init(Ts_base, Ts_scan, Ts_save);

% run the simulation for the first time step
set_param(mdl, 'StartTime', num2str(0));
set_param(mdl, 'StopTime', num2str(0));
set_param(mdl, 'SaveFinalState', 'on');
set_param(mdl, 'FinalStateName', 'xFinal');
set_param(mdl, 'SaveFormat', 'Structure');
set_param(mdl, 'SaveCompleteFinalSimState', 'off');  % this is must 
set_param(mdl, 'LoadInitialState', 'on');
set_param(mdl, 'InitialState', 'xInitial');

% set the IC for plant to [] fo first run
assignin('base','tesim_ic',[]);

% set new start time
set_param(mdl, 'StartTime', num2str(tstart));

% set new stop time
set_param(mdl, 'StopTime', num2str(tstop));   

% run the simulation 
% disp(['tstart: ' get_param(mdl, 'StartTime')])
% disp(['tstop: ' get_param(mdl, 'StopTime')])
simout = sim(mdl);

% save states, inputs, and outputs to file
%  xFinal
xFinal = simout.get('xFinal'); 
save('xFinal.mat','xFinal');
%  xmeas
xmeas_out = simout.get('xmeas_out'); 
dlmwrite('xmeas_out.txt',xmeas_out,'\t');
%  xmv (includes on the nine used by the plant)
xmv_out = simout.get('xmv_out'); 
dlmwrite('xmv_out.txt',xmv_out,'\t');
%  xmv_full (includes all 12 manipulated)
xmv_full = simout.get('xmv_full'); 
dlmwrite('xmv_full.txt', xmv_full, '\t');
%  rx_k
dlmwrite('rx_k.txt', simout.get('rx_k'), '\t');

end  

function xmv_in = model_data_init(Ts_base, Ts_scan, Ts_save)

    % Base case initialization
    u0=[63.053, 53.98, 24.644, 61.302, 22.21, 40.064, 38.10, 46.534, 47.446, 41.106, 18.114, 50];
    assignin('base','u0',u0);
    assignin('base','Eadj_0',0);
    assignin('base','SP17_0',80.1);
    
    % controller loop initials
    for i=1:12;
        iChar=int2str(i);
        evalin('base',['xmv',iChar,'_0=u0(',iChar,');']);
    end
    
    % set the manipulated variables 
%     xmv_in = dlmread('xmv_in_init.txt', '\t');  
    xmv_in = u0([1 2 3 4 6 7 8 10 11]);
    assignin('base','xmv_in',xmv_in);

    % set the measured variables 
    xmeas_in = dlmread('xmeas_in_init.txt', '\t');  
    assignin('base','xmeas_in', xmeas_in);    

    Fp_0=100;           assignin('base','Fp_0',Fp_0);
    r1_0=0.251/Fp_0;    assignin('base','r1_0',r1_0);
    r2_0=3664/Fp_0;     assignin('base','r2_0',r2_0);
    r3_0=4509/Fp_0;     assignin('base','r3_0',r3_0);
    r4_0=9.35/Fp_0;     assignin('base','r4_0',r4_0);
    r5_0=0.337/Fp_0;    assignin('base','r5_0',r5_0);
    r6_0=25.16/Fp_0;    assignin('base','r6_0',r6_0);
    r7_0=22.95/Fp_0;    assignin('base','r7_0',r7_0);
    
    Eadj_0=0;           assignin('base','Eadj_0',0);
    SP17_0=80.1;        assignin('base','SP17_0',SP17_0);

    % TS_base is the integration step size in hours.  The simulink model
    % is using a fixed step size (Euler) method.  Variable step-size methods
    % don't work very well (high noise level).  TS_base is also the sampling
    % period of most discrete PI controllers used in the simulation.
    assignin('base','Ts_base',Ts_base);
    assignin('base','Ts_scan',Ts_scan);
    assignin('base','Ts_save',Ts_save);

    % initialize the IDV condition (disturbances)
    IDVspec = zeros(1, 20);
    assignin('base','IDVspec',IDVspec);
    
    xInitial = set_xInitial(bdroot, 0);
    assignin('base','xInitial',xInitial);

end

function xInitial = set_xInitial(model_name, cmd)
% This function evaluates to supply initial conditions to the specified
% model. 
%
% Usage:
% 
% initial_conditions <model_name: string> <cmd: int>
% where cmd indicates what to initialize
%   cmd = 0:  init the plant and controller
%   cmd = -1:  init the plant only
%   cmd = 1: init the controller only
%
if nargin < 2
    error 'Must specify model name and commands'
end

xInitial = struct;
xInitial.time = [];
xInitial.signals = struct;

if cmd <= 0
    xInitial.signals(1).values = [11.952176106184915 7.950300220587021 4.8684537248819986 ...
                                  0.27297286851217462 18.170957937566698 6.0756857605350705 ...
                                  138.77247446239622 136.12805974424231 2.5240502421064033 ...
                                  62.627138395126366 41.656312838317461 25.522301888401437 ...
                                  0.15168510251838813 10.746156544195594 3.5933712166350196 ...
                                  52.279082275951872 41.117503515415947 0.6488228317916539 ...
                                  0.42478972487351035 0.0078827063665042191 ...
                                  0.89337220488861935 0.0096027938562258663 ...
                                  0.51506858356948537 0.16681715341625392 48.183122896125951 ...
                                  39.420524571989105 0.380153203870486 113.68415846339275 ...
                                  52.611730377082857 66.661346315892743 21.212165336364368 ...
                                  58.941968290602773 14.336659127811398 17.649098880625942 ...
                                  8.5121732654982658 1.1379896577954212 102.4800282325948 ...
                                  92.262530705397239 62.806559460324159 53.286026966911585 ...
                                  26.661254009105338 60.484801083141221 4.44659081257122E-323 ...
                                  24.234688815797391 37.209102766947353 46.430892344071623 ...
                                  8.20148972096469E-322 35.944588369409736 ...
                                  12.209547880014606 99.999999999999915];
    xInitial.signals(1).dimensions = 50;
    xInitial.signals(1).label = 'CSTATE';
    xInitial.signals(1).blockName = [model_name '/TE Plant/TE Plant Model/TE Code'];
end

if cmd >= 0
    xInitial.signals(2).values = 62.806983901159221;
    xInitial.signals(2).dimensions = 1;
    xInitial.signals(2).label = 'DSTATE';
    xInitial.signals(2).blockName = [model_name '/Controller/D feed rate/Discrete PI/Unit Delay'];
    xInitial.signals(3).values = 53.286708861352047;
    xInitial.signals(3).dimensions = 1;
    xInitial.signals(3).label = 'DSTATE';
    xInitial.signals(3).blockName = [model_name '/Controller/E feed rate/Discrete PI/Unit Delay'];
    xInitial.signals(4).values = 26.662172563362368;
    xInitial.signals(4).dimensions = 1;
    xInitial.signals(4).label = 'DSTATE';
    xInitial.signals(4).blockName = [model_name '/Controller/A feed rate/Discrete PI/Unit Delay'];
    xInitial.signals(5).values = 60.482853992876215;
    xInitial.signals(5).dimensions = 1;
    xInitial.signals(5).label = 'DSTATE';
    xInitial.signals(5).blockName = [model_name '/Controller/C feed rate/Discrete PI/Unit Delay'];
    xInitial.signals(6).values = 24.229300906565072;
    xInitial.signals(6).dimensions = 1;
    xInitial.signals(6).label = 'DSTATE';
    xInitial.signals(6).blockName = [model_name '/Controller/Purge rate/Discrete PI/Unit Delay'];
    xInitial.signals(7).values = 37.208198128091411;
    xInitial.signals(7).dimensions = 1;
    xInitial.signals(7).label = 'DSTATE';
    xInitial.signals(7).blockName = [model_name '/Controller/Separator flow rate/Discrete PI/Unit Delay'];
    xInitial.signals(8).values = 46.430526395103641;
    xInitial.signals(8).dimensions = 1;
    xInitial.signals(8).label = 'DSTATE';
    xInitial.signals(8).blockName = [model_name '/Controller/Stripper flow rate/Discrete PI/Unit Delay'];
    xInitial.signals(9).values = 35.86532151232776;
    xInitial.signals(9).dimensions = 1;
    xInitial.signals(9).label = 'DSTATE';
    xInitial.signals(9).blockName = [model_name '/Controller/Reactor temperature/Unit Delay'];
    xInitial.signals(10).values = 12.930642047543767;
    xInitial.signals(10).dimensions = 1;
    xInitial.signals(10).label = 'DSTATE';
    xInitial.signals(10).blockName = [model_name '/Controller/Separator temperature/Unit Delay'];
    xInitial.signals(11).values = -0.44888336148273855;
    xInitial.signals(11).dimensions = 1;
    xInitial.signals(11).label = 'DSTATE';
    xInitial.signals(11).blockName = [model_name '/Controller/Production rate/Unit Delay'];
    xInitial.signals(12).values = -0.50665674770999658;
    xInitial.signals(12).dimensions = 1;
    xInitial.signals(12).label = 'DSTATE';
    xInitial.signals(12).blockName = [model_name '/Controller/%G in product/Vel PI/Unit Delay'];
    xInitial.signals(13).values = 0.63225024450851919;
    xInitial.signals(13).dimensions = 1;
    xInitial.signals(13).label = 'DSTATE';
    xInitial.signals(13).blockName = [model_name '/Controller/%G in product/Unit Delay'];
    xInitial.signals(14).values = 0.0027228362442928825;
    xInitial.signals(14).dimensions = 1;
    xInitial.signals(14).label = 'DSTATE';
    xInitial.signals(14).blockName = [model_name '/Controller/Ratio trimming/Unit Delay'];
    xInitial.signals(15).values = 0.15679989365145985;
    xInitial.signals(15).dimensions = 1;
    xInitial.signals(15).label = 'DSTATE';
    xInitial.signals(15).blockName = [model_name '/Controller/yA control/Unit Delay'];
    xInitial.signals(16).values = 0.0026940509994885509;
    xInitial.signals(16).dimensions = 1;
    xInitial.signals(16).label = 'DSTATE';
    xInitial.signals(16).blockName = [model_name '/Controller/A feed rate/Discrete PI/Vel PI/Unit Delay'];
    xInitial.signals(17).values = -0.19359235829376331;
    xInitial.signals(17).dimensions = 1;
    xInitial.signals(17).label = 'DSTATE';
    xInitial.signals(17).blockName = [model_name '/Controller/yAC control/Unit Delay'];
    xInitial.signals(18).values = 0.0922366099003225;
    xInitial.signals(18).dimensions = 1;
    xInitial.signals(18).label = 'DSTATE';
    xInitial.signals(18).blockName = [model_name '/Controller/Ratio trimming/Unit Delay1'];
    xInitial.signals(19).values = 0.083514558990948728;
    xInitial.signals(19).dimensions = 1;
    xInitial.signals(19).label = 'DSTATE';
    xInitial.signals(19).blockName = [model_name '/Controller/C feed rate/Discrete PI/Vel PI/Unit Delay'];
    xInitial.signals(20).values = 62.429570964992763;
    xInitial.signals(20).dimensions = 1;
    xInitial.signals(20).label = 'DSTATE';
    xInitial.signals(20).blockName = [model_name '/Controller/D feed rate/Discrete PI/Vel PI/Unit Delay'];
    xInitial.signals(21).values = 9.7233862746652449;
    xInitial.signals(21).dimensions = 1;
    xInitial.signals(21).label = 'DSTATE';
    xInitial.signals(21).blockName = [model_name '/Controller/E feed rate/Discrete PI/Vel PI/Unit Delay'];
    xInitial.signals(22).values = -0.140664345075038;
    xInitial.signals(22).dimensions = 1;
    xInitial.signals(22).label = 'DSTATE';
    xInitial.signals(22).blockName = [model_name '/Controller/Production rate/Vel PI/Unit Delay'];
    xInitial.signals(23).values = 0.0019155098024649312;
    xInitial.signals(23).dimensions = 1;
    xInitial.signals(23).label = 'DSTATE';
    xInitial.signals(23).blockName = [model_name '/Controller/Reactor pressure/Unit Delay'];
    xInitial.signals(24).values = -0.0063601433077061786;
    xInitial.signals(24).dimensions = 1;
    xInitial.signals(24).label = 'DSTATE';
    xInitial.signals(24).blockName = [model_name '/Controller/Purge rate/Discrete PI/Vel PI/Unit Delay'];
    xInitial.signals(25).values = 0.0407516156845702;
    xInitial.signals(25).dimensions = 1;
    xInitial.signals(25).label = 'DSTATE';
    xInitial.signals(25).blockName = [model_name '/Controller/Reactor level/Vel PI/Unit Delay'];
    xInitial.signals(26).values = 92.020048977831067;
    xInitial.signals(26).dimensions = 1;
    xInitial.signals(26).label = 'DSTATE';
    xInitial.signals(26).blockName = [model_name '/Controller/Reactor level/Unit Delay'];
    xInitial.signals(27).values = 1.0408538619039973;
    xInitial.signals(27).dimensions = 1;
    xInitial.signals(27).label = 'DSTATE';
    xInitial.signals(27).blockName = [model_name '/Controller/Reactor pressure/Vel PI/Unit Delay'];
    xInitial.signals(28).values = 0.0079613898323600552;
    xInitial.signals(28).dimensions = 1;
    xInitial.signals(28).label = 'DSTATE';
    xInitial.signals(28).blockName = [model_name '/Controller/Reactor temperature/Vel PI/Unit Delay'];
    xInitial.signals(29).values = 0.25370500558318143;
    xInitial.signals(29).dimensions = 1;
    xInitial.signals(29).label = 'DSTATE';
    xInitial.signals(29).blockName = [model_name '/Controller/Separator level/Unit Delay'];
    xInitial.signals(30).values = 0.048094569533532194;
    xInitial.signals(30).dimensions = 1;
    xInitial.signals(30).label = 'DSTATE';
    xInitial.signals(30).blockName = [model_name '/Controller/Separator flow rate/Discrete PI/Vel PI/Unit Delay'];
    xInitial.signals(31).values = -1.0417700470544915;
    xInitial.signals(31).dimensions = 1;
    xInitial.signals(31).label = 'DSTATE';
    xInitial.signals(31).blockName = [model_name '/Controller/Separator level/Vel PI/Unit Delay'];
    xInitial.signals(32).values = -0.14156661603362863;
    xInitial.signals(32).dimensions = 1;
    xInitial.signals(32).label = 'DSTATE';
    xInitial.signals(32).blockName = [model_name '/Controller/Separator temperature/Vel PI/Unit Delay'];
    xInitial.signals(33).values = 0.2287117711745113;
    xInitial.signals(33).dimensions = 1;
    xInitial.signals(33).label = 'DSTATE';
    xInitial.signals(33).blockName = [model_name '/Controller/Stripper level/Unit Delay'];
    xInitial.signals(34).values = 0.077381965807635567;
    xInitial.signals(34).dimensions = 1;
    xInitial.signals(34).label = 'DSTATE';
    xInitial.signals(34).blockName = [model_name '/Controller/Stripper flow rate/Discrete PI/Vel PI/Unit Dela' ...
                                      'y'];
    xInitial.signals(35).values = 0.94917471361589634;
    xInitial.signals(35).dimensions = 1;
    xInitial.signals(35).label = 'DSTATE';
    xInitial.signals(35).blockName = [model_name '/Controller/Stripper level/Vel PI/Unit Delay'];

end

end
