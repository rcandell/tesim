function mc_tesim(output_dir, IDVmat, Pvec, Rvec)
% FUNCITON MC_TESIM(output_dir, IDVmat, Pvec, Rvec)
% This funciton runs a parametric sweep of the TE simulator.  The function
% sweeps across vectors of disturbances and channel probabilities, P and R.
% Results of the simulation are stored in "idv_NN" directories in the
% folder specified by the caller.
%
% Args
%   Pvec must be specified in ascending order starting from zero inclusive
%   Rvec must be specified in descending order stating from one inclusive
%   IDVmat is a matrix of row-wise disturbance vectors specified in
%   accordance with the Downs and Vogel format [ idv(1) idv(2) .... ]
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

% Number of iterations per scenario
NIterations = 5;

% loop through the distrurbances (IDV)
for ii = 1:Ndist
    
    % create output directory
    output_dir_path = [output_dir '\idv_' num2str(ii)];
    if ~exist(output_dir_path,'dir')
        mkdir(output_dir_path);
    end
    
    IDVspec = IDVmat(ii,:);  %ORIG: [ 0 0 0 0 0 0 0 1 zeros(1,12)]
    for jj = 1:length(Pvec)
        
        base_case_saved = false;
        P = Pvec(jj);       

        for kk = 1:length(Rvec)
            
            R = Rvec(kk);
                
            RESULTS = results_struct(); %repmat(results_struct(),1,NIterations);
            for ll = 1:NIterations

                % if check to only run the baseline case P=0 once
                if (P==0 && kk==1 && ll==1) || (P~=0)

                    disp(['Running scenario P=' num2str(P) ' R=' num2str(R) ' It=' num2str(ll)])

                    % initialize the results structure     
                    RESULTS_t = results_struct();

                    % run the simulation
                    sim('MultiLoop_mode1.slx');

                    % save the results to file
                    RESULTS_t.P = P;
                    RESULTS_t.R = R;
                    RESULTS_t.IDVspec = IDVspec;
                    RESULTS_t.Iteration = ll;
                    RESULTS_t.Ts_base = Ts_base;
                    RESULTS_t.Ts_scan = Ts_scan;
                    RESULTS_t.Ts_save = Ts_save;
                    RESULTS_t.link_status.sensors = link_status_sensors;
                    RESULTS_t.link_status.actuators = link_status_actuators;
                    RESULTS_t.xmeas = xmeas;
                    RESULTS_t.xmv = xmv;
                    RESULTS_t.idv = idv;
                    RESULTS_t.OpCost = OpCost;
                    RESULTS_t.Production = Production;
                    RESULTS_t.Quality = Quality;

                    RESULTS(ll) = RESULTS_t;
                else
                    disp(['      Skipping scenario P=' num2str(P) ' R=' num2str(R) ' It=' num2str(ll)])
                end % IF
            end % Iterations

            if P==0 && (~base_case_saved)  % save baseline case to separate file

                disp(['   Saving base scenario P=' num2str(P) ' R=' num2str(R)])
                save([output_dir_path '\BASELINE.mat'], 'RESULTS');
                base_case_saved = true;

                disp(['   Saving test scenario P=' num2str(P) ' R=' num2str(R)])
                RESULTS_FILENAME ...
                    = [output_dir_path '\results_IDV(' ...
                    num2str(ii) ')_P(' num2str(P) ')_R(' num2str(R) ').mat'];             
                    save(RESULTS_FILENAME, 'RESULTS');                      

            elseif P>0

                disp(['   Saving test scenario P=' num2str(P) ' R=' num2str(R)])
                RESULTS_FILENAME ...
                    = [output_dir_path '\results_IDV(' ...
                    num2str(ii) ')_P(' num2str(P) ')_R(' num2str(R) ').mat'];             
                    save(RESULTS_FILENAME, 'RESULTS');  

            end %IF                  
        end % Rvec
    end % Pvec
end % Ndist

function RESULTS = results_struct()
    RESULTS = struct( ...
        'P', [], ...
        'R', [], ...
        'IDVspec', [], ...
        'Iteration', [], ...
        'Ts_base', [], ...
        'Ts_scan', [], ...
        'Ts_save', [], ...
        'link_status', [], ...
        'xmeas', [], ...
        'xmv', [], ...
        'idv', [], ...
        'OpCost', [], ...
        'Production', [], ...
        'Quality', [] );
    
