/*

* Author:       Rick Candell (rick.candell@nist.gov)

* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include <iostream>     // std::cout, std::ostream, std::ios
#include <utility>
#include <iomanip>
#include <string>

#include "TEErrorFreeChannel.h"

TEErrorFreeChannel::TEErrorFreeChannel(unsigned dlen, const double* init_values) 
	: TEChannel(dlen, init_values)
{
}

TEErrorFreeChannel::~TEErrorFreeChannel()
{
}

double * TEErrorFreeChannel::operator+(double* data)
{
	for (unsigned ii = 0; ii < m_dlen; ii++)
	{
		m_data[ii] = data[ii];
	}
	return m_data;
}


