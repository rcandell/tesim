
clear variables;

for ii = 1:16
    disp(['IDV ' num2str(ii) '...'])
    cd(['idv_' num2str(ii)])
    generate_metrics('.', 'BASELINE.mat', 'results_*.mat')
    cd('..')
end

% now import all CSV files into single XLS file at top level
mkdatabase();