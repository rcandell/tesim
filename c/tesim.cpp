/*

* Author:       Rick Candell (rick.candell@nist.gov)
*               Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/
// tesim.cpp : Defines the main() entry point for the console application.
//

#include "TEPlant.h"
#include "TEController.h"
#include <fstream>
#include <iostream>

dbl last_tMod = 0;

int main(int argc, char* argv[])
{
	TEPlant* teplant = TEPlant::getInstance();
	TEController* tectlr = TEController::getInstance();

	// Create the log file
	#ifndef _DEBUG
	std::ofstream logFile;
	logFile.open("xmeasured_outputs.log");
	#endif

	int nsteps = 72 * 2000;
	double t, tstep, tscan;
	double *xmeas, *xmv;
	t = 0;
	tstep = 0.0005;
	//tstep = 1. / 3600.;

	// init the plant
	teplant->initialize();
	xmeas = (double*)(teplant->get_xmeas());

	// init the controller
	tscan = 1. / 3600.;
	tectlr->initialize(tstep, tscan);
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
		//std::cout << *teplant << std::endl;
		dbl tMod = fmod(t, 0.01);
		if ((last_tMod > tMod) || t == 0) {
			#ifndef _DEBUG
			logFile << t << "\t" << *teplant << *tectlr << std::endl;
			#endif
		}
		last_tMod = tMod;
		//std::cout << teplant->get_xmeas(12) << std::endl;

		// advance the time step
		t += tstep;
	}
	#ifndef _DEBUG
	logFile.close();
	#endif

	return 0;
}

