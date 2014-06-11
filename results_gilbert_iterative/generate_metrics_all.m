% GENERATE_METRICSALL
%   This funciton serves as the top level driver to generate_metrics(...).
%   Use this script as a template.
%
% Rick Candell, 2014

clear variables;

for ii = 1:16
    disp(['IDV ' num2str(ii) '...'])
    cd(['idv_' num2str(ii)])
    generate_metrics('.', 'BASELINE.mat', 'results*.mat')
    cd('..')
end

% now import all CSV files into single XLS file at top level
disp('Building Testcase XLS file, one page for all IDVs...')
mkdatabase();

