

file_ext = 'xmeas_chan.dat';
files = dir(['*' file_ext]);
nfiles = length(files);
xmeas_id = ;

for ii=1:nfiles
    disp(files(ii).name)
    x = dlmread(files(ii).name,'\t');
    plot(x(:,2*xmeas_id)), hold on
    drawnow
end

hold off




