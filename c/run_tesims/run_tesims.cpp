
/*

* Author:       Rick Candell (rick.candell@nist.gov)

* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include <boost/random.hpp>
#include <boost/timer/timer.hpp>
#include <boost/program_options.hpp>

#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>

int tc_nochan_baseline(int argc, char* argv[]);
int tc_idv_baseline(int argc, char* argv[]); 
int tc_all_mc(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	// auto timer used as a performance profiler
	boost::timer::auto_cpu_timer t_wall_auto;

	// return tc_nochan_baseline(argc, argv);
	return tc_idv_baseline(argc, argv);
	// return tc_all_mc(argc, argv);
}

int tc_all_mc(int argc, char* argv[])
{
	unsigned num_iterations = 0;
	std::string logfile_prefix;

	// program options
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "print the help message")
		("num-iterations,n", po::value<unsigned>(&num_iterations)->required(), "set the number of iterations for the simulation.")
		("output-prefix,p", po::value<std::string>(&logfile_prefix)->required(), "set the prefix for the output files.")
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

	const std::string exec("..\\debug\\tesim");
	const double simtime = 32;
	const double tstep = 0.0005;
	const double tscan = 0.0005;
	const unsigned ksave = 20;

	// monte carlo parameters
	unsigned seed = 1731;

	boost::mt19937 rng_p(seed); 
	boost::uniform_real<float> u_p(0.6F, 1.0F);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > gen_p(rng_p, u_p);

	boost::mt19937 rng_q(seed);
	boost::uniform_real<float> u_q(0.05F, 0.5F);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > gen_q(rng_q, u_q);

	unsigned ii = 0;
	while (ii < num_iterations)
	{
		float xmeas_p = gen_p();
		float xmeas_q = gen_q();

		float xmv_p = xmeas_p; // gen_p();
		float xmv_q = xmeas_q; // gen_q();

		std::string the_call;
		std::ostringstream the_call_ss;
		the_call_ss << exec << " "
			<< "-s " << std::to_string(simtime) << " "
			<< "-t " << std::to_string(tstep) << " "
			<< "-c " << std::to_string(tscan) << " "
			<< "-k " << std::to_string(ksave) << " "
			<< "--xmeas-pq " << std::to_string(xmeas_p) << ":" << std::to_string(xmeas_q) << " "
			<< "--xmv-pq " << std::to_string(xmv_p) << ":" << std::to_string(xmv_q) << " "
			<< "--logfile-prefix " << logfile_prefix;
		if (ii > 0)
			the_call_ss << " -a";
		the_call = the_call_ss.str();
		std::cout << the_call << std::endl;
		
		// run the program
		std::system(the_call.c_str());

		ii++;
	}

	return 0;
}

int tc_nochan_baseline(int argc, char* argv[])
{
	std::string logfile_prefix;

	// program options
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "print the help message")
		("output-prefix,p", po::value<std::string>(&logfile_prefix)->required(), "set the prefix for the output files.")
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

	const std::string exec("..\\debug\\tesim");
	const double simtime = 32;
	const double tstep = 0.0005;
	const double tscan = 0.0005;
	const unsigned ksave = 20;

	std::string the_call;
	std::ostringstream the_call_ss;
	the_call_ss << exec << " "
		<< "-s " << std::to_string(simtime) << " "
		<< "-t " << std::to_string(tstep) << " "
		<< "-c " << std::to_string(tscan) << " "
		<< "-k " << std::to_string(ksave) << " "
		<< "--xmeas-pq " << std::to_string(0.0) << ":" << std::to_string(1.0) << " "
		<< "--xmv-pq " << std::to_string(0.0) << ":" << std::to_string(1.0) << " "
		<< "--logfile-prefix " << logfile_prefix;

	the_call = the_call_ss.str();
	std::cout << the_call << std::endl;

	// run the program
	std::system(the_call.c_str());

	return 0;
}

int tc_idv_baseline(int argc, char* argv[])
{
	std::string logfile_prefix;

	// program options
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "print the help message")
		("output-prefix,p", po::value<std::string>(&logfile_prefix)->required(), "set the prefix for the output files.")
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

	const std::string exec("..\\debug\\tesim");
	const double simtime = 72;
	const double tstep = 0.0005;
	const double tscan = 0.0005;
	const unsigned ksave = 20;

	for (unsigned idv_idx = 1; idv_idx <= 15; idv_idx++)
	{
		std::string the_call;
		std::ostringstream the_call_ss;
		the_call_ss << exec << " "
			<< "-s " << std::to_string(simtime) << " "
			<< "-t " << std::to_string(tstep) << " "
			<< "-c " << std::to_string(tscan) << " "
			<< "-k " << std::to_string(ksave) << " "
			<< "--logfile-prefix " << logfile_prefix << " "
			<< "-i " << idv_idx;

		if (idv_idx > 1)
		{
			the_call_ss << " -a ";
		}

		the_call = the_call_ss.str();
		std::cout << the_call << std::endl;

		// run the program
		std::system(the_call.c_str());
	}

	return 0;
}