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

#include "TEGEErrorChannel.h"

TEGEErrorChannel::TEGEErrorChannel(pq_pair error_rate, unsigned dlen, const double* init_values, const unsigned link_id, int seed)
	: TEChannel(dlen, init_values), m_error_rate(error_rate), m_distribution(0.0, 1.0), m_numberGenerator(m_generator, m_distribution)
{
	// refer to http://www.radmangames.com/programming/generating-random-numbers-in-c
	// seed the generator
	m_generator.seed(seed); // seed with the current time 
}

TEGEErrorChannel::~TEGEErrorChannel()
{	
}

double TEGEErrorChannel::operator()()
{
	return m_numberGenerator();
}

double* TEGEErrorChannel::operator+(double* data)
{
	for (unsigned ii = 0; ii < m_dlen; ii++)
	{
		if ((m_link_id>-1) && (ii==m_link_id))
		{
			// roll the dice to determine state fo this increment
			double rnd = (*this)();
			if (m_chan_state[ii]) // channel is up
			{
				// test for transition to channel down/error state
				m_chan_state[ii] = (rnd <= m_error_rate.first) ? false : true;
			}
			else // channel is currently down
			{
				// test for transition to channel up/good state
				m_chan_state[ii] = (rnd <= m_error_rate.second) ? true : false;
			}

			// take action on the data based on links state
			if (m_chan_state[ii])  // link is good, update the channel state with new data
			{
				// retain the data for the next increment
				m_data[ii] = data[ii];
			}
		}
		else
		{
			m_data[ii] = data[ii];
		}
	}
	return m_data;
}

std::ostream& TEGEErrorChannel::print(std::ostream& os) const
{
	return TEChannel::print(os);
}

#if 0
// overloaded output stream for channel
std::ostream& operator<< (std::ostream& lhs, const TEGEErrorChannel& rhs)
{
	unsigned dlen = rhs.dlen();
	for (unsigned ii = 0; ii < dlen - 1; ii++)
	{
		lhs << rhs.chan_state()[ii] << "\t";
	}
	lhs << rhs.chan_state()[dlen - 1];
	return lhs;
}
#endif // 0


