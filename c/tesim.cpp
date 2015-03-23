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
#include <iomanip>

void print_sim_params(double tstep, double tscan, int nsteps, double simtime)
{
	std::cout << "TE simulation" << std::endl;
	std::cout << "simulation time: " << simtime << "hrs" << std::endl;
	std::cout << "plant dt: " << tstep << "hrs" << std::endl;
	std::cout << "ctlr dt: " << tscan << "hrs" << std::endl;
	std::cout << "time steps: " << nsteps << std::endl;
}

int main(int argc, char* argv[])
{
	double simtime = 0.0;
	int ilog = 1;
	if (argc < 2)
	{
		std::cerr << "tesim usage error" << std::endl;
		std::cerr << "usage: tesim <swim_time_in_hours> <save_decimator>" << std::endl;
		exit(0);
	}
	else
	{
		simtime = atof(argv[1]);
		ilog = atoi(argv[2]);
	}
	TEPlant* teplant = TEPlant::getInstance();
	TEController* tectlr = TEController::getInstance();

	// Create the log files
	std::ofstream plant_log;
	std::ofstream ctlr_log;
	plant_log.open("teplant.dat");
	plant_log.precision(15);
	ctlr_log.open("tectlr.dat");
	ctlr_log.precision(15);

	// simulation parameters
	double t, tstep, tscan;
	double *xmeas, *xmv;
	t = 0;
	tstep = 0.0005;		// in hours
	tscan = tstep;		// in hours
	int nsteps = int(simtime / tstep);
	print_sim_params(tstep, tscan, nsteps, simtime);

	// init the controller
	tectlr->initialize(tstep, tscan);
	xmv = (double*)(tectlr->get_xmv());
	//ctlr_log << t << "\t" << *tectlr << std::endl;

	// init the plant
	teplant->initialize();
	xmeas = (double*)(teplant->get_xmeas());
	//plant_log << t << "\t" << *teplant << std::endl;

	// start console time log
	std::cout << std::setprecision(3) << t << " ";

	for (int ii = 0; ii < nsteps; ii++)
	{
		// increment the plant and controller
		xmeas = teplant->increment(t, tstep, xmv);
		xmv = tectlr->increment(t, tscan, xmeas);

		// log the variables
		if (!(ii%ilog))
		{
			plant_log << t << "\t" << *teplant << std::endl;
			ctlr_log << t << "\t" << *tectlr << std::endl;
		}

		// log current time to console
		if (!(ii % 5000)) { std::cout << std::setprecision(3) << t << " "; }

		// Increment the time
		t += tstep;
	}

	return 0;
}

