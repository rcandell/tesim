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

void print_sim_params(double tstep, double tscan, int nsteps, double simtime)
{
	std::cout << "TE simulation" << std::endl;
	std::cout << "simulation time: " << simtime << std::endl;
	std::cout << "plant dt: " << tstep << std::endl;
	std::cout << "ctlr dt: " << tscan << std::endl;
	std::cout << "time steps: " << nsteps << std::endl;
}

int main(int argc, char* argv[])
{
	double simtime = 0.0;
	if (argc < 2)
	{
		printf("usage error.  simulation duration not specified");
		exit(0);
	}
	else
	{
		simtime = atof(argv[1]);
	}
	TEPlant* teplant = TEPlant::getInstance();
	TEController* tectlr = TEController::getInstance();

	// Create the log files
	std::ofstream plant_log;
	std::ofstream ctlr_log;
	plant_log.open("teplant.dat");
	ctlr_log.open("tectlr.dat");

	// simulation parameters
	double t, tstep, tscan;
	double *xmeas, *xmv;
	t = 0;
	tstep = 0.0005;
	tscan = tstep;
	int nsteps = int(simtime/tstep);
	print_sim_params(tstep, tscan, nsteps, simtime);


	// init the plant
	teplant->initialize();
	xmeas = (double*)(teplant->get_xmeas());
	plant_log << t << "\t" << *teplant << std::endl;

	// init the controller
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

