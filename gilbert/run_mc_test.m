% RUN_MC_TEST
%   This funciton serves as the top level driver to mc_tesim(...).
%   Use this script as a template for running simulations.
%
% Rick Candell, 2014

clear variables
format compact

global IDVspec
global P
global R

% specify the Pvec and Rvec for the Gilbert channel model
Pvec = 0:0.1:1.0;
Rvec = [1:-0.1:0.1 0.08:-0.01:0];
% Rvec = 0.1:-0.01:0;

% specify the disturbances
IDVmat = [eye(12) zeros(12,8)];
IDVmat = [IDVmat; [full(round(sprand(4,12,0.5))) zeros(4,8)]];

% Run the simulations
diary off
diary('results_gilbert_iterative/simulation_data2.log');
mc_tesim('results_gilbert_iterative', IDVmat, Pvec, Rvec);
diary off

