// tesim.cpp : Defines the entry point for the console application.
//

#include "TESimulator.h"

int main(int argc, char* argv[])
{
	TESimulator* tesim = TESimulator::getInstance();

	int nsteps = 10000;
	double t, tstep;
	double *xmeas, *xmv;
	t = 0;
	tstep = 1. / 3600.;
	tesim->initializePlant();
	tesim->initializeController();
	for (int ii = 0; ii < nsteps; ii++)
	{
		// get the measured data
		xmeas = (double*)(tesim->get_xmeas());

		// network transmits xmeas data
		// ...

		// run the controller
		xmv = tesim->increment_controller(xmeas);

		// network transmits xmv data
		// ...

		// run the plant
		tesim->increment_plant(t, xmv);

		// advance the time step
		t += tstep;
	}
	return 0;
}

