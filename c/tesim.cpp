/*

* Author: Rick Candell (rick.candell@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License: Public Domain

*/
// tesim.cpp : Defines the main() entry point for the console application.
//

#include "TEPlant.h"
#include "TEController.h"
#include <iostream>

int main(int argc, char* argv[])
{
	TEPlant* teplant = TEPlant::getInstance();
	TEController* tectlr = TEController::getInstance();

	int nsteps = 10000;
	double t, tstep, tscan;
	double *xmeas, *xmv;
	t = 0;
	tstep = 1. / 3600.;

	// init the plant
	teplant->initialize();
	xmeas = (double*)(teplant->get_xmeas());

	// init the controller
	tscan = 1. / 3600.; 
	tectlr->initialize();
	for (int ii = 0; ii < nsteps; ii++)
	{
		// network transmits xmeas data
		// ...

		// run the controller
		xmv = tectlr->increment(t, tscan, xmeas);

		// network transmits xmv data
		// ...

		// run the plant
		xmeas = teplant->increment(t, tstep, xmv);
		std::cout << *teplant << std::endl;
		//std::cout << teplant->get_xmeas(12) << std::endl;

		// advance the time step
		t += tstep;
	}
	return 0;
}

