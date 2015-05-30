# Tennessee Eastman C++ ported Simulation
This section of the TEsim repository is devoted to a C++ implementation of the TE challenge problem. We use the Lawrence  Ricker decentralized control algorithm as our baseline.

## Authors
Original: Author: Rick Candell <br>
Co-author: Tim Zimmerman <br>
National Institute of Standards and Technology (NIST) <br>

## C++ code dependencies
This implementation requires the following software tools and libraries in order to compile, link, and run.
* Microsoft Visual Studio 2013 Express.  Every effort has been made to make the code portable to other platforms, but cross-platform functionality has not been validated.
* Boost C++ version 1.57 32 bits.  Other versions may work, but this has not been tested.  The 64 bit variant did not link using VS 2013 Express. 

## Motivation
The original TE plant was written in Fortran 77.  Since that time, other implementations were developed under MATLAB/Simulink with the plant ported to C MEX code using the f2c utility and the controller implemented in Simulink.  This approach worked well for development of control design; however, this repository is intended to support research in cyber-security and wireless in factories using hardware-in-the-loop (HIL) mechanisms.  As such, a more portable code base that does not rely on expensive or difficult to manage software tools is needed.  C++ was chosen to accommodate portability and performance.

## Tasks

- [x] Port the TE plant to C++
- [x] Port the Lawrence Ricker controller to C++
- [x] Breakout TEsim into library and executable projects
- [x] Implement wall clock synchonrization (real-time sync)
- [x] Add support for Gilbert Elliot channel model for all variables.
- [x] Add program options and Monte Carlo support for channel
- [ ] Add Beckhoff ADS interface integration for measured variables.

## Directories
Directory | Purpose
--- | ---
C | Top level for VS solution and library source code
f | The original FORTRAN 
tesimlib | Project directory for *tesimlib*. Does not contain code.
Debug | Build output directory for Debug build type
Release | Build output directory for Release build type
data | Location of output data files and post processing scripts
ipy | Contains IPython notebooks for offline analysis

# Installation Prerequisites and Notes 
* Install Visual Studio 2013 Community Edition (Base VS IDE) <br>
* Install boost c++ 1.57 or later using 32 bit binaries <br>
