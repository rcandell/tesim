/*

* Author:       Rick Candell (rick.candell@nist.gov)
*               
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

// TETimeSync is used to sync simulation time with wall clock time

#ifndef __TETIMESYNC_H__
#define __TETIMESYNC_H__

#include <boost/chrono.hpp>

typedef boost::chrono::duration<double, boost::micro> dbl_us;
typedef boost::chrono::duration<double> dbl_sec;

class TETimeSync
{
public:
	TETimeSync();
	virtual ~TETimeSync();

	void init();
	void sync(dbl_sec sim_time_sec);

private:
	TETimeSync(const TETimeSync&) {};
	TETimeSync& operator=(const TETimeSync&) {};

	// state variables
	boost::chrono::steady_clock::time_point m_wall_start;
};

#endif 
