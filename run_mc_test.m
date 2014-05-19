
% Link loss analysis of the TE simulator
%  R. Candell
%  5/2/2014

clear variables

format compact

global IDVspec
global P
global R

% specify the Pvec and Rvec for the Gilbert channel model
Pvec = [0 0.2:0.1:1.0];
Rvec = [1 0.08:0.01:0.2];
rand('state', 4);
IDVmat = [eye(12) zeros(12,8)];
IDVmat = [IDVmat; [full(round(sprand(4,12,0.5))) zeros(4,8)]];
% spy(IDVmat)
mc_tesim('results_gilbert', IDVmat, Pvec, Rvec);

