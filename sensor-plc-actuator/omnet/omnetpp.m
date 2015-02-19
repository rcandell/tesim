function omnetpp( )
%OMNETPP Summary of this function goes here
%   Detailed explanation goes here

status = copyfile('xmeas_out.txt', 'xmeas_in.txt');
if ~status
    error 'problem copying xmeas output to input'
end

status = copyfile('xmv_out.txt', 'xmv_in.txt');
if ~status
    error 'problem copying xmv output to input'
end

end

