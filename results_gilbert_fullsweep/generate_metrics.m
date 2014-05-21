function generate_metrics(results_dir, base_results_file, results_file_mask)
% GENERATE_METRICS(results_dir, base_results_file, results_file_mask)
%   This funciton generates a metrics file in each idv_N directory
%   Each idv_N directory must contain the results of the parametric sweep
%   of Gilbert channel mode state transition probabilities P and R.
%
% Rick Candell, 2014


    % specify 
    Nxmeas = 41; % use all xmeas variables
    xmeas_idx = 1:Nxmeas; 
    
    if nargin < 3
        results_file_mask = '*.mat';
    end
    
    if nargin < 2
        error 'base results filename must be specified'
    end
    
    if nargin < 1
        results_dir = '.';
    end
    output_file = 'metrics.csv';
    
    % load the base IDV results
    if ~exist(results_dir,'dir')
        error 'results directory does not exist'
    end
    load([results_dir '\' base_results_file])
    BASE=RESULTS;
    Lxmeas = size(BASE.xmeas,1);
    clear RESULTS

    % loop through all files in the results directory
    mat_files = dir([results_dir '\' results_file_mask]);
    kk = 1;
    for ii=1:length(mat_files)
        this_file = mat_files(ii);
        if strcmp(this_file.name,base_results_file)
            continue
        end
        load(this_file.name);
        
        % Gilbert channel probabilities
        P = RESULTS.P;
        R = RESULTS.R;
%         IDVnum = bi2de(RESULTS.IDVspec,'right-msb');
        IDVnum = RESULTS.IDVspec*pow2(0:19)';
             
        % check if the plant shutdown
        thisShutdown = false;
        thisLxmeas = size(RESULTS.xmeas,1);
        if size(BASE.xmeas,1) ~= thisLxmeas
            thisShutdown = true;
        end
        
        % calculate the deviation from base case
        if ~thisShutdown
            thisDevs = BASE.xmeas - RESULTS.xmeas;
        else
            thisDevs = BASE.xmeas(1:thisLxmeas,:) - RESULTS.xmeas;
        end
        
        % measure the statistical deviations from base case measured
        % variables
        thisMaxDevs = max(thisDevs, [], 1);
        thisVarDevs = var(thisDevs);
        
        % quality metric
        
        % output vector for statistical quantities
        %  TODO: Consider only the first three hours for the transient rsp
        corr_metrics = zeros(1,Nxmeas);
        for jj = 1:Nxmeas
            % correlation between base case and the measured variable
            thisCorr = corr(BASE.xmeas(1:thisLxmeas,jj),RESULTS.xmeas(:,jj));
            corr_metrics(jj) = thisCorr;
        end
        
        % metrics for baseline operating cost deviations
        thisOpCostCorr = corr(BASE.OpCost(1:thisLxmeas),RESULTS.OpCost);     
        thisOpCostMaxDev = max(BASE.OpCost(1:thisLxmeas) - RESULTS.OpCost);
        
        % metrics for quality compared to baseline
        thisQualityPctGspCorr = corr(BASE.Quality.signals.values(1:thisLxmeas,2),RESULTS.Quality.signals.values(:,2));     
        thisQualityPctGspMaxDev = max(BASE.Quality.signals.values(1:thisLxmeas,2) - RESULTS.Quality.signals.values(:,2));
        thisQualityPctGspVar = var(BASE.Quality.signals.values(1:thisLxmeas,2) - RESULTS.Quality.signals.values(:,2));
        
        % write the stats to the output file
        if kk == 1
            writeHdr(output_file);
            dlmwrite(output_file, [P R IDVnum thisMaxDevs thisVarDevs corr_metrics thisOpCostCorr thisOpCostMaxDev thisQualityPctGspCorr thisQualityPctGspMaxDev thisQualityPctGspVar],'delimiter',',','precision','%0.3f','-append');
        else
            dlmwrite(output_file, [P R IDVnum thisMaxDevs thisVarDevs corr_metrics thisOpCostCorr thisOpCostMaxDev thisQualityPctGspCorr thisQualityPctGspMaxDev thisQualityPctGspVar],'delimiter',',','precision','%0.3f','-append');
        end
        
        kk = kk + 1;

    end
    
end % function

function writeHdr(output_file)
    domain_names = {'P','R','IDVnum'};
    max_names =  strcat({'MAXDEV from '}, getXMeasNames(1:41));
    var_names =  strcat({'VAR from '}, getXMeasNames(1:41));
    corr_names = strcat({'CORR of '}, getXMeasNames(1:41));
    opcost_names = {'Corr_to_OpCost', 'MaxDev_from_OpCost'};
    quality_name = {'Corr_to_PctGsp', 'MaxDev_from_PctG', 'Var_from_PctG'};
    hdr = strjoin([domain_names max_names var_names corr_names opcost_names quality_name],',');
    fid = fopen(output_file,'w');
    fprintf(fid,[hdr '\n']);
    fclose(fid);
end

function names = getXMeasNames(idx)
    names = { ...
        'A feed stream 1', ...
        'D feed stream 2', ...
        'E feed stream 3', ...
        'A and C feed stream 4', ...
        'Recycle flow Stream 8', ...
        'Reactor feed rate stream 6', ...
        'Reactor pressure', ...
        'Reactor level', ...
        'Reactor temperature', ...
        'Purge rate stream 9', ...
        'Product separator temperature ', ...
        'Product separator level', ...
        'Product separator pressure', ...
        'Product separator underflow stream 10', ...
        'stripper level', ...
        'stripper pressure', ...
        'Stripper underflow stream 11', ...
        'stripper temperature', ...
        'strippx steam dew', ...
        'Compressor work', ...
        'Reactor cooling water outlet temperature', ...
        'separator cooling water outlet temperature', ...
        'Reactor feed A', ...
        'Reactor feed B', ...
        'Reactor feed C', ...
        'Reactor feed D', ...
        'Reactor feed E', ...
        'Reactor feed F', ...
        'Purge gas A', ...
        'Purge gas B', ...
        'Purge gas C', ...
        'Purge gas D', ...
        'Purge gas E', ...
        'Purge gas F', ...
        'Purge gas G', ...
        'Purge gas H', ...
        'Product D', ...
        'Product E', ...
        'Product F', ...
        'Product G', ...
        'Product H' };
    names = names(idx);
end




