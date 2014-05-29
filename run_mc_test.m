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
% Rvec = 1:-0.1:0.1;
Rvec = [0.1 0.12 0.14 0.16 0.18 0.2:0.1:0.9];
rand('state', 4);
IDVmat = [eye(12) zeros(12,8)];
IDVmat = [IDVmat; [full(round(sprand(4,12,0.5))) zeros(4,8)]];
% spy(IDVmat)
% mc_tesim('results_gilbert_fullsweep', IDVmat, Pvec, Rvec);
mc_tesim('results_gilbert_test', IDVmat, Pvec, Rvec);

