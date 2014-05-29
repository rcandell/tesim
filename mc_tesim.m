function mc_tesim(output_dir, IDVmat, Pvec, Rvec)
% FUNCITON MC_TESIM(output_dir, IDVmat, Pvec, Rvec)
% This funciton runs a parametric sweep of the TE simulator.  The function
% sweeps across vectors of disturbances and channel probabilities, P and R.
% Results of the simulation are stored in "idv_NN" directories in the
% folder specified by the caller.
% 
%  R. Candell
%  5/2/2014

% setup globals
global IDVspec
global P
global R
global Ts_base
global Ts_scan
global Ts_save

% R vector
if nargin < 4
    Rvec = 0.1:0.05:0.25;    
end

% P vector
if nargin < 3
    Pvec = 0.0:0.05:0.5;
end

% IDV vector
if nargin < 2
    IDVmat = eye(20);
    IDVmat = [zeros(1,20); IDVmat];
end
Ndist = size(IDVmat,1);

if nargin < 1
    output_dir = '.';
end

% create output directory
if ~exist(output_dir,'dir')
    mkdir(output_dir);
end

% the current date string
thedatestr = datestr(now,'dd-mmm-yyyy_HHMMSS');

% loop through the distrurbances (IDV)
for ii = 1:Ndist
    
    % create output directory
    output_dir_path = [output_dir '\idv_' num2str(ii)];
    if ~exist(output_dir_path,'dir')
        mkdir(output_dir_path);
    end
    
    IDVspec = IDVmat(ii,:);  %ORIG: [ 0 0 0 0 0 0 0 1 zeros(1,12)]
    for jj = 1:length(Pvec)

        P = Pvec(jj);       

        for kk = 1:length(Rvec)
            
            R = Rvec(kk);
            
            if (P==0  && R==1) || (P~=0)
                % initialize the results structure     
                RESULTS = results_struct();

                % run the simulation
                sim('MultiLoop_mode1.slx');

                % save the results to file
                RESULTS.P = P;
                RESULTS.R = R;
                RESULTS.IDVspec = IDVspec;
                RESULTS.Ts_base = Ts_base;
                RESULTS.Ts_scan = Ts_scan;
                RESULTS.Ts_save = Ts_save;
                RESULTS.link_status.sensors = link_status_sensors;
                RESULTS.link_status.actuators = link_status_actuators;
                RESULTS.xmeas = xmeas;
                RESULTS.xmv = xmv;
                RESULTS.idv = idv;
                RESULTS.OpCost = OpCost;
                RESULTS.Production = Production;
                RESULTS.Quality = Quality;
                
                RESULTS_FILENAME ...
                    = [output_dir_path '\results_IDV(' ...
                    num2str(ii) ')_P(' num2str(P) ')_R(' num2str(R) ').mat'];             
                save(RESULTS_FILENAME, 'RESULTS');

                if jj==1 && kk==1  % first iteration for disturbance type
                    save([output_dir_path '\BASELINE.mat'], 'RESULTS');
                end 
            end
        end
    end
end

function RESULTS = results_struct()
    RESULTS = struct( ...
        'P', [], ...
        'R', [], ...
        'IDVspec', [], ...
        'Ts_base', [], ...
        'Ts_scan', [], ...
        'Ts_save', [], ...
        'xmeas', [], ...
        'xmv', [], ...
        'idv', [], ...
        'OpCost', [], ...
        'Production', [], ...
        'Quality', [] );
    
    