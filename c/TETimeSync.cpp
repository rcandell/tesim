/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include "TETimeSync.h"

#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

TETimeSync::TETimeSync()
{
}

TETimeSync::~TETimeSync()
{

}

void 
TETimeSync::init()
{
	m_wall_start = boost::chrono::steady_clock::now();
}

void 
TETimeSync::sync(dbl_sec sim_secs)
{
	// delay process to wait for wall clock
	boost::chrono::duration<double> elapsed_wall_time = boost::chrono::steady_clock::now() - m_wall_start;

	if (sim_secs > elapsed_wall_time)
	{
		boost::this_thread::sleep_for(sim_secs - elapsed_wall_time);
	}
	else
	{
		std::cerr << "problem running in real-time" << std::endl;
	}
}

