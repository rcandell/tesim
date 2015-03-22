%
% Author:         Tim Zimmerman (timothy.zimmerman@nist.gov)
% Organization:   National Institute of Standards and Technology
%                 U.S. Department of Commerce
% License:        Public Domain
%
% Description:
% Creates two plots: production rate vs. time and product quality vs. time
% using an output log file from the TESim plant. The script will prompt the
% user to select the log file upon execution (if the file is not in the 
% 'Release' directory). Both the production rate and 'mol % G in product' 
% include the ±5% error bounds for the setpoint. These error bounds will 
% follow any rate-limited changes that occur to the setpoint, if it was 
% changed during plant operation.
%

% Column definitions (in case the log file changes)
stripperUnderflow_col   = 18;
prodRate_SP_col         = 63;
product_D_col           = 38;
product_E_col           = 39;
product_F_col           = 40;
product_G_col           = 41;
product_H_col           = 42;
pctG_SP_col             = 62;

% Check to see if the log file exists in the 'Release' directory
% If not, prompt the user for the file location.
if exist('../c/Release/xmeas_xmv_outputs.log', 'file') 
    load ../c/Release/xmeas_xmv_outputs.log;
    logData = xmeas_xmv_outputs;
else 
    [filename, path] = uigetfile('*.log', 'Select the TESim output log file...');
    logData =  importdata(fullfile(path,filename), '\t');
end

% Store the time values
t = logData(:,1);
% Determine the maximum time for autoscaling
t_len = max(t);

%%%%%%%%%%%%%%%%%%%%%%%%
% Production Rate Plot %
%%%%%%%%%%%%%%%%%%%%%%%%

% Grab the required data
stripperUnderflow = logData(:,stripperUnderflow_col);
prodRate_SP       = logData(:,prodRate_SP_col);
% Calculate the ±5% error bounds
hi_prodRate_SP = prodRate_SP + (prodRate_SP * 0.05);
lo_prodRate_SP = prodRate_SP - (prodRate_SP * 0.05);
% Y-Axis autoscaling around max and min setpoint values
max_SP = max(hi_prodRate_SP) + 0.5;
min_SP = min(lo_prodRate_SP) - 0.5;
% Construct the production plot
subplot(1,2,1);
plot(t, stripperUnderflow);
line('XData',t,'YData',prodRate_SP,'Color','k','linestyle','--');
line('XData',t,'YData',lo_prodRate_SP,'Color','k','linestyle','--');
line('XData',t,'YData',hi_prodRate_SP,'Color','k','linestyle','--');
title('Production Rate');
ylabel('Stipper Underflow (m^3/hr)');
xlabel('time (hours)');
axis([0,t_len,min_SP,max_SP]);
grid on;
grid minor;

%%%%%%%%%%%%%%%%%%%%%%%%%
% mol % of Product Plot %
%%%%%%%%%%%%%%%%%%%%%%%%%

% Grab the required data
product_D =  logData(:, product_D_col);
product_E =  logData(:, product_E_col);
product_F =  logData(:, product_F_col);
product_G =  logData(:, product_G_col);
product_H =  logData(:, product_H_col);
pctG_SP   =  logData(:, pctG_SP_col);
% Calculate the ±5 error bounds for G
hi_pctG_SP = pctG_SP + 5.;
lo_pctG_SP = pctG_SP - 5;
% Construct the mol % plot
subplot(1,2,2);
plot(t,product_D,t,product_E,t,product_F,t,product_G,t,product_H);
line('XData',t,'YData',pctG_SP,'Color','k','linestyle','--');
line('XData',t,'YData',hi_pctG_SP,'Color','k','linestyle','--');
line('XData',t,'YData',lo_pctG_SP,'Color','k','linestyle','--');
title('mol % of Production');
ylabel('mol %');
xlabel('time (hours)');
axis([0,t_len,0,100]);
legend( 'mol % of D in Product', ...
        'mol % of E in Product', ...
        'mol % of F in Product', ...
        'mol % of G in Product', ...
        'mol % of H in Product', ...
        'location','northeast');
grid on;
grid minor;
