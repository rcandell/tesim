/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

// TEChannel is a base class for all channel types

#ifndef __TECHANNEL_H__
#define __TECHANNEL_H__

#include <iostream>     // std::cout, std::ostream, std::ios

class TEChannel
{
public:
	TEChannel(unsigned dlen, const double* init_values)
		: m_dlen(dlen)
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
	virtual ~TEChannel()
	{
		delete m_data;
		delete m_chan_state;
	}

	// apply the channel to a vector of values
	virtual double* operator+(double* data) = 0;

	// accessors
	virtual double* data() const { return m_data; }
	unsigned dlen() const { return m_dlen; }
	const bool* chan_state() const { return m_chan_state; }

	// ostream printing
	virtual std::ostream& print(std::ostream&) const;
	friend std::ostream& operator<< (std::ostream&, const TEChannel&);

protected:
	
	double* m_data;			// data in the channel
	unsigned m_dlen;		// number of elements
	bool* m_chan_state;		// channel state of last increment, true means channel is up

	TEChannel(){}
	TEChannel(const TEChannel&){}
	TEChannel& operator=(const TEChannel&){}
};

#endif 
