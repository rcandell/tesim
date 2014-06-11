function mkdatabase()
% mkdatabase()
%   This funciton aggregates the metrics data from all idv_N directories.
%   Data is stored to a an Excel spreasheet which can be imported into the
%   Microsoft Access database, results.accdb.
%
% Rick Candell, 2014

outputxls = 'metrics.xls';

disp('Building XLS file, one page for all DEVs...')
rownum = 2;
for ii = 1:16
    disp(['IDV ' num2str(ii) '...'])
    csvpath = ['idv_' num2str(ii) '\metrics_devs.csv'];
	sheet_name = ['DEV'];
    A=importdata(csvpath);
    if rownum == 2
        names = A.colheaders;
        xlswrite(outputxls,names,sheet_name,'A1');
    end
    values = A.data;
    Nvalues = size(A.data,1);
    xlswrite(outputxls,values,sheet_name, ...
        ['A' num2str(rownum)] );
    rownum = rownum+Nvalues;    
end

disp('Building XLS file, one page for all IERRs...')
rownum = 2;
for ii = 1:16
    disp(['IDV ' num2str(ii) '...'])
    csvpath = ['idv_' num2str(ii) '\metrics_interr.csv'];
	sheet_name = ['INTERR'];
    A=importdata(csvpath);
    if rownum == 2
        names = A.colheaders;
        xlswrite(outputxls,names,sheet_name,'A1');
    end
    values = A.data;
    Nvalues = size(A.data,1);
    xlswrite(outputxls,values,sheet_name, ...
        ['A' num2str(rownum)] );
    rownum = rownum+Nvalues;    
end

end

