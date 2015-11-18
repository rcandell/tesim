/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

// TEErrorFreeChannel is used to sync simulation time with wall clock time

#ifndef __TEERRORFREECHANNEL_H__
#define __TEERRORFREECHANNEL_H__

#include <boost/random.hpp>
#include <iostream>

#include "TETypes.h"
#include "TEChannel.h"

class TEErrorFreeChannel : public TEChannel
{
public:
	// Construct a channel object with global packet error rate, 
	// length of the data, and initial values.
	TEErrorFreeChannel(unsigned dlen, const double* init_values);

	// destroys all memory associate with object and deletes object
	virtual ~TEErrorFreeChannel();

	// This method will apply the random channel to the input data
	// and return the data with channel applied.  If packets are dropped,
	// the last know value is returned.
	double* operator+(double* data);

	// overloaded output stream for channel
	friend std::ostream& operator<< (std::ostream&, const TEErrorFreeChannel&);

	// accessors
	unsigned dlen() const { return m_dlen; }
	const double* previous() const { return m_previous; }
	const bool* chan_state() const { return m_chan_state; }

private:
	TEErrorFreeChannel();
	TEErrorFreeChannel(const TEErrorFreeChannel&);
	TEErrorFreeChannel& operator=(const TEErrorFreeChannel&) {};

	// state variables
	double* m_previous;		// previous elements
	unsigned m_dlen;		// number of elements
	bool* m_chan_state;		// channel state of last increment, true means channel is up

};

#endif 
