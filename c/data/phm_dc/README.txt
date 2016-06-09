National Institute of Standards and Technology
Engineering Lab, Intelligent Systems Division
Rick Candell (rick.candell@nist.gov)

This folder contains multiple files intended for use in the PHM data challenge.
Files are organized according to scenario type.  Scenarios include various types
of situations found in real plant environments ranging from stuck valves to 
communication link faults and failures.  The goal of this challenge is to train
machine learning systems to detect what type of fault exists to expedite corrective
measures.

The following scenarios are included:

    Training data set 1:
    The chemical plant is run at steady state with physical disturbances.
    The physical disturbances are defined in Down and Vogel, 1993
    Ten (10) runs of each disturbance type are produced as separate files.
    
    Training data set 2:
    The reactor is run at steady state but with a bad communications link for
    the reactor sensor reading.  A Gilbert Elliot channel model is used to model the link's
    behavior.

    Training data set 3:
    The reactor is run at steady state but with a bad communications link for
    the reactor cooling water temperature reading.  A Gilbert Elliot channel model
    is used to model the link's behavior.

    Training data set 4:
    The reactor is run at steady state but with a bad communications link for
    the purge valve command channel.  A Gilbert Elliot channel model is used to model
    the link's behavior.

    Training data set 5:
    The reactor is run at steady state but with a bad communications link for
    the A feed flow command channel.  A Gilbert Elliot channel model is used to model
    the link's behavior.

    Training data set 6:
    The reactor is run at steady state but with a bad communications link for
    the D feed flow command channel.  A Gilbert Elliot channel model is used to model
    the link's behavior.

Files are named as follows:

_meta.dat files describe the parameters used for the simulation

_simlog.data files contain data from the plant perspective.  Note that in the real world, 
one can only see what the controller sees.

_xmeas_chan.dat files contain plant sensor data transmitted from the plant to the controller.  Each 
field is preceded by a communication link status 0 = down, 1 = up.  The link status is informational
only for training purposes and will not be included in the test data.  Networks often do not have 
accurate knowledge of the link status.

_xmv_chan.dat files contain controller command data transmitted from the controller to the plant.  Each 
field is preceded by a communication link status 0 = down, 1 = up.  The link status is informational
only for training purposes and will not be included in the test data.  Networks often do not have 
accurate knowledge of the link status.

The code that was used to produce the data may be found at https://github.com/usnistgov/tesim.git/c
A debug copy of the executable is included as a part of this data package.
The paper describing the chemical process is included in the parent directory. 

