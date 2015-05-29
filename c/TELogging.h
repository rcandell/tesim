/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#ifndef __TE_LOGGING_H__
#define __TE_LOGGING_H__

#include <fstream>     
#include <string>

#include "TETypes.h"

class xmeas_ofstream : public std::ofstream 
{
public:
	xmeas_ofstream(pq_pair xmeas_pq, pq_pair xmv_pq)
		: xmeas_pq(xmeas_pq), xmv_pq(xmv_pq)
	{};

	pq_pair xmeas_pq;
	pq_pair xmv_pq;

};

class xmv_ofstream : public std::ofstream {};

#endif