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
#include <math.h> 

void print_sim_params(double tstep, double tscan, int nsteps, int steps_per_scan, double simtime)
{
	std::cout << "TE simulation" << std::endl;
	std::cout << "simulation time: " << simtime << " hrs" << std::endl;
	std::cout << "plant dt: " << tstep << " hrs" << std::endl;
	std::cout << "ctlr dt: " << tscan << " hrs" << std::endl;
	std::cout << "steps per scan: " << steps_per_scan << std::endl;
	std::cout << "time steps: " << nsteps << std::endl;
}

int main(int argc, char* argv[])
{
	// simulation parameters
	double simtime = 0.0;
	int ilog = 1;
	double t, tstep, tscan;
	double *xmeas, *xmv;
	t = 0;

	// Variables 'tstep' and 'tscan' contain a floating-point round-off error.
	// Scrutinize any calculations that use these variables.
	tstep = (10.0E-3) / 3600;		// Plant update time in hours (10 milliseconds)
	tscan = 0.0005;				// PLC scan time in hours (1.8 seconds, same as Ricker)
	if (argc >= 5)
	{
		tscan = atof(argv[4]);
	}
	if (argc >= 4)
	{
		tstep = atof(argv[3]);
	}
	if (argc < 2)
	{
		std::cerr << "tesim usage error" << std::endl;
		std::cerr << "usage: tesim <sim_time_in_hours> <plant_save_decimator> <tstep - optional> <tscan - optional>" << std::endl;
		exit(0);
	}
	else
	{
		simtime = atof(argv[1]);
		//ilog = atoi(argv[2]);
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

	// derived simulation parameters
	int nsteps = int(simtime/tstep);
	int steps_per_scan = (int)round(tscan / tstep);
	print_sim_params(tstep, tscan, nsteps, steps_per_scan, simtime);

	// init the controller
	tectlr->initialize(tscan);
	xmv = (double*)(tectlr->get_xmv());

	// init the plant
	teplant->initialize();
	xmeas = (double*)(teplant->get_xmeas());

	// start console time log

	for (int ii = 0; ii < nsteps; ii++)
	{
		// increment the plant and controller
		xmeas = teplant->increment(t, tstep, xmv);

		// run the controller if time is at a scan boundary
		if (!(ii%steps_per_scan))
		{
			xmv = tectlr->increment(t, tscan, xmeas);

			// log plant and controller data to file when the controller runs
			ctlr_log << t << "\t" << *tectlr << std::endl;
			plant_log << t << "\t" << *teplant << std::endl;
		}

		// log current time to console
		if (!(ii % (1000*steps_per_scan)))
		{ 
			std::cout << "\r" << "time: " << std::setprecision(3) << t << " hours            "; 
		}

		// Increment to the next time step
		// Approximation of tstep because of limited memory causes errors to 
		// integrate over time (round-off error), so we must recalculate t on 
		// every iteration.
		t = (double)(ii + 1) * tstep;
	}

	return 0;
}

