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

#include "TEIIDErrorChannel.h"

TEIIDErrorChannel::TEIIDErrorChannel(double error_rate, unsigned dlen, const double* init_values, int seed)
	:m_error_rate(error_rate), m_dlen(dlen), m_previous(0), m_chan_state(0), m_distribution(0.0, 1.0), m_numberGenerator(m_generator, m_distribution)
{
	m_previous = new double[m_dlen]();
	m_chan_state = new bool[m_dlen]();
	for (unsigned ii = 0; ii < dlen; ii++) m_chan_state[ii] = true;  // init to good state
	// std::fill_n(m_chan_state, dlen, true); // init to good state

	// copy the initial values into state memory
	for (unsigned ii = 0; ii < m_dlen; ii++)
	{
		m_previous[ii] = init_values[ii];
	}

	// refer to http://www.radmangames.com/programming/generating-random-numbers-in-c
	// seed the generator
	m_generator.seed(seed); // seed with the current time 
}

TEIIDErrorChannel::~TEIIDErrorChannel()
{
	delete m_previous;
}

double TEIIDErrorChannel::operator()()
{
	return m_numberGenerator();
}

double* TEIIDErrorChannel::operator+(double* data)
{
	for (unsigned ii = 0; ii < m_dlen; ii++)
	{
		// roll the dice to determine state fo this increment
		double rnd = (*this)();
		m_chan_state[ii] = (rnd <= m_error_rate) ? false : true;

		// take action on the data based on link state
		if (!m_chan_state[ii])
		{
			// a packet error has occured.  apply previous value.
			data[ii] = m_previous[ii];
		}

		// retain the data for the next increment
		m_previous[ii] = data[ii];
	}
	return data;
}

// overloaded output stream for channel
std::ostream& operator<< (std::ostream& lhs, const TEIIDErrorChannel& rhs)
{
	unsigned dlen = rhs.dlen();
	for (unsigned ii = 0; ii < dlen-1; ii++)
	{
		lhs << rhs.chan_state()[ii] << "\t";
	}
	lhs << rhs.chan_state()[dlen-1];
	return lhs;
}

