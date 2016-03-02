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
#include <iomanip>

TETimeSync::TETimeSync()
{
}

TETimeSync::~TETimeSync()
{

}

void 
TETimeSync::init()
{
	m_wall_start = boost::chrono::system_clock::now();
}

void 
TETimeSync::sync(dbl_sec sim_secs)
{
	if (sim_secs == boost::chrono::duration<double>::zero()) return;

	// delay process to wait for wall clock
	boost::chrono::duration<double> elapsed_wall_time = boost::chrono::system_clock::now() - m_wall_start;

	if (sim_secs > elapsed_wall_time)
	{
		boost::this_thread::sleep_for(sim_secs - elapsed_wall_time);
	}
	else if (sim_secs < elapsed_wall_time )
	{
		std::cerr << "problem running in real-time" << std::endl;
	}
	// do nothing if in lock-step
}

void 
TETimeSync::sync(dbl_sec sim_secs, std::ofstream& time_log)
{
	boost::chrono::system_clock::time_point the_now_before = boost::chrono::system_clock::now();
	boost::chrono::duration<double> wall_before = the_now_before - m_wall_start;
	
	sync(sim_secs);

	boost::chrono::system_clock::time_point the_now_after = boost::chrono::system_clock::now();
	boost::chrono::duration<double> wall_after = the_now_after - m_wall_start;
	time_log 
		<< sim_secs.count() << "\t"
		<< wall_before.count() << "\t"  
		<< wall_after.count() << "\t" 
		<< (sim_secs-wall_after).count() << std::endl;
}
