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
	TEChannel(unsigned dlen, const double* init_values);

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
	void link_id(const int link_id) { m_link_id = link_id; }
	const int link_id() const { return m_link_id;  }

	// ostream printing
	virtual std::ostream& print(std::ostream&) const;
	friend std::ostream& operator<< (std::ostream&, const TEChannel&);

protected:
	
	int m_link_id; 	// to what link to apply channel
	double* m_data;			// data in the channel
	unsigned m_dlen;		// number of elements
	bool* m_chan_state;		// channel state of last increment, true means channel is up

	TEChannel(){}
	TEChannel(const TEChannel&){}
	TEChannel& operator=(const TEChannel&){}
};

#endif 
