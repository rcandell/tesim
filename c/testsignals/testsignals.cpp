/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/
// testsignals.cpp : Defines the main() entry point for the console application.
//

// compiled using BOOST 1.61
#include <boost/system/config.hpp>
#include <boost/log/trivial.hpp>
#include <boost/timer/timer.hpp>
#include <boost/program_options.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <utility>

#include <exception>

#include "TETimeSync.h"
#include "TENames.h"
#include "TEPlant.h"

#define SIM_SHMEM_NAME ("sim_shmem")
#define SP_SHMEM_NAME  ("sp_shmem")

using namespace boost::interprocess;

typedef struct { int index; double value; } xmv_pair;
typedef struct { int index; int value; } idv_pair;

struct shm_remove
{
	shm_remove()
	{
		shared_memory_object::remove(SIM_SHMEM_NAME);
		shared_memory_object::remove(SP_SHMEM_NAME);
	}
	~shm_remove()
	{
		shared_memory_object::remove(SIM_SHMEM_NAME);
		shared_memory_object::remove(SP_SHMEM_NAME);
	}
};

int main(int argc, char* argv[])
{
	/*
	Declaration of variables for main line program
	*/
	std::string log_file_prefix = "nochan";
	bool append_flag = false, RT = false, use_ads = false, ads_remote = false,
		gechan_on = false, enable_idv = false, shdmem_on = false, ext_control = false;
	double simtime = 0.0, t = 0.0, tplant = 0.0005, tctlr = 0.0005, tsave;
	unsigned ksave = 20, idv_idx = 0;
	double *xmeas, *xmv;

	// test signal parameters
	unsigned test_type = 1;  // 0 = constant, 1 = ramp, 2 = pulse train
	double ramp_slope = 1.0;
	double ramp_max = 100.0;
	double ramp_min = 0.0;
	double ramp_ic = 0.0;

	// initialize xmeas and xmv
	xmeas = new double[TEPlant::NY];
	xmv = new double[TEPlant::NX];

	// print the simulation parameters
	std::cout << "TE Simulator C++" << std::endl
		<< "  Authors: Rick Candell" << std::endl
		<< "  National Institute of Standards and Technology (NIST)" << std::endl
		<< "  License: Public Domain" << std::endl
		<< "******************************************************" << std::endl << std::endl;

	// program options
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "print the help message")
		("simtime,s", po::value<double>(&simtime)->required(), "set the simulation time in seconds")
		("tplant,t", po::value<double>(&tplant), "set the base time step in seconds")
		("real-time,r", po::bool_switch(&RT)->default_value(false), "run the simulation in real time")

		// test parameters
		("test-type", po::value<unsigned>(&test_type)->default_value(0), "test signal type, 0 = constant, 1 = ramp, 2 = pulse train")
		("ramp-slope", po::value<double>(&ramp_slope)->default_value(1.0), "ramp slope, default (1.0)")
		("ramp-max", po::value<double>(&ramp_max)->default_value(100.0), "ramp max, default (100.0)")
		("ramp-min", po::value<double>(&ramp_min)->default_value(0.0), "ramp min, default (0.0)")
		("ramp-ic", po::value<double>(&ramp_ic)->default_value(0.0), "ramp initial condition, default (0.0)")

		// shared memory
		("shared-memory", po::bool_switch(&shdmem_on)->default_value(false), "xmv and idv variables")
		("external-ctrl", po::bool_switch(&ext_control)->default_value(false), "read xmv from shared memory")
		;

	po::variables_map vm;
	try	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		if (vm.count("help"))
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

	// disable shared memory interface in fast mode
	if (!RT && shdmem_on)
	{
		std::cout << "shared memory mode requires real-time mode" << std::endl;
		return(1);
	}
	if (!(shdmem_on && RT) && ext_control)
	{
		std::cout << "enabling real-time mode and shared memory for external control" << std::endl;
		shdmem_on = true;
		RT = true;
	}
	if (!RT) shdmem_on = false;

	std::cout << "Simulation time : " << simtime << std::endl;
	std::cout << "Tplant:                       " << tplant << std::endl;
	std::cout << "Ksave:                       " << ksave << std::endl;
	std::cout << "log file prefix:             " << log_file_prefix << std::endl;
	std::cout << "Append:                      " << append_flag << std::endl;
	std::cout << "Run RT:                      " << RT << std::endl;
	std::cout << "*****************************" << std::endl;
	std::cout << "test type:                     " << test_type << std::endl;
	std::cout << "Ramp slope:                    " << ramp_slope << std::endl;
	std::cout << "Ramp max:                      " << ramp_max << std::endl;
	std::cout << "Ramp min:                      " << ramp_min << std::endl;
	std::cout << "Ramp I.C.:                     " << ramp_ic << std::endl;

	std::cout << std::endl;

	// Create the log files
	std::ofstream metadata_log;
	std::ofstream sim_log;
	std::ofstream time_log;

	// store meta data
	metadata_log.open(log_file_prefix + "_meta.dat");
	metadata_log << "Simulation time : " << simtime << std::endl;
	metadata_log << "Tplant:                       " << tplant << std::endl;
	metadata_log << "Tctlr:                       " << tctlr << std::endl;
	metadata_log << "Ksave:                       " << ksave << std::endl;
	metadata_log << "log file prefix:             " << log_file_prefix << std::endl;
	metadata_log << "Append:                      " << append_flag << std::endl;
	metadata_log << "Run RT:                      " << RT << std::endl;
	metadata_log << "test type:                   " << test_type << std::endl;
	metadata_log << "Ramp slope:                  " << ramp_slope << std::endl;
	metadata_log << "Ramp max:                    " << ramp_max << std::endl;
	metadata_log << "Ramp min:                    " << ramp_min << std::endl;
	metadata_log << "Ramp I.C.:                   " << ramp_ic << std::endl;
	metadata_log << std::endl;
	metadata_log.close();

	// create shared memory for control of the plant
	using namespace boost::interprocess;
	shared_memory_object * xmv_shm = 0, *idv_shm = 0, *sim_shm = 0, *sp_shm = 0;
	shm_remove remover;
	if (shdmem_on && RT)
	{
		try
		{
			sim_shm = new shared_memory_object(open_or_create, SIM_SHMEM_NAME, read_write);
			sim_shm->truncate(sizeof(double)*TEPlant::NY + sizeof(double)*TEPlant::NU);
			std::cout << "xmeas shared memory object created" << std::endl;
		}
		catch (interprocess_exception & ex)
		{
			std::cout << "shared memory error: " << ex.what() << std::endl;
			return 1;
		}
	}

	// open the simulation log files
	if (append_flag)
	{
		sim_log.open(log_file_prefix + "_simlog.dat", std::fstream::out | std::fstream::app);
		std::cout << "plant data file: " << log_file_prefix << "_simlog.dat" << std::endl;
	}
	else
	{
		sim_log.open(log_file_prefix + "_simlog.dat");
		sim_log << TENames::simlog_all() << std::endl;
	}
	sim_log.precision(15);

	// if we run in real-time, then create a time synch log
	if (RT)
	{
		time_log.open(log_file_prefix + "_time.dat");
		time_log.precision(15);
		time_log.fill('0');
	}

	// derived simulation parameters
	int nsteps = int(simtime / tplant) + 1;

	// auto timer used as a performance profiler
	boost::timer::auto_cpu_timer t_wall_auto;

	// being time synch with wall clock
	TETimeSync tesync;
	tesync.init();

	tsave = tplant*double(ksave);
	double tstep = 0.0;
	if (!RT)
		tstep = 0.001 / 3600;  // default time step for simulation is 1 ms for non-real-time performance
	else
		tstep = tplant / 10;  // default time step for simulation is 1/10 plant for real-time performance

	unsigned long epoch_sim = 0;
	double tplant_next = 0.0, tctlr_next = 0.0, tsave_next = 0.0;
	double Tau = 0.0, dTau = 0.0;
	double ramp_out = ramp_min;
	do
	{
		/***************************************************************************
		*
		* This is the plant section
		*
		****************************************************************************/
		if (t >= tplant_next)
		{
			//
			// Generate the test signal
			//
			ramp_out = ramp_slope*Tau + ramp_min;
			if (ramp_out <= ramp_max)
			{
				ramp_out = ramp_min;
				Tau = 0.0;
			}
			else
			{
				Tau += tplant;
			}
			memcpy(xmeas, &ramp_out, TEPlant::NY);

			try
			{

				if (shdmem_on && RT)
				{
					// copy the xmeas values to shared memory
					mapped_region reg_sim(*sim_shm, read_write);
					double *shm_sim = static_cast<double*>(reg_sim.get_address());
					memcpy(shm_sim, xmeas, sizeof(double)*TEPlant::NY);
					memcpy(shm_sim + TEPlant::NY, xmv, sizeof(double)*TEPlant::NU);
				}


			}
			catch (TEPlant::ShutdownException& e)
			{
				std::cerr << e << std::endl;
				std::cerr << "ending simulation" << std::endl;
				return 0;
			}

		}

		// log current time to console
		double t_print = floor(t * 1000) / 1000;
		std::cout << "\r" << "time: " << std::setprecision(5) << std::setfill('0') << t_print << " hours            ";

		// log plant and controller data
		if (t >= tsave_next)
		{
			tsave_next += tsave;

			//plant
			sim_log
				<< "600D BEEF"
				<< std::endl;

		}

		// try to sync sim time to match wall clock time
		if (RT)
		{
			dbl_sec sim_time_dur(t);
			tesync.sync(sim_time_dur, time_log);
		}

		// Increment to the next time step
		// Approximation of tplant because of limited memory causes errors to 
		// integrate over time (round-off error), so we must recalculate t on 
		// every iteration using a method that truncates the floating point error.
		epoch_sim++;
		t = (double)(epoch_sim)* tstep;

	} while (t <= simtime);

	// destroy the shared memory objects
	if (sim_shm) delete sim_shm;
	if (sp_shm) delete sp_shm;

	std::cout << std::endl;
	return 0;
}


