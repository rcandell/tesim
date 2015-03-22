function omnetpp( )
%OMNETPP This function serves as a stub for the real omnet++ network model
% For our testing purposes, we just copy tesim outputs to tesim inputs.
% Input and outputs are from the Simulink model perspective.
%
% In the federated system, some output values will be delayed or lost by 
% the network depending on numbers of hops, SNR, and other factors.
%
% Author: Rick Candell
% Organization: National Institute of Standards and Technology
%               U.S. Department of Commerce
% License: Public Domain

status = copyfile('xmeas_out.txt', 'xmeas_in.txt');
if ~status
    error 'problem copying xmeas output to input'
end
status = copyfile('xmv_out.txt', 'xmv_in.txt');
if ~status
    error 'problem copying xmv output to input'
end

end % omnet++

