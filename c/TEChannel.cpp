/*

* Author:       Rick Candell (rick.candell@nist.gov)

* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include "TEChannel.h"

TEChannel::TEChannel(unsigned dlen, const double* init_values)
	: m_dlen(dlen), m_link_id(-1)
{
	m_data = new double[m_dlen]();
	m_chan_state = new bool[m_dlen]();
	for (unsigned ii = 0; ii < dlen; ii++) m_chan_state[ii] = true;  // init to good state
	// std::fill_n(m_chan_state, dlen, true); // init to good state

	// copy the initial values into state memory
	for (unsigned ii = 0; ii < m_dlen; ii++)
	{
		m_data[ii] = init_values[ii];
	}
}

std::ostream& TEChannel::print(std::ostream& os) const
{
	unsigned dlen = this->dlen();
	for (unsigned ii = 0; ii < dlen - 1; ii++)
	{
		os << this->chan_state()[ii] << "\t" << this->data()[ii] << "\t";
	}
	os << this->chan_state()[dlen - 1] << "\t" << this->data()[dlen - 1];
	return os;
}
std::ostream& operator<< (std::ostream& lhs, const TEChannel& rhs)
{
	return rhs.print(lhs);
}

