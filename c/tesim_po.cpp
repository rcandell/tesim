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
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "TENames.h"
#include "TEIIDErrorChannel.h"
#include "TEGEErrorChannel.h"
#include "TETimeSync.h"
#include "TEPlant.h"
#include "TEController.h"
#ifdef USE_ADS_IF
#include "TEADSInterface.h"
#endif
#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h> 
#include <utility>

#define XMV_SHMEM_NAME ("xmv_shmem")
#define IDV_SHMEM_NAME ("idv_shmem")

// function prototypes
void print_sim_params(double tstep, double tscan, double simtime, bool rt, pq_pair xmeas_pq, pq_pair xmv_pq);
void log_time_console(unsigned RT, double t);

using namespace boost::interprocess;

typedef struct { int index; double value; } xmv_pair;
typedef struct { int index; int value;    } idv_pair;

struct shm_remove
{
	shm_remove()
	{
		shared_memory_object::remove(XMV_SHMEM_NAME);
		shared_memory_object::remove(IDV_SHMEM_NAME);
	}
	~shm_remove()
	{
		shared_memory_object::remove(XMV_SHMEM_NAME);
		shared_memory_object::remove(IDV_SHMEM_NAME);
	}
};

int main(int argc, char* argv[])
{
	// simulation parameters
	std::string log_file_prefix = "nochan";
	bool append_flag = false;
	bool RT = 0;
	bool use_ads = false;
	bool ads_remote = false;
	bool gechan_on = false;
	bool idv_on = false;
	bool shdmem_on = false;
	double simtime = 0.0;
	double t, tstep, tscan;
	unsigned ksave = 1;
	unsigned idv_idx = 0;
	double *xmeas, *xmv;
	t = 0;

	shm_remove remover;

	// important time steps used by the simulation
	tstep = (10.0E-3) / 3600;		// Plant update time in hours (10 milliseconds)
	tscan = tstep;					// PLC scan time in hours (1.8 seconds, same as Ricker)

	// error channel parameters
	double per = 0.0;
	pq_pair xmeas_pq = std::make_pair(0.0, 1.0);
	pq_pair xmv_pq = std::make_pair(0.0, 1.0);

	// program options
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "print the help message")
		("simtime,s", po::value<double>()->required(), "set the simulation time in hours")
		("tstep,t", po::value<double>(), "set the base time step in hours")
		("tscan,c", po::value<double>(), "set the scan interval in hours")
		("ksave,k", po::value<unsigned>(), "decimation factor for saving trace data")
		("setidv,i", po::value<unsigned>(), "set the idv at index provided (1 - 20)")
		("real-time,r", "run the simulation in real time")
		("per", po::value<double>(&per), "enable iid packet error rate between 0.0 and 1.0.")
		("enable-ge-channel,g", "enable the Gilbert Elliot channel model.  Specify pq parameter with later options.")
		("xmeas-pq", po::value<pq_pair>(), "xmeas burst link status probabilities, (Perror:Precover)")
		("xmv-pq", po::value<pq_pair>(), "xmv burst link status probabilities, (Perror:Precover)")
		("logfile-prefix,p", po::value<std::string>(), "prefix for all of the log files")
		("append-data,a", "append plant data to output file")
		("shared-memory", "xmv and idv variables")
		("enable-ads", "turns on the ADS interface to PLC")
		("ads-remote", "enables remote connection.  remote connection is current hard-coded to 5.20.215.224.1.1")
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

	try {

		if (vm.count("real-time"))
		{
			RT = true;
			if (vm.count("shared-memory"))
			{
				shdmem_on = true;
			}
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

		if (vm.count("ksave"))
		{
			ksave = vm["ksave"].as<unsigned>();
		}

		if (vm.count("setidv"))
		{
			idv_on = true;
			idv_idx = vm["setidv"].as<unsigned>();
		}

		if (vm.count("enable-ads"))
		{
			use_ads = true;
		}

		if (vm.count("ads-remote"))
		{
			ads_remote = true;
		}

		if (vm.count("enable-ge-channel"))
		{
			gechan_on = true;
			if (vm.count("xmeas-pq"))
			{
				xmeas_pq = vm["xmeas-pq"].as<pq_pair>();
			}

			if (vm.count("xmv-pq"))
			{
				xmv_pq = vm["xmv-pq"].as<pq_pair>();
			}
		}

		if (vm.count("logfile-prefix"))
		{
			log_file_prefix = vm["logfile-prefix"].as<std::string>();
		}

		if (vm.count("append-data"))
		{
			append_flag = true;
		}
	}
	catch (po::error& e) {
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

	// create shared memory for control of the plant
	using namespace boost::interprocess;
	shared_memory_object * xmv_shm = 0;
	shared_memory_object * idv_shm = 0;
	if (shdmem_on && RT)
	{
		try
		{
			xmv_shm = new shared_memory_object(open_or_create, XMV_SHMEM_NAME, read_write);
			xmv_shm->truncate(sizeof(xmv_pair));
			std::cout << "xmv shared memory object created" << std::endl;

			idv_shm = new shared_memory_object(open_or_create, IDV_SHMEM_NAME, read_write);
			idv_shm->truncate(sizeof(idv_pair));
			std::cout << "idv shared memory object created" << std::endl;
		}
		catch (interprocess_exception & ex)
		{
			std::cout << "shared memory error: " << ex.what() << std::endl;
			return 1;
		}
	}

	// do we append or create a new log file?
	if (append_flag)
	{
		plant_log.open(log_file_prefix + "_plant.dat", std::fstream::out | std::fstream::app);
		std::cout << "plant data file: " << log_file_prefix << "_plant.dat" << std::endl;
	}
	else
	{
		plant_log.open(log_file_prefix + "_plant.dat");
		plant_log << TENames::plant_all() << std::endl;
	}
	plant_log.precision(15);

	ctlr_log.open(log_file_prefix + "_tectlr.dat");
	ctlr_log.precision(15);

	// if we run in real-time, then create a time synch log
	if (RT)
	{
		time_log.open(log_file_prefix + "_time.dat");
		time_log.precision(15);
		time_log.fill('0');
	}

	if (gechan_on || per>0.0)
	{
		xmeas_chan_log.open(log_file_prefix + "_xmeas_chan.log");
		xmv_chan_log.open(log_file_prefix + "_xmv_chan.log");
	}

#ifdef USE_ADS_IF
	// setup the ads interface
	TEADSInterface ads;
	if (use_ads)
	{
		if (ads_remote)
		{
			AmsAddr plc_addr;
			plc_addr.netId.b[0] = 5;
			plc_addr.netId.b[1] = 20; 
			plc_addr.netId.b[2] = 215; 
			plc_addr.netId.b[3] = 224;
			plc_addr.netId.b[4] = 1; 
			plc_addr.netId.b[5] = 1;
			plc_addr.port = 851;
			ads.connect("G_IO.XMEAS", &plc_addr);
		}
		else
		{
			ads.connect("MAIN.XMEAS", 851);
		}
	}
#endif 

	// plant shutdown indicator
	int shutdown = 0;
	char * plant_msg = NULL;

	// derived simulation parameters
	int nsteps = int(simtime/tstep) + 1;
	int steps_per_scan = (int)round(tscan / tstep);
	print_sim_params(tstep, tscan, simtime, RT, xmeas_pq, xmv_pq);

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
	TEErrorChannel* xmeas_channel = 0;
	TEErrorChannel* xmv_channel = 0;
	if (per > 0.0)
	{
		xmeas_channel = new TEIIDErrorChannel(per, TEPlant::NY, teplant->get_xmeas(), seed_rand);
		xmv_channel = new TEIIDErrorChannel(per, TEPlant::NU, tectlr->get_xmv(), seed_rand);
	}
	else if (gechan_on)
	{
		xmeas_channel = new TEGEErrorChannel(xmeas_pq, TEPlant::NY, teplant->get_xmeas(), seed_rand);
		xmv_channel = new TEGEErrorChannel(xmv_pq, TEPlant::NU, tectlr->get_xmv(), seed_rand);
	}
	
	for (int ii = 0; ii < nsteps; ii++)
	{
		// increment the plant and controller
		try
		{
			// examine shared memory
			if (shdmem_on && RT)
			{
				// first update the disturbance vector
				mapped_region reg_idv(*idv_shm, read_write);
				idv_pair *mem1 = static_cast<idv_pair*>(reg_idv.get_address());
				teplant->idv(mem1->index, mem1->value);
				for (int ii = 0; ii < TEPlant::NIDV; ii++)
				{
					std::cout << (teplant->get_idv())[ii] << ",";
				}
				std::cout << std::endl;

				// apply new control/setpoint updates
				mapped_region reg_xmv(*idv_shm, read_write);
				xmv_pair * mem2 = static_cast<xmv_pair*>(reg_xmv.get_address());
				tectlr->set_xmv(mem2->index, mem2->value);
				std::cout << "xmv " << mem2->index << "set to " << tectlr->get_xmv(mem2->index) << std::endl;
			}

			// set the disturbance
			if (idv_on)
			{
				teplant->idv(idv_idx-1);
			}

			// increment the plant
			xmeas = teplant->increment(t, tstep, xmv, &shutdown);

			// send the measured variables to the PLC
#ifdef USE_ADS_IF
			if (use_ads)
			{
				ads.write(xmeas);
			}
#endif

			// apply the sensors channel
			if (per > 0.0)
			{
				xmeas = *static_cast<TEIIDErrorChannel*>(xmeas_channel)+xmeas;
				xmeas_chan_log << *static_cast<TEIIDErrorChannel*>(xmeas_channel) << std::endl;
			}
			else if (gechan_on)
			{
				xmeas = *static_cast<TEGEErrorChannel*>(xmeas_channel) + xmeas;
				xmeas_chan_log << *static_cast<TEGEErrorChannel*>(xmeas_channel) << std::endl;
			}
		}
		catch (TEPlant::ShutdownException& e)
		{
			plant_log << xmeas_pq << "\t" << xmv_pq << "\t" << t << "\t" << *teplant 
				<< "\t" << e.m_sd_code << std::endl;
			std::cerr << e << std::endl;
			std::cerr << "ending simulation" << std::endl;
			return 0;
		}

		// run the controller if time is at a scan boundary
		if (!(ii%steps_per_scan))
		{
			xmv = tectlr->increment(t, tscan, xmeas);

			// apply the control channel
			if (per > 0.0)
			{
				xmv = *static_cast<TEIIDErrorChannel*>(xmv_channel)+xmv;
				xmv_chan_log << *static_cast<TEIIDErrorChannel*>(xmv_channel) << std::endl;
			}
			else if (gechan_on)
			{
				xmv = *static_cast<TEGEErrorChannel*>(xmv_channel) + xmv;
				xmv_chan_log << *static_cast<TEGEErrorChannel*>(xmv_channel) << std::endl;
			}

			// log current time to console
			log_time_console(RT, t);
		}

		// log plant and controller data
		if (!(ii%ksave))
		{
			//plant
			plant_log << xmeas_pq << "\t" << xmv_pq << "\t" << t << "\t" << *teplant << "\t" << 0 << std::endl;

			//controller
			ctlr_log << t << "\t" << *tectlr << std::endl;
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

void print_sim_params(double tstep, double tscan, double simtime, bool rt, pq_pair xmeas_pq, pq_pair xmv_pq)
{
	BOOST_LOG_TRIVIAL(info) << std::endl << "TE simulation" << std::endl
	  << "simulation time: " << simtime << " hrs" << std::endl
	  << "plant dt: " << tstep << " hrs" << std::endl
	  << "ctlr dt: " << tscan << " hrs" << std::endl
	  << "Real-time: " << rt << std::endl
	  << "PER (xmeas): " << xmeas_pq << std::endl
	  << "PER (xmv): " << xmv_pq << std::endl;
}

