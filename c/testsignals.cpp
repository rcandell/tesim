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

#include <math.h>

#include "TETimeSync.h"
#include "TENames.h"
#include "TEPlant.h"

#ifdef USE_ADS_IF
#include "TEADSInterface.h"
#endif

#define SIM_SHMEM_NAME ("sim_shmem")
#define SP_SHMEM_NAME  ("sp_shmem")

#define TEST_TYPE_CONST (0)
#define TEST_TYPE_RAMP (1)
#define TEST_TYPE_PULSE (2)

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
	bool append_flag = false, RT = false, use_ads = false, shdmem_on = false;
	double simtime = 0.0, t = 0.0, tplant = 0.1;
	unsigned ksave = 1;
	double *xmeas, *xmv, mbs_xmeas[4];

	// test signal parameters
	unsigned test_type = 1;  // 0 = constant, 1 = ramp, 2 = pulse train
	double ramp_slope = 1.0;
	double signal_max = 100.0;
	double signal_min = 0.0;
	double signal_ic = 0.0;
	double signal_period_sec = 0.0;
	double signal_duty_cycle = 0.0;

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
		("simtime,s", po::value<double>(&simtime)->default_value(600.0), "set the simulation time in seconds")
		("tplant,t", po::value<double>(&tplant)->default_value(1.0), "set the base time step in seconds")
		("real-time,r", po::bool_switch(&RT), "run the simulation in real time")
		//("logfile-prefix,p", po::value<std::string>(&log_file_prefix), "prefix for all of the log files")

		// test type
		("test-type", po::value<unsigned>(&test_type)->default_value(0), "test signal type, 0 = constant, 1 = ramp, 2 = pulse train")

		// signal test parameters
		("signal-slope", po::value<double>(&ramp_slope)->default_value(1.0), "ramp slope, default (1.0)")
		("signal-max", po::value<double>(&signal_max)->default_value(100.0), "signal max, default (100.0)")
		("signal-min", po::value<double>(&signal_min)->default_value(0.0), "signal min, default (0.0)")
		("signal-ic", po::value<double>(&signal_ic)->default_value(0.0), "signal initial condition, default (0.0)")
		("signal-period", po::value<double>(&signal_period_sec)->default_value(10.0), "period in seconds of each signal cycle, default (10.0)")
		("signal-duty-cycle-pct", po::value<double>(&signal_duty_cycle)->default_value(50.0), "duty cycle, default (50.0%)")

		// shared memory
		("shared-memory", po::bool_switch(&shdmem_on)->default_value(false), "xmv and idv variables")

#ifdef USE_ADS_IF
		// ADS interface
		("enable-ads,b", po::bool_switch(&use_ads)->default_value(false), "turns on the ADS interface to PLC")
#endif
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
	if (!RT) shdmem_on = false;

	std::cout << "Simulation time : " << simtime << std::endl;
	std::cout << "Tplant:                      " << tplant << std::endl;
	std::cout << "Ksave:                       " << ksave << std::endl;
	std::cout << "log file prefix:             " << log_file_prefix << std::endl;
	std::cout << "Append:                      " << append_flag << std::endl;
	std::cout << "Run RT:                      " << RT << std::endl;
	std::cout << "test type:                   " << test_type << std::endl;
	std::cout << "Signal slope:                " << ramp_slope << std::endl;
	std::cout << "Signal max:                  " << signal_max << std::endl;
	std::cout << "Signal min:                  " << signal_min << std::endl;
	std::cout << "Signal I.C.:                 " << signal_ic << std::endl;
	std::cout << "Signal period:               " << signal_period_sec << std::endl;
	std::cout << "Duty cycle %:                " << signal_duty_cycle << std::endl;
	std::cout << "Use ADS:                     " << use_ads << std::endl;

	std::cout << std::endl;

	// Create the log files
	std::ofstream metadata_log;
	std::ofstream sim_log;
	std::ofstream mbs_xmeas_log;
	std::ofstream time_log;

	// store meta data
	metadata_log.open(log_file_prefix + "_meta.dat");
	metadata_log << "Simulation time : " << simtime << std::endl;
	metadata_log << "Tplant:                      " << tplant << std::endl;
	metadata_log << "Ksave:                       " << ksave << std::endl;
	metadata_log << "log file prefix:             " << log_file_prefix << std::endl;
	metadata_log << "Append:                      " << append_flag << std::endl;
	metadata_log << "Run RT:                      " << RT << std::endl;
	metadata_log << "test type:                   " << test_type << std::endl;
	metadata_log << "Signal slope:                " << ramp_slope << std::endl;
	metadata_log << "Signal max:                  " << signal_max << std::endl;
	metadata_log << "Signal min:                  " << signal_min << std::endl;
	metadata_log << "Signal I.C.:                 " << signal_ic << std::endl;
	metadata_log << "Signal perdiod:              " << signal_period_sec << std::endl;
	metadata_log << "Signal duty cycle %:         " << signal_duty_cycle << std::endl;
	metadata_log << "Use ADS:                     " << use_ads << std::endl;
	metadata_log << std::endl;
	metadata_log.close();

	// create shared memory for control of the plant
	using namespace boost::interprocess;
	shared_memory_object * xmv_shm = 0, *idv_shm = 0, *xmeas_shm = 0, *sp_shm = 0;
	shm_remove remover;
	if (shdmem_on && RT)
	{
		try
		{
			xmeas_shm = new shared_memory_object(open_or_create, SIM_SHMEM_NAME, read_write);
			xmeas_shm->truncate(sizeof(double)*TEPlant::NY + sizeof(double)*TEPlant::NU);
			std::cout << "xmeas shared memory object created" << std::endl;
		}
		catch (interprocess_exception & ex)
		{
			std::cout << "shared memory error: " << ex.what() << std::endl;
			return 1;
		}
	}


	sim_log.open(log_file_prefix + "_simlog.dat", std::fstream::out);
	sim_log << std::setprecision(3);
	sim_log << std::fixed;
	sim_log << "time\ttau\tsout" << std::endl;

	mbs_xmeas_log.open(log_file_prefix + "_mbslog.dat", std::fstream::out);
	mbs_xmeas_log << std::setprecision(3);
	mbs_xmeas_log << std::fixed;

	// if we run in real-time, then create a time synch log
	if (RT)
	{
		time_log.open(log_file_prefix + "_time.dat");
		time_log.precision(15);
		time_log.fill('0');
	}

#ifdef USE_ADS_IF
	// setup the ads interface
	TEADSInterface ads_xmeas_plant, ads_xmeas_gw;
	if (use_ads)
	{
		AmsAddr plc_addr;
		// 5.20.215.224.1.1
		plc_addr.netId.b[0] = 5;
		plc_addr.netId.b[1] = 20;
		plc_addr.netId.b[2] = 215;
		plc_addr.netId.b[3] = 224;
		plc_addr.netId.b[4] = 1;
		plc_addr.netId.b[5] = 1;
		plc_addr.port = 851;
		ads_xmeas_plant.connect("G_IO.XMEAS", &plc_addr);
		ads_xmeas_gw.connect("G_IO.MBS_XMEAS", &plc_addr);
	}
#endif 


	// derived simulation parameters
	int nsteps = int(simtime / tplant) + 1;

	// auto timer used as a performance profiler
	boost::timer::auto_cpu_timer t_wall_auto;

	// being time synch with wall clock
	TETimeSync tesync;
	tesync.init();

	unsigned long epoch_sim = 0;
	double tplant_next = 0.0, tctlr_next = 0.0, tsave_next = 0.0;
	double Tau = t, dTau = 0.0;
	double signal_out = signal_ic;

	double pulse_width_sec = signal_period_sec * signal_duty_cycle / 100.0;

	do
	{
		/***************************************************************************
		*
		* This is the plant section
		*
		****************************************************************************/
		if (t >= tplant_next)
		{
			// update local sw clock
			Tau = fmod(t, signal_period_sec);

			switch (test_type)
			{
			case TEST_TYPE_RAMP:

				//
				// Generate the ramp signal
				//
				signal_out = ramp_slope*Tau + signal_min;
				break;

			case TEST_TYPE_PULSE:

				//
				// Generate the pulse train signal
				//
				(Tau < pulse_width_sec) ? (signal_out = 1.0) : (signal_out = 0.0);
				break;
			
			case TEST_TYPE_CONST:
			default:
				break;
			} 

			for (int ii = 0; ii < TEPlant::NY; ii++) { xmeas[ii] = signal_out; };

#ifdef USE_ADS_IF
			if (use_ads)
			{
				// send the measured variables to the PLC
				//ads_xmeas_plant.write_lreal(xmeas, TEPlant::NY);
				ads_xmeas_plant.write_value<double>(xmeas, TEPlant::NY);

				// query the ADS interface for xmeas values at the gateway.
				// the ADS data overrides simulated channel
				float mbs_xmeas_gw[4];
				mbs_xmeas_gw[0] = mbs_xmeas_gw[1] = mbs_xmeas_gw[2] = mbs_xmeas_gw[3] = 0;
				ads_xmeas_gw.read_value<float>(mbs_xmeas_gw, 4);
				mbs_xmeas[0] = mbs_xmeas_gw[0];		// Flow: Feed A
				mbs_xmeas[1] = mbs_xmeas_gw[1];		// Flow: Feed D
				mbs_xmeas[2] = mbs_xmeas_gw[2];		// Flow: Feed E
				mbs_xmeas[3] = mbs_xmeas_gw[3];		// Reactor Pressure
			}

#endif

			try
			{

				if (shdmem_on && RT)
				{
					// copy the xmeas values to shared memory
					mapped_region reg_sim(*xmeas_shm, read_write);
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

			// log the plant data
			sim_log << t << "\t" << Tau << "\t" << signal_out << std::endl;
			std::cout << t << "\t" << Tau << "\t" << signal_out << std::endl;

			// log the modbus data
			mbs_xmeas_log << t << "\t" << Tau << "\t" << mbs_xmeas[0] << "\t" << mbs_xmeas[1] << "\t" << mbs_xmeas[2] << "\t" << mbs_xmeas[3] << std::endl;

			// update the time information
			tplant_next += tplant;

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
		t = (double)(epoch_sim)* tplant;

	} while (t < simtime);

	// destroy the shared memory objects
	if (xmeas_shm) delete xmeas_shm;
	if (sp_shm) delete sp_shm;

	std::cout << std::endl;
	return 0;
}


