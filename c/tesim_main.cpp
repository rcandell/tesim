/*

* Author:       Rick Candell (rick.candell@nist.gov)
*               Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/
// tesim.cpp : Defines the main() entry point for the console application.
//

//#include <boost/system/config.hpp>
#include <boost/timer/timer.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "TENames.h"
#include "TEIIDErrorChannel.h"
#include "TEGEErrorChannel.h"
#include "TEErrorFreeChannel.h"
#include "TETimeSync.h"
#include "TEPlant.h"
#include "TEController.h"
#ifdef USE_ADS_IF
#include "TEADSInterface.h"
#endif
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <utility>

#include <exception>

#define XMV_SHMEM_NAME ("xmv_shmem")
#define IDV_SHMEM_NAME ("idv_shmem")
#define SIM_SHMEM_NAME ("sim_shmem")
#define SP_SHMEM_NAME  ("sp_shmem")

// function prototypes
void print_sim_params(double tplant, double tctlr, double simtime, bool rt, pq_pair xmeas_pq, pq_pair xmv_pq);

using namespace boost::interprocess;

typedef struct { int index; double value; } xmv_pair;
typedef struct { int index; int value;    } idv_pair;

struct shm_remove
{
	shm_remove()
	{
		shared_memory_object::remove(SIM_SHMEM_NAME);
		shared_memory_object::remove(XMV_SHMEM_NAME);
		shared_memory_object::remove(IDV_SHMEM_NAME);
		shared_memory_object::remove(SP_SHMEM_NAME);
	}
	~shm_remove()
	{
		shared_memory_object::remove(SIM_SHMEM_NAME);
		shared_memory_object::remove(XMV_SHMEM_NAME);
		shared_memory_object::remove(IDV_SHMEM_NAME);
		shared_memory_object::remove(SP_SHMEM_NAME); 
	}
};

int main(int argc, char* argv[])
{
	/* 
	Declaration of variables for main line program
	*/
	std::string log_file_prefix = "nochan";
	bool append_flag = false, RT = false, 
		use_ads_all = false, use_ads_xmeas = false, use_ads_xmv = false, use_ads_sp = false, use_ads_any = false,
		gechan_on = false, enable_idv = false, shdmem_on = false, ext_control = false;
	int net_type = 0;
	double simtime = 0.0, t = 0.0, tplant = 0.0005, tctlr = 0.0005, tsave;
	unsigned ksave = 20, idv_idx = 0;
	double *xmeas, *xmv;

	/*
	Set point values.  Default values for setpoints are assigned here.
	tesim allows for override of these setpoints at time t=0 throgh the command line program options.
	*/
	double 
		prod_rate_sp			= 22.89, 
		reactor_pressure_sp		= 2800.0, 
		reactor_level_sp		= 65.0, 
		reactor_temp_sp			= 122.9,
		pctg_sp					= 53.8, 
		sep_level_sp			= 50.0, 
		stripper_level_sp		= 50.0;

	// print the simulation parameters
	std::cout << "TE Simulator C++" << std::endl
		<< "  Authors: Rick Candell, Tim Zimmerman" << std::endl
		<< "  National Institute of Standards and Technology (NIST)" << std::endl
		<< "  License: Public Domain" << std::endl
		<< "******************************************************" << std::endl << std::endl;

	// error channel parameters
	double per = 0.0;
	pq_pair xmeas_pq = std::make_pair(0.0, 1.0);
	unsigned xmeas_ge_link_id = 0, xmv_ge_link_id = 0, xmeas_iid_chan_id = 0, xmv_iid_chan_id = 0;
	pq_pair xmv_pq = std::make_pair(0.0, 1.0);

	// program options
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "print the help message")
		("simtime,s", po::value<double>(&simtime)->required(), "set the simulation time in hours")
		("tplant,t", po::value<double>(&tplant), "set the base time step in hours")
		("tctlr,c", po::value<double>(&tctlr), "set the scan interval in hours")
		("ksave,k", po::value<unsigned>(&ksave), "decimation factor for saving plant trace data")
		("real-time,r", po::bool_switch(&RT)->default_value(false), "run the simulation in real time")
		("logfile-prefix,p", po::value<std::string>(&log_file_prefix), "prefix for all of the log files")
		("append-data,a", po::bool_switch(&append_flag)->default_value(false), "append plant data to output file")

		// disturbances
		("setidv,i", po::value<unsigned>(&idv_idx), "set the idv at index provided (1 - 20)")

		// shared memory
		("shared-memory", po::bool_switch(&shdmem_on)->default_value(false), "xmv and idv variables")
		("external-ctrl", po::bool_switch(&ext_control)->default_value(false), "read xmv from shared memory")

#ifdef USE_ADS_IF
		// ADS interface
		("ads-net-type,o", po::value<int>(&net_type)->required(), "specify network type code")
		("ads-all", po::bool_switch(&use_ads_all)->default_value(false), "turns on ADS interface for all variables")
		("ads-xmeas", po::bool_switch(&use_ads_xmeas)->default_value(false), "turns on ADS interface for the selected XMEAS variables")
		("ads-xmv", po::bool_switch(&use_ads_xmv)->default_value(false), "turns on ADS interface for the selected XMV variables")
		("ads-sp", po::bool_switch(&use_ads_sp)->default_value(false), "turns on ADS interface for the selected Setpoint variables")
#endif

		// packet error rate parameters
		("per", po::value<double>(&per), "Enable iid packet error rate between 0.0 and 1.0.")
		("xmeas-iid-chan-id", po::value<unsigned>(&xmeas_iid_chan_id), "xmeas iid channel link number (1-41). Default is all channels (0).")
		("xmv-iid-chan-id", po::value<unsigned>(&xmv_iid_chan_id), "xmv iid channel link number (1-41). Default is all channels (0).")

		("enable-ge-channel,g", po::bool_switch(&gechan_on)->default_value(false), "Enable the Gilbert Elliot channel model.")
		("xmeas-pq", po::value<pq_pair>(&xmeas_pq), "xmeas GE probs as Perror:Precover")
		("xmeas-ge-chan-id", po::value<unsigned>(&xmeas_ge_link_id), "xmeas GE channel link number (1-41). Default is all channels (0).")
		("xmv-pq", po::value<pq_pair>(&xmv_pq), "xmv GE probs as Perror:Precover")
		("xmv-ge-chan-id", po::value<unsigned>(&xmv_ge_link_id), "xmv GE channel link number (1-41). Default is all channels (0).")

		// set point overrides
		("sp-prod-rate", po::value<double>(&prod_rate_sp),					"change setpoint at Tstart (default: 22.89)")
		("sp-reactor-pressure", po::value<double>(&reactor_pressure_sp),	"change setpoint at Tstart (default: 2800.)")
		("sp-reactor-level", po::value<double>(&reactor_level_sp),			"change setpoint at Tstart (default: 65.0 )")
		("sp-reactor-temp", po::value<double>(&reactor_temp_sp),			"change setpoint at Tstart (default: 122.9)")
		("sp-pctg", po::value<double>(&pctg_sp),							"change setpoint at Tstart (default: 53.8 )")
		("sp-separator-level", po::value<double>(&sep_level_sp),			"change setpoint at Tstart (default: 50.0 )")
		("sp-stripper-level", po::value<double>(&stripper_level_sp),		"change setpoint at Tstart (default: 50.0 )")		
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

	try 
	{
		if (vm.count("setidv"))		
		{ 
			if (idv_idx > 0)
			{
				enable_idv = true;
			}
		}
	}
	catch (po::error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << desc << std::endl;
		return 0;
	}

	// disable shared memory interface in fast mode
	if (!RT && shdmem_on)
	{
		std::cout << "shared memory mode requires real-time mode" << std::endl;
		return(1);
	}
	if ( !(shdmem_on && RT) && ext_control )
	{
		std::cout << "enabling real-time mode and shared memory for external control" << std::endl;
		shdmem_on = true;
		RT = true;
	}
	if (!RT) shdmem_on = false;

	std::cout << "Simulation time : " << simtime << std::endl;
	std::cout << "Tplant:                      " << tplant << " hrs (" << tplant*3600 << " secs)" << std::endl;
	std::cout << "Tctlr:                       " << tctlr  << " hrs (" << tplant*3600 << " secs)" << std::endl;
	std::cout << "Ksave:                       " << ksave << std::endl;
	std::cout << "log file prefix:             " << log_file_prefix << std::endl;
	std::cout << "Append:                      " << append_flag << std::endl;
	std::cout << "Run RT:                      " << RT << std::endl;
	std::cout << "Network type:                " << net_type << std::endl;
	std::cout << "Use ADS xmeas:               " << use_ads_xmeas << std::endl;
	std::cout << "Use ADS xmv:                 " << use_ads_xmv << std::endl;
	std::cout << "Use ADS sp:                  " << use_ads_sp << std::endl;

	std::cout << "IID chan PER:                " << per << std::endl;
	if (xmeas_iid_chan_id > 0)
	std::cout << "  apply to link:             " << xmeas_iid_chan_id << std::endl;
	if (xmv_iid_chan_id > 0)
	std::cout << "  apply to link:             " << xmv_iid_chan_id << std::endl;

	std::cout << "Enabled GE chan:             " << gechan_on << std::endl;
	std::cout << "GE chan xmeas:               " << xmeas_pq << std::endl;
	if (xmeas_ge_link_id > 0)
	std::cout << "  apply to link:             " << xmeas_ge_link_id << std::endl;
	std::cout << "GE chan xmv:                 " << xmv_pq << std::endl;
	if (xmv_ge_link_id > 0)
	std::cout << "  apply to link:             " << xmv_ge_link_id << std::endl;

	std::cout << "Enable IDV:                  " << enable_idv << std::endl;
	std::cout << "IDV index:                   " << idv_idx << std::endl;
	std::cout << "Enable shdmem:               " << shdmem_on << std::endl;
	std::cout << "Use ext. controller:         " << ext_control << std::endl;
	std::cout << "Prod Rate:                   " << prod_rate_sp << (vm.count("sp-prod-rate") ? " override" : "  default") << std::endl;
	std::cout << "Reactor pressure:            " << reactor_pressure_sp << (vm.count("sp-reactor-pressure") ? " override" : "  default") << std::endl;
	std::cout << "Reactor level:               " << reactor_level_sp << (vm.count("sp-reactor-level") ? " override" : "  default") << std::endl;
	std::cout << "Reactor temp:                " << reactor_temp_sp << (vm.count("sp-reactor-temp") ? " override" : "  default") << std::endl;
	std::cout << "Pct G:                       " << pctg_sp << (vm.count("sp-pctg") ? " override" : "  default") << std::endl;
	std::cout << "Sep level:                   " << sep_level_sp << (vm.count("sp-separator-level") ? " override" : "  default") << std::endl;
	std::cout << "Strip level:                 " << stripper_level_sp << (vm.count("sp-stripper-level") ? " override" : "  default") << std::endl;
	std::cout << std::endl;

	// are any ADS flags turned on?
	use_ads_any = use_ads_sp | use_ads_xmeas | use_ads_xmv;

	// enable all ADS interfaces if necessary
	if (use_ads_all)
		use_ads_sp = use_ads_xmeas = use_ads_xmv = true;

	TEPlant* teplant = TEPlant::getInstance();
	TEController* tectlr = TEController::getInstance();

	// Create the log files
	std::ofstream metadata_log;
	std::ofstream sim_log;
	//std::ofstream plant_log;
	//std::ofstream ctlr_log;
	std::ofstream time_log;
	std::ofstream xmeas_chan_log;
	std::ofstream xmv_chan_log;

	// store meta data
	metadata_log.open(log_file_prefix + "_meta.dat");
	metadata_log << "Simulation time : " << simtime << std::endl;
	metadata_log << "Tplant:                       " << tplant << std::endl;
	metadata_log << "Tctlr:                       " << tctlr << std::endl;
	metadata_log << "Ksave:                       " << ksave << std::endl;
	metadata_log << "log file prefix:             " << log_file_prefix << std::endl;
	metadata_log << "Append:                      " << append_flag << std::endl;
	metadata_log << "Run RT:                      " << RT << std::endl;
	metadata_log << "Network type:				  " << net_type << std::endl;
	metadata_log << "Use ADS xmeas:               " << use_ads_xmeas << std::endl;
	metadata_log << "Use ADS xmv:                 " << use_ads_xmv << std::endl;
	metadata_log << "Use ADS sp:                  " << use_ads_sp << std::endl;
	metadata_log << "IID chan per:                " << per << std::endl;
	if (xmeas_iid_chan_id > 0)
		metadata_log << "  apply to link:         " << xmeas_iid_chan_id << std::endl;
	if (xmv_iid_chan_id > 0)
		metadata_log << "  apply to link:         " << xmv_iid_chan_id << std::endl;

	metadata_log << "Enabled GE chan:             " << gechan_on << std::endl;
	metadata_log << "GE chan xmeas:               " << xmeas_pq << std::endl;
	if (xmeas_ge_link_id > 0)
		metadata_log << "  apply to link:         " << xmeas_ge_link_id << std::endl;
	metadata_log << "GE chan xmv:                    " << xmv_pq << std::endl;
	if (xmv_ge_link_id > 0)
		metadata_log << "  apply to link:         " << xmv_ge_link_id << std::endl;

	metadata_log << "Enable IDV:                  " << enable_idv << std::endl;
	metadata_log << "IDV index:                   " << idv_idx << std::endl;
	metadata_log << "Enable shdmem:               " << shdmem_on << std::endl;
	metadata_log << "Use ext. controller:         " << ext_control << std::endl;
	metadata_log << "Prod Rate:                   " << prod_rate_sp << (vm.count("sp-prod-rate") ? " override" : "  default") << std::endl;
	metadata_log << "Reactor pressure:            " << reactor_pressure_sp << (vm.count("sp-reactor-pressure") ? " override" : "  default") << std::endl;
	metadata_log << "Reactor level:               " << reactor_level_sp << (vm.count("sp-reactor-level") ? " override" : "  default") << std::endl;
	metadata_log << "Reactor temp:                " << reactor_temp_sp << (vm.count("sp-reactor-temp") ? " override" : "  default") << std::endl;
	metadata_log << "Pct G:                       " << pctg_sp << (vm.count("sp-pctg") ? " override" : "  default") << std::endl;
	metadata_log << "Sep level:                   " << sep_level_sp << (vm.count("sp-separator-level") ? " override" : "  default") << std::endl;
	metadata_log << "Strip level:                 " << stripper_level_sp << (vm.count("sp-stripper-level") ? " override" : "  default") << std::endl;
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
			idv_shm = new shared_memory_object(open_or_create, IDV_SHMEM_NAME, read_write);
			idv_shm->truncate(sizeof(idv_pair));
			std::cout << "idv shared memory object created" << std::endl;

			xmv_shm = new shared_memory_object(open_or_create, XMV_SHMEM_NAME, read_write);
			xmv_shm->truncate(sizeof(double)*TEPlant::NU);
			std::cout << "xmv shared memory object created" << std::endl;

			sim_shm = new shared_memory_object(open_or_create, SIM_SHMEM_NAME, read_write);
			sim_shm->truncate(sizeof(double)*TEPlant::NY + sizeof(double)*TEPlant::NU);
			std::cout << "xmeas shared memory object created" << std::endl;

			sp_shm = new shared_memory_object(open_or_create, SP_SHMEM_NAME, read_write);
			sp_shm->truncate(sizeof(sp_pair));
			std::cout << "setpoints shared memory object created" << std::endl;
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
	sim_log.precision(6);
	
	// if we run in real-time, then create a time synch log
	if (RT)
	{
		time_log.open(log_file_prefix + "_time.dat");
		time_log.precision(15);
		time_log.fill('0');
	}

	// open the channel log files
	xmeas_chan_log.open(log_file_prefix + "_xmeas_chan.dat");
	xmv_chan_log.open(log_file_prefix + "_xmv_chan.dat");

	// plant shutdown indicator
	int shutdown = 0;
	char * plant_msg = NULL;

	// derived simulation parameters
	int nsteps = int(simtime/tplant) + 1;
	int steps_per_scan = (int)round(tctlr / tplant);
	//print_sim_params(tplant, tctlr, simtime, RT, xmeas_pq, xmv_pq);

	// auto timer used as a performance profiler
	boost::timer::auto_cpu_timer t_wall_auto;

	// being time synch with wall clock
	TETimeSync tesync;
	tesync.init();

	// init the controller
	tectlr->initialize(tctlr);
	xmv = (double*)(tectlr->get_xmv());

	// apply the overrides to the set-points
	// this must be performed after the initialize call for obvious reasons
	if (vm.count("sp-prod-rate"))			{ tectlr->prod_rate_sp(prod_rate_sp); }
	if (vm.count("sp-reactor-pressure"))	{ tectlr->reactor_pressure_sp(reactor_pressure_sp); }
	if (vm.count("sp-reactor-level"))		{ tectlr->reactor_level_sp(reactor_level_sp); }
	if (vm.count("sp-reactor-temp"))		{ tectlr->reactor_temp_sp(reactor_temp_sp); }
	if (vm.count("sp-pctg"))				{ tectlr->pctg_sp(pctg_sp); }
	if (vm.count("sp-separator-level"))		{ tectlr->sep_level_sp(sep_level_sp); }
	if (vm.count("sp-stripper-level"))		{ tectlr->strip_level_sp(stripper_level_sp); }

	// init the plant
	teplant->initialize();
	xmeas = (double*)(teplant->get_xmeas());

#ifdef USE_ADS_IF
	// setup the ads interface
	TEADSInterface ads_net_type;
	TEADSInterface ads_xmeas_tx, ads_xmeas_rx, ads_xmv_tx, ads_xmv_rx;
	TEADSInterface ads_prod_rate_sp, ads_preactor_sp, ads_lreactor_sp, ads_treactor_sp, ads_pctg_sp, ads_lseparator_sp, ads_lstripper_sp;
	TEADSInterface ads_idv, ads_sd_flag;
	if (use_ads_any)
	{
		AmsAddr plc_addr;
		plc_addr.netId.b[0] = 5;
		plc_addr.netId.b[1] = 20;
		plc_addr.netId.b[2] = 215;
		plc_addr.netId.b[3] = 224;
		plc_addr.netId.b[4] = 1;
		plc_addr.netId.b[5] = 1;
		plc_addr.port = 851;

		// network type
		ads_xmeas_tx.connect("G_IO.XMEAS", &plc_addr);
		ads_net_type.connect("G_IO.NET_TYPE", &plc_addr);
		ads_net_type.write_value<int>(&net_type, 1);

		// measured variables
		ads_xmeas_tx.connect("G_IO.XMEAS", &plc_addr);
		ads_xmeas_rx.connect("G_IO.MBS_XMEAS", &plc_addr);

		// initialize PLC with xmeas values
		ads_xmeas_tx.write_value<double>(xmeas, 41);  // initialize the XMEAS in PLC
		float xmeas0[4];
		xmeas0[0] = (float)xmeas[0]; // A feed
		xmeas0[1] = (float)xmeas[1]; // D feed
		xmeas0[2] = (float)xmeas[2]; // E feed
		xmeas0[3] = (float)xmeas[3]; // A+C feed
		ads_xmeas_rx.write_value<float>(xmeas0, 4);

		// manipulated variables
		ads_xmv_tx.connect("G_IO.XMV", &plc_addr);
		ads_xmv_rx.connect("G_IO.NET_XMV", &plc_addr);   // TODO match name to new convention (XMV_RX)

		// initialize PLC with xmv values
		ads_xmv_tx.write_value<double>(xmv, 12);  // initialize the XMV in PLC
		float xmv0[4];
		xmv0[0] = (float)xmv[0]; // D valve
		xmv0[1] = (float)xmv[1]; // E valve
		xmv0[2] = (float)xmv[2]; // A valve
		xmv0[3] = (float)xmv[4]; // A+C valve
		ads_xmv_rx.write_value<float>(xmv0, 4);

		// set points
		ads_prod_rate_sp.connect("G_IO.PROD_RATE", &plc_addr);
		ads_preactor_sp.connect("G_IO.P_REACTOR", &plc_addr);
		ads_lreactor_sp.connect("G_IO.L_REACTOR", &plc_addr);
		ads_treactor_sp.connect("G_IO.T_REACTOR", &plc_addr);
		ads_pctg_sp.connect("G_IO.PCT_G", &plc_addr);
		ads_lseparator_sp.connect("G_IO.L_SEPARATOR", &plc_addr);
		ads_lstripper_sp.connect("G_IO.L_STRIPPER", &plc_addr);

		// disturbances
		ads_idv.connect("G_IO.IDV", &plc_addr);

		// shutdown flag
		ads_sd_flag.connect("G_IO.SD_FLAG", &plc_addr);
	}
#endif 

	// create the communications channels
	int seed_rand = 17;
	double * xmeas_chan_ptr = 0, * xmv_chan_ptr = 0;
	TEChannel* xmeas_channel = 0;
	TEChannel* xmv_channel = 0;
	if (per > 0.0)  // IID channel
	{
		xmeas_channel = new TEIIDErrorChannel(per, TEPlant::NY, teplant->get_xmeas(), seed_rand);
		xmv_channel = new TEIIDErrorChannel(per, TEPlant::NU, tectlr->get_xmv(), seed_rand);
	}
	else if (gechan_on)  // GE channel
	{
		xmeas_channel = new TEGEErrorChannel(xmeas_pq, TEPlant::NY, teplant->get_xmeas(), seed_rand);
		xmv_channel = new TEGEErrorChannel(xmv_pq, TEPlant::NU, tectlr->get_xmv(), seed_rand);
	}
	else // error free channel
	{
		xmeas_channel = new TEErrorFreeChannel(TEPlant::NY, teplant->get_xmeas());
		xmv_channel = new TEErrorFreeChannel(TEPlant::NU, tectlr->get_xmv());
	}
	if (xmeas_ge_link_id > 0)
		xmeas_channel->link_id(xmeas_ge_link_id - 1);
	if (xmv_ge_link_id > 0)
		xmv_channel->link_id(xmv_ge_link_id - 1);

	tsave = tplant*double(ksave);
	double tstep = 0.0;
	if (!RT)
		tstep = 0.001 / 3600;  // default time step for simulation is 1 ms for non-real-time performance
	else
		tstep = tplant/10;  // default time step for simulation is 1/10 plant for real-time performance
	unsigned long epoch_sim = 0;
	double tplant_next = 0.0, tctlr_next = 0.0, tsave_next = 0.0;
	do
	{
		/***************************************************************************
		*
		* This is the plant section
		*
		****************************************************************************/
		if (t >= tplant_next)
		{
			if (RT) { t = tplant_next; }  // get rid of rouding error for RT mode
			tplant_next += tplant;

			try
			{
				// examine shared memory for disturbances
				if (shdmem_on && RT)
				{
					// first update the disturbance vector
					mapped_region reg_idv(*idv_shm, read_write);
					idv_pair *mem = static_cast<idv_pair*>(reg_idv.get_address());
					if (mem->index > 0)
					{
						teplant->idv(mem->index - 1, mem->value);
						std::cout << std:: endl << "IDV update: " << mem->index - 1 << "," << mem->value << std::endl;
						mem->index = 0; 
					}
				}

				// set the disturbance to hold on from command line
				if (enable_idv)
				{
					teplant->idv(idv_idx - 1);
				}

				// query the ADS interface for XMV values.
				// the ADS data overrides simulated channel
#ifdef USE_ADS_IF
				if (use_ads_xmv)
				{

					float xmv_rx[4];
					double xmv_ads[12];
					memcpy(xmv_ads, xmv, sizeof(double)*TEPlant::NU);
					ads_xmv_rx.read_value<float>(xmv_rx, 4);
					xmv_ads[0] = xmv_rx[0];		// Control valve: Feed D
					xmv_ads[1] = xmv_rx[1];		// Control valve: Feed E
					xmv_ads[2] = xmv_rx[2];		// Control valve: Feed A
					xmv_ads[3] = xmv_rx[3];		// Control valve: Feed A+C 
					xmv_chan_ptr = (*xmv_channel) + xmv_ads;
				}
#endif
				// increment the plant
				xmeas = teplant->increment(t, tplant, xmv_channel->data(), &shutdown);

				if (shdmem_on && RT)
				{
					// copy the xmeas values to shared memory
					mapped_region reg_sim(*sim_shm, read_write);
					double *shm_sim = static_cast<double*>(reg_sim.get_address());
					memcpy(shm_sim, xmeas, sizeof(double)*TEPlant::NY);
					memcpy(shm_sim + TEPlant::NY, xmv, sizeof(double)*TEPlant::NU);
				}

#ifdef USE_ADS_IF
				if (use_ads_xmeas) 
				{ 
					// send the measured variables to the simulation PLC
					ads_xmeas_tx.write_value<double>(xmeas, TEPlant::NY);

					// send disturbance vector to the simulation PLC
					ads_idv.write_value<int>(teplant->get_idv(), TEPlant::NIDV);
				}
#endif

				// apply the sensors channel to plant readings
				xmeas_chan_ptr = (*xmeas_channel) + xmeas;
				xmeas_chan_log << *xmeas_channel << std::endl;

			}
			catch (TEPlant::ShutdownException& e)
			{
				sim_log
					<< xmeas_pq << "\t"
					<< xmv_pq << "\t"

					<< t << "\t"
					<< *teplant << "\t"
					<< e.m_sd_code

					<< t << "\t"
					<< *tectlr << "\t"

					<< std::endl;

				std::cerr << e << std::endl;
				std::cerr << "ending simulation" << std::endl;

				// inform the PLC of shutdown
#ifdef USE_ADS_IF
				if (use_ads_any)
				{
					bool sd_flag = true;
					ads_sd_flag.write_value<bool>(&sd_flag, 1);
				}
#endif
				
				return 0;
			}

		}


		/***************************************************************************
		*
		* This is the controller section
		*
		****************************************************************************/
		// run the controller if time is at a scan boundary
		if (t >= tctlr_next)
		{
			if (RT) { t = tctlr_next; }  // get rid of rouding error for RT mode
			tctlr_next += tctlr;

			// increment the controller
			if (! (ext_control  && shdmem_on && RT) )
			{

				// query the ADS interface for xmeas values at the gateway.
				// the ADS data overrides simulated channel
#ifdef USE_ADS_IF
				if (use_ads_xmeas)
				{
					float mbs_xmeas_gw[4];
					double xmeas_ads[41];
					memcpy(xmeas_ads, xmeas, sizeof(double)*TEPlant::NY);
					ads_xmeas_rx.read_value<float>(mbs_xmeas_gw, 4);
					xmeas_ads[0] = mbs_xmeas_gw[0];		// Flow: Feed A
					xmeas_ads[1] = mbs_xmeas_gw[1];		// Flow: Feed D
					xmeas_ads[2] = mbs_xmeas_gw[2];		// Flow: Feed E
					xmeas_ads[3] = mbs_xmeas_gw[3];		// Flow: Feed A+C
					xmeas_chan_ptr = (*xmeas_channel) + xmeas_ads;
				}
#endif
				xmv = tectlr->increment(t, tctlr, xmeas_channel->data());
			}
			else
			{
				// apply new manipulation updates from external controller
				mapped_region reg_xmv(*xmv_shm, read_write);
				double * mem = static_cast<double*>(reg_xmv.get_address());
				memcpy(xmv, mem, sizeof(double)*TEPlant::NU);
			}

			// apply the control channel
			xmv_chan_ptr = (*xmv_channel) + xmv;
			xmv_chan_log << *xmv_channel << std::endl;

			// check for any setpoint overrides to apply
			if (shdmem_on && RT)
			{
				// apply new manipulation updates from external controller
				mapped_region reg_sp(*sp_shm, read_write);
				sp_override_pair * mem = static_cast<sp_override_pair*>(reg_sp.get_address());
				if (mem->first)
				{
					switch (mem->second.first)
					{
					case TEController::PROD_RATE:
						std::cout << "changing production rate to " << mem->second.second << std::endl;
						tectlr->prod_rate_sp(mem->second.second);
						break;
					case TEController::REACTOR_PRESS:
						std::cout << "changing reactor pressure to " << mem->second.second << std::endl;
						tectlr->reactor_pressure_sp(mem->second.second);
						break;
					case TEController::REACTOR_LEVEL:
						std::cout << "changing reactor level to " << mem->second.second << std::endl;
						tectlr->reactor_level_sp(mem->second.second);
						break;
					case TEController::REACTOR_TEMP:
						std::cout << "changing reactor temp to " << mem->second.second << std::endl;
						tectlr->reactor_temp_sp(mem->second.second);
						break;
					case TEController::SEP_LEVEL:
						std::cout << "changing separator level to " << mem->second.second << std::endl;
						tectlr->sep_level_sp(mem->second.second);
						break;
					case TEController::STRIP_LEVEL:
						std::cout << "changing stripper level to " << mem->second.second << std::endl;
						tectlr->strip_level_sp(mem->second.second);
						break;
					case TEController::PCTG:
						std::cout << "changing pctg to " << mem->second.second << std::endl;
						tectlr->pctg_sp(mem->second.second);
						break;
					default:
						std::cout << "unknown setpoint specifiy" << std::endl;
						break;
					}
					mem->first = false;
				}
			}

#ifdef USE_ADS_IF
			try
			{
				if (use_ads_xmv)
				{
					ads_xmv_tx.write_value<double>(xmv, 12);
				}
				if (use_ads_sp)
				{
					ads_prod_rate_sp.write_value<double>(tectlr->prod_rate_sp(), 1);
					ads_preactor_sp.write_value<double>(tectlr->reactor_pressure_sp(), 1);
					ads_lreactor_sp.write_value<double>(tectlr->reactor_level_sp(), 1);
					ads_treactor_sp.write_value<double>(tectlr->reactor_temp_sp(), 1);
					ads_pctg_sp.write_value<double>(tectlr->pctg_sp(), 1);
					ads_lseparator_sp.write_value<double>(tectlr->sep_level_sp(), 1);
					ads_lstripper_sp.write_value<double>(tectlr->strip_level_sp(), 1);
				}
			}
			catch (TEADSInterface::ADSError& e)
			{
				std::cout << e.errStr << std::endl;
			}

#endif

			// log current time to console
			double t_print = floor(t*1000) / 1000;
			std::cout << "\r" << "time: " << std::setprecision(5) << std::setfill('0') << t_print << " hours            ";
		}

		// log plant and controller data
		if (t >= tsave_next)
		{
			tsave_next += tsave;

			//plant
			sim_log 
				<< xmeas_pq << "\t" 
				<< xmv_pq << "\t" 

				<< t << "\t" 
				<< *teplant << "\t" 
				<< 0 << "\t" // shutdown 0

				<< t << "\t"
				<< *tectlr << "\t"

				<< std::endl;

		}

		// try to sync sim time to match wall clock time
		if (RT)
		{
			dbl_sec sim_time_dur(t * 3600.0);
			tesync.sync(sim_time_dur, time_log);
		}

		// Increment to the next time step
		// Approximation of tplant because of limited memory causes errors to 
		// integrate over time (round-off error), so we must recalculate t on 
		// every iteration using a method that truncates the floating point error.
		// Thanks to TAZ for finding this bug!
		epoch_sim++;
		t = (double)(epoch_sim) * tstep;

	} while (t <= simtime);

	// destroy the shared memory objects
	if (xmv_shm) delete xmv_shm;
	if (sim_shm) delete sim_shm;
	if(idv_shm) delete idv_shm;
	if (sp_shm) delete sp_shm;

	std::cout << std::endl;
	return 0;
}

void print_sim_params(double tplant, double tctlr, double simtime, bool rt, pq_pair xmeas_pq, pq_pair xmv_pq)
{
	BOOST_LOG_TRIVIAL(info) << std::endl << "TE simulation" << std::endl
		<< "simulation time: " << simtime << " hrs" << std::endl
		<< "plant dt: " << tplant << " hrs" << std::endl
		<< "ctlr dt: " << tctlr << " hrs" << std::endl
		<< "Real-time: " << rt << std::endl
		<< "PER (xmeas): " << xmeas_pq << std::endl
		<< "PER (xmv): " << xmv_pq << std::endl;
}

