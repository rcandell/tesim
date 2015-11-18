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

TEErrorFreeChannel::TEErrorFreeChannel(unsigned dlen, const double* init_values) : m_dlen(dlen), m_previous(0)
{
	m_previous = new double[m_dlen]();
	for (unsigned ii = 0; ii < dlen; ii++) m_chan_state[ii] = true;  // init to good state

	// copy the initial values into state memory
	for (unsigned ii = 0; ii < m_dlen; ii++)
	{
		m_previous[ii] = init_values[ii];
	}
}

TEErrorFreeChannel::~TEErrorFreeChannel()
{
	delete m_previous;
}

double* TEErrorFreeChannel::operator+(double* data)
{
	for (unsigned ii = 0; ii < m_dlen; ii++)
	{
		// retain the data for the next increment
		m_previous[ii] = data[ii];
	}
	return m_previous;
}

// overloaded output stream for channel
std::ostream& operator<< (std::ostream& lhs, const TEErrorFreeChannel& rhs)
{
	unsigned dlen = rhs.dlen();
	for (unsigned ii = 0; ii < dlen-1; ii++)
	{
		lhs << rhs.chan_state()[ii] << "\t";
	}
	lhs << rhs.chan_state()[dlen-1];
	return lhs;
}

