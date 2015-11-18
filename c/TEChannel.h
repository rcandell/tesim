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

class TEChannel
{
public:
	TEChannel(unsigned dlen, const double* init_values)
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
	}
	virtual ~TEChannel()
	{
		delete m_previous;
		delete m_chan_state;
	}

	virtual double* data() const { return m_previous; }

	unsigned dlen() const { return m_dlen; }
	const double* previous() const { return m_previous; }
	const bool* chan_state() const { return m_chan_state; }

protected:
	
	double* m_previous;		// previous elements
	unsigned m_dlen;		// number of elements
	bool* m_chan_state;		// channel state of last increment, true means channel is up

	TEChannel(){}
	TEChannel(const TEChannel&){}
	TEChannel& operator=(const TEChannel&){}
};

#endif 
