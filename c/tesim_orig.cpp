/*

* Author:       Rick Candell (rick.candell@nist.gov)
*               Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/
// tesim.cpp : Defines the main() entry point for the console application.
//

#include <boost/system/config.hpp>
#include <boost/timer/timer.hpp>
#include <boost/log/trivial.hpp>

#include "TENames.h"
#include "TEErrorChannel.h"
#include "TETimeSync.h"
#include "TEPlant.h"
#include "TEController.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h> 

// function prototypes
void print_sim_params(double tstep, double tscan, int nsteps, 
	int steps_per_scan, double simtime, unsigned rt, bool echan = false, 
	double per = 0.0, double per_xmv = 0.0);
void log_time_console(unsigned RT, double t);

int main(int argc, char* argv[])
{
	// auto timer used as a performance profiler
	boost::timer::auto_cpu_timer t_wall_auto;

	// simulation parameters
	unsigned RT = 0;
	double simtime = 0.0;
	int ilog = 1;
	double t, tstep, tscan;
	double *xmeas, *xmv;
	t = 0;

	// important time steps used by the simulation
	tstep = (10.0E-3) / 3600;		// Plant update time in hours (10 milliseconds)
	tscan = 0.0005;					// PLC scan time in hours (1.8 seconds, same as Ricker)

	// error channel parameters
	bool ERROR_CHAN = false;
	double PER_XMEAS = 0.0;
	double PER_XMV = 0.0;

	// parse the command line arguments
	if (argc >= 9)
	{
		PER_XMV = atof(argv[8]);
	}
	if (argc >= 8)
	{
		PER_XMEAS = atof(argv[7]);
	}
	if (argc >= 7)
	{
		// apply the error channel
		ERROR_CHAN = true;
	}
	if (argc >= 6)
	{
		// non-zero value will indicate that the simulation will run in real-time
		RT = atoi(argv[5]);
	}
	if (argc >= 5)
	{
		// indicates how often the controller is run
		tscan = atof(argv[4]);
	}
	if (argc >= 4)
	{
		// the integration time step used by the plant
		tstep = atof(argv[3]);
	}
	if (argc < 2)
	{
		std::cerr << "tesim usage error" << std::endl;
		std::cerr << "usage: tesim <sim_time_in_hours> <plant_save_decimator> <tstep - optional> <tscan - optional> <reat-time flag> <use_chan flag> <xmeas per> <xmv per>" << std::endl;
		exit(0);
	}
	else
	{
		// indicates the total simulation duration
		simtime = atof(argv[1]);
		//ilog = atoi(argv[2]);
	}
	TEPlant* teplant = TEPlant::getInstance();
	TEController* tectlr = TEController::getInstance();

	// Create the log files
	std::ofstream plant_log;
	std::ofstream ctlr_log;
	std::ofstream time_log;
	std::ofstream xmeas_chan_log;
	std::ofstream xmv_chan_log;
	plant_log.open("teplant.dat");
	plant_log.precision(15);
	plant_log << TENames::time() << "\t" << TENames::xmv() << "\t" << TENames::xmeas() << "\t" << TENames::idv() << std::endl;

	ctlr_log.open("tectlr.dat");
	ctlr_log.precision(15);

	time_log.open("time.dat");
	time_log.precision(15);
	time_log.fill('0');

	xmeas_chan_log.open("xmeas_chan.log");
	xmv_chan_log.open("xmv_chan.log");

	// plant shutdown indicator
	int shutdown = 0;
	char * plant_msg = NULL;

	// derived simulation parameters
	int nsteps = int(simtime/tstep);
	int steps_per_scan = (int)round(tscan / tstep);
	print_sim_params(tstep, tscan, nsteps, steps_per_scan, simtime, RT, ERROR_CHAN, PER_XMEAS);

	// being time synch with wall clock
	TETimeSync tesync;
	tesync.init();

	// init the controller
	tectlr->initialize(tscan);
	xmv = (double*)(tectlr->get_xmv());

	// init the plant
	teplant->initialize();
	xmeas = (double*)(teplant->get_xmeas());

	// create the communications channels
	int seed_rand = 17;
	TEErrorChannel xmeas_channel(PER_XMEAS, TEPlant::NY, teplant->get_xmeas(), seed_rand);
	TEErrorChannel xmv_channel(PER_XMV, TEPlant::NU, tectlr->get_xmv(), seed_rand);

	for (int ii = 0; ii < nsteps; ii++)
	{
		// increment the plant and controller
		try
		{
			xmeas = teplant->increment(t, tstep, xmv, &shutdown);

			// apply the sensors channel
			xmeas = xmeas_channel + xmeas;

			// log the xmeas channel errors
			xmeas_chan_log << xmeas_channel << std::endl;
		}
		catch (TEPlant::ShutdownException& e)
		{
			std::cerr << e << std::endl;
			std::cerr << "ending simulation" << std::endl;
			return 0;
		}

		// run the controller if time is at a scan boundary
		if (!(ii%steps_per_scan))
		{
			xmv = tectlr->increment(t, tscan, xmeas);

			// apply the control channel
			xmv = xmv_channel + xmv;
			xmv_chan_log << xmv_channel << std::endl;

			// log plant and controller data to file when the controller runs
			ctlr_log << t << "\t" << *tectlr << std::endl;
			plant_log << t << "\t" << *teplant << std::endl;

			// log current time to console
			log_time_console(RT, t);
		}

		// try to sync sim time to match wall clock time
		if (RT)
		{
			dbl_sec sim_time_dur(t * 3600.0);
			tesync.sync(sim_time_dur, time_log);
		}

		// Increment to the next time step
		// Approximation of tstep because of limited memory causes errors to 
		// integrate over time (round-off error), so we must recalculate t on 
		// every iteration using a method that truncates the floating point error.
		t = (double)(ii + 1) * tstep;
	}

	std::cout << std::endl;
	return 0;
}

void log_time_console(unsigned RT, double t)
{
	if (!RT)
	{
		std::cout << "\r" << "time: " << std::setprecision(8) << std::setfill('0') << t << " hours            ";
	}
	else
	{
		// todo: fixed precision problem when logging
		std::cout << "\r" << "time: " << std::setprecision(8) << std::setfill('0') << (t * 3600.0) << " secs            ";
	}
}

void print_sim_params(double tstep, double tscan, int nsteps, int steps_per_scan, double simtime, unsigned rt, bool echan, double per_xmeas, double per_xmv)
{
	BOOST_LOG_TRIVIAL(info) << std::endl << "TE simulation" << std::endl
	  << "simulation time: " << simtime << " hrs" << std::endl
	  << "plant dt: " << tstep << " hrs" << std::endl
	  << "ctlr dt: " << tscan << " hrs" << std::endl
	  << "steps per scan: " << steps_per_scan << std::endl
	  << "time steps: " << nsteps << std::endl
	  << "Real-time: " << rt << std::endl
	  << "Apply channel: " << echan << std::endl
	  << "PER (xmeas): " << per_xmeas << std::endl
	  << "PER (xmv): " << per_xmv << std::endl;
}

