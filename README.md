TEsim: Tennessee Eastman Simulator with Gilbert-Elliot Channel Network Model
=====

Tennessee Eastman simulation with Gilbert-Elliot channel emulating PLC scan failure due to latency uncertainty introduced by a cyber-security device such as an industrial firewall.

#Summary 

This software package is an implementation of a decentralized controller designed by N. Lawrence Ricker at the University of Washington in 1996.  The plant and controller models are both implemented in Simulink.  Wrapper M-code serves to parametrize the model with network degradation in accordance with a Gilbert-Elliot (GE) channel model.  The GE channel is used to characterize a single firewall between the sensor network and the PLC.   The premise behind using the GE model is that conventional industrial controllers such as a programmable logic controller (PLC) use a scan-based query model for polling the status of sensors.  The more often a PLC scans sensors, the more traffic is introduced to the network between the sensors and the controller, and the more likely the firewall will impact the performance of the controller.  

# Parts of the Model

## Model Model Software

The model consists of the following components:

* LICENSE: License for the software
* README.md: This document.
* ricker96.pdf: The paper describing the decentralized controller design.
* MultiLoop_model1.slx:  The Simulink model. Simulink 2012A or later is required to run the simulation.  
* Mode_1_Init.m: Called by the Simulink model during initialization.
* Mode1xInitial.mat:  Contains initial conditions for the controller loops.
* run_mc_test.m:  Top level script the runs a parametric sweep of the GE channel probabilities.
* mc_tessim: Implements the parametric sweep of the simulink model and stores the results for each iteration in a MAT file.

## Results folder and post-processing scripts

The *results_gilbert* folder is specified by run_mc_test.m and can be modified to store results of more targeted simulations.  The contents of the *results_gilbert* folder include the following:

* generate_metric_all.m:  Top level script that begins the metrics generation process using the idv_N directories created when run_mc_test.m is called.
* generate_metrics.m: calculates the metrics for each IDV folder and stores a results file in CSV format in each IDV folder.
* mkdatabase.m: imports metrics from each IDV and stores the metrics in one Microsoft Excel (XLS) file.   
* results_onetable.xls:  The XLS file produced by mkdatabase.m
* results.accdb: Access database containing all of the metrics.  Access is not required but can be useful for analyzing the results with pivot charts.  MATLAB, Excel, and many other programming languages can be equally useful for analyzing the data.

# Using TEsim

Using TEsim is very simple, but a few steps are required to setup your simulation environment.  

1. MEX build the temexd.c.  As long as your MEX build environment is properly setup, this is performed by executing mex temexd.c.
2. Add the appropriate directories to your MATLAB path.  At a minimum, the root of TEsim must be included in you path.

From there, the simlunk model can be executed directly or run from the run_mc_test.m script.

# Results Folders
* *results_gilbert* contains simulation results and post-processing metrics for a partial sweep of P and R
* *results_gilbert_fullsweep* contains simulation results and post-processing metrics for a full sweep of P and R

## Notes 
* Each run of the TE model can produce large amounts of raw data that is far too large for GIT to handle.  A full P,R sweep produces close to 3 GB of data.  That data is available upon request, but users are encouraged to produce the data themselves by executing simulation in MATLAB.

# Contact Information

Rick Candell
Technical Lead, ICS Security Testbed
Intelligent Systems Division, EL
National Institute of Standards and Technology
301-975-4287
rick.candell@nist.gov

