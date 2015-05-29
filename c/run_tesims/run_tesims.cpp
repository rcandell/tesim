
/*

* Author:       Rick Candell (rick.candell@nist.gov)

* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>

int main(int argc, char* argv[])
{
	const std::string exec("..\\debug\\tesim");
	const double simtime = 32;
	const double tstep = 0.0005;
	const double tscan = 0.0005;
	double xmeas_p_min = 0.5, xmeas_p_max = 1.0, xmeas_p_step = 0.1;
	double xmeas_q_min = 0.5, xmeas_q_max = 1.0, xmeas_q_step = 0.1;

	std::string logfile_prefix;

	for (double xmeas_p = xmeas_p_min; xmeas_p <= xmeas_p_max; xmeas_p += xmeas_p_step)
	{
		for (double xmeas_q = xmeas_q_min; xmeas_q <= xmeas_q_max; xmeas_q += xmeas_q_step)
		{
			std::string logfile_prefix;
			std::ostringstream lfp_ss;
			lfp_ss << "xmeas_" << std::fixed << std::setprecision(5) << xmeas_p << "_"
				<< std::fixed << std::setprecision(5) << xmeas_q
				<< "_noxmvchan";
			logfile_prefix = lfp_ss.str();

			std::string the_call;
			std::ostringstream the_call_ss;
			the_call_ss << exec << " " 
				<< "-s " << std::to_string(simtime) << " "
				<< "-t " << std::to_string(tstep) << " "
				<< "-c " << std::to_string(tscan) << " "
				<< "--xmeas-per " << std::to_string(xmeas_p) << ":" << std::to_string(xmeas_q) << " "
				<< "--xmv-per 0.0:1.0 "
				<< "--logfile-prefix " << logfile_prefix;
			the_call = the_call_ss.str();
			std::cout << the_call << std::endl;

			std::system(the_call.c_str());
		}
	}
	
	return 0;
}

