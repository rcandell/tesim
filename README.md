# Tennessee Eastman HIL Simulator
This repository contains code that allows the Tennesse Eastman (TESIM) 
chemical process model to be simulated using hardware-based simulator 
architectures.  Using the code within this repository will allow for 
integration with external sensor and actuator equipment.  The TESIM simulator 
is intended for research purposes.

## Repository Structure
* c:        Includes the C++ simulator code
* cad:      Includes related CAD diagrams 
* analysis: Includes scripts used for analysis of simulator output

## C++ Code
The C++ code is written for execution on a Windows platform.  Installation of 
Microsoft Visual Studio is required to make use of the TESIM project structure.
The C++ Solution is structured as follows:
* tesimlib:         Library used for the TESIM model
* tesim:            The TESIM executable which runs without HIL integration
* tesim2plc:        The TESIM executable which runs with ADS Comms (Beckhoff) integration
* rtclient:         Executable allowing run-time modification of set-points and control parameters
* testsignals:      Executable to inject ramp and pulse train test signals
* testsignals2plc:  Executable to inject ramp and pulse train test signals with ADS Comms (Beckhoff) integration





