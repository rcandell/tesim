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

	// Create the log files
	std::ofstream plant_log;
	std::ofstream ctlr_log;
	plant_log.open("teplant.dat");
	ctlr_log.open("tectlr.dat");

	int nsteps = 100; // 72 * 2000;
	double t, tstep, tscan;
	double *xmeas, *xmv;
	t = 0;
	tstep = 0.0005;

	// init the plant
	teplant->initialize();
	xmeas = (double*)(teplant->get_xmeas());
	plant_log << t << "\t" << *teplant << std::endl;

	// init the controller
	tscan = tstep;
	tectlr->initialize(tstep, tscan);
	xmv = (double*)(tectlr->get_xmv());
	ctlr_log << t << "\t" << *tectlr << std::endl;
	
	for (int ii = 0; ii < nsteps; ii++)
	{
		// increment the plant and controller
		xmeas = teplant->increment(t, tstep, xmv);
		xmv = tectlr->increment(t, tscan, xmeas); 
		t += tstep;

		// log the variables
		plant_log << t << "\t" << *teplant << std::endl;
		ctlr_log << t << "\t" << *tectlr << std::endl;
	}

	return 0;
}

