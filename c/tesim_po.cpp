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
#include <boost/program_options.hpp>

#include "TENames.h"
#include "TEErrorChannel.h"
#include "TETimeSync.h"
#include "TEPlant.h"
#include "TEController.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h> 
#include <utility>

// function prototypes
void print_sim_params(double tstep, double tscan, double simtime, bool rt, per_pair per_xmeas, per_pair per_xmv);
void log_time_console(unsigned RT, double t);

int main(int argc, char* argv[])
{

	// program options
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "print the help message")
		("simtime,s", po::value<double>()->required(), "set the simulation time in hours")
		("tstep,t", po::value<double>(), "set the base time step in hours")
		("tscan,c", po::value<double>(), "set the scan interval in hours")
		("real-time,r", "run the simulation in real time")
		("xmeas-per", po::value<per_pair>(), "xmeas burst packet error rate tuple, (Perror:Precover)")
		("xmv-per", po::value<per_pair>(), "xmv purst packet error rate tuple, (Perror:Precover)")
		("logfile-prefix,p", po::value<std::string>(), "prefix for all of the log files")
		;

	po::variables_map vm;
	try	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		if( vm.count("help") )
		{
			std::cout << desc << std::endl;
			return 0;
		}

		po::notify(vm);
	}
	catch (po::error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << desc << std::endl;
		return 0;
	}
	po::notify(vm);

	// simulation parameters
	std::string log_file_prefix = "nochan";
	bool RT = 0;
	double simtime = 0.0;
	double t, tstep, tscan;
	double *xmeas, *xmv;
	t = 0;

	// important time steps used by the simulation
	tstep = (10.0E-3) / 3600;		// Plant update time in hours (10 milliseconds)
	tscan = tstep;					// PLC scan time in hours (1.8 seconds, same as Ricker)

	// error channel parameters
	per_pair per_xmeas = std::make_pair(0.0, 1.0);
	per_pair per_xmv = std::make_pair(0.0, 1.0);

	try {
		if (vm.count("real-time"))
		{
			RT = true;
		}
		if (vm.count("simtime"))
		{
			simtime = vm["simtime"].as<double>();
		}
		if (vm.count("tstep"))
		{
			tstep = vm["tstep"].as<double>();
		}
		if (vm.count("tscan"))
		{
			tscan = vm["tscan"].as<double>();
		}
		else
		{
			tscan = tstep;
		}
		if (vm.count("xmeas-per"))
		{
			per_xmeas = vm["xmeas-per"].as<per_pair>();
		}
		if (vm.count("xmv-per"))
		{
			per_xmv = vm["xmv-per"].as<per_pair>();
		}
		if (vm.count("logfile-prefix"))
		{
			log_file_prefix = vm["logfile-prefix"].as<std::string>();
		}
	} catch (po::error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << desc << std::endl;
		return 0;
	}

	TEPlant* teplant = TEPlant::getInstance();
	TEController* tectlr = TEController::getInstance();

	// Create the log files
	std::ofstream plant_log;
	std::ofstream ctlr_log;
	std::ofstream time_log;
	std::ofstream xmeas_chan_log;
	std::ofstream xmv_chan_log;

	plant_log.open(log_file_prefix+"_plant.dat");
	plant_log.precision(15);
	plant_log << TENames::time() << "\t" << TENames::xmv() << "\t" << TENames::xmeas() << "\t" << TENames::idv() << std::endl;

	ctlr_log.open(log_file_prefix + "_tectlr.dat");
	ctlr_log.precision(15);

	time_log.open(log_file_prefix + "_time.dat");
	time_log.precision(15);
	time_log.fill('0');

	xmeas_chan_log.open(log_file_prefix + "_xmeas_chan.log");
	xmv_chan_log.open(log_file_prefix + "_xmv_chan.log");

	// plant shutdown indicator
	int shutdown = 0;
	char * plant_msg = NULL;

	// derived simulation parameters
	int nsteps = int(simtime/tstep);
	int steps_per_scan = (int)round(tscan / tstep);
	print_sim_params(tstep, tscan, simtime, RT, per_xmeas, per_xmv);

	// auto timer used as a performance profiler
	boost::timer::auto_cpu_timer t_wall_auto;

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
	TEErrorChannel xmeas_channel(per_xmeas, TEPlant::NY, teplant->get_xmeas(), seed_rand);
	TEErrorChannel xmv_channel(per_xmv, TEPlant::NU, tectlr->get_xmv(), seed_rand);

	for (int ii = 0; ii < nsteps; ii++)
	{
		// increment the plant and controller
		try
		{
			xmeas = teplant->increment(t, tstep, xmv, &shutdown);

			// apply the sensors channel
			if (per_xmeas.first > 0.0)
			{
				xmeas = xmeas_channel + xmeas;
				xmeas_chan_log << xmeas_channel << std::endl;
			}
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
			if (per_xmv.first > 0.0)
			{
				xmv = xmv_channel + xmv;
				xmv_chan_log << xmv_channel << std::endl;
			}

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

void print_sim_params(double tstep, double tscan, double simtime, bool rt, per_pair per_xmeas, per_pair per_xmv)
{
	BOOST_LOG_TRIVIAL(info) << std::endl << "TE simulation" << std::endl
	  << "simulation time: " << simtime << " hrs" << std::endl
	  << "plant dt: " << tstep << " hrs" << std::endl
	  << "ctlr dt: " << tscan << " hrs" << std::endl
	  << "Real-time: " << rt << std::endl
	  << "PER (xmeas): " << per_xmeas << std::endl
	  << "PER (xmv): " << per_xmv << std::endl;
}

