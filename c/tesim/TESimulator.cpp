
#include "TESimulator.h"

TESimulator* TESimulator::instance = 0;

TESimulator* TESimulator::getInstance()
{
	if (TESimulator::instance == 0)
	{
		TESimulator::instance = new TESimulator;
	}
    return TESimulator::instance;
}

TESimulator::~TESimulator()
{
	delete TESimulator::instance;
	TESimulator::instance = 0;
}

// initialize the simulation
void TESimulator::initialize(const double ts_ode, const double ts_plant, const double ts_ctlr)
{
	
}

// increment both plant and controller one time step
void TESimulator::increment()
{
	
}

// run the plant one time step
void TESimulator::increment_plant()
{
	
}

// run the controller one scan interval
void TESimulator::increment_controller()
{
	
}

// validates that tstep is integer multiple of ode time step
void TESimulator::valid_times(const double ts_plant, const double ts_ctlr) throw (...)
{
	
}


