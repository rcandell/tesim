/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

// TEGEErrorChannel is used to sync simulation time with wall clock time

#ifndef __TEGEERRORCHANNEL_H__
#define __TEGEERRORCHANNEL_H__

#include <boost/random.hpp>
#include <iostream>

#include "TETypes.h"
#include "TEChannel.h"

class TEGEErrorChannel : public TEChannel
{

	typedef boost::uniform_real<> NumberDistribution;
	typedef boost::mt19937 RandomNumberGenerator;
	typedef boost::variate_generator<RandomNumberGenerator&, NumberDistribution> Generator;

public:
	// Construct a channel object with global packet error rate, 
	// length of the data, and initial values.
	TEGEErrorChannel(pq_pair error_rate, unsigned dlen, const double* init_values, const unsigned link_id = -1, int seed = 17);

	// destroys all memory associate with object and deletes object
	virtual ~TEGEErrorChannel();

	// This method will apply the random channel to the input data
	// and return the data with channel applied.  If packets are dropped,
	// the last know value is returned.
	double* operator+(double* data);

	// overloaded output stream for channel
	virtual std::ostream& print(std::ostream& os) const;
	//friend std::ostream& operator<< (std::ostream&, const TEGEErrorChannel&);

	// sample random value from distribution
	double operator()();

	// accessors
	pq_pair error_rate() const { return m_error_rate; }

private:
	TEGEErrorChannel();
	TEGEErrorChannel(const TEGEErrorChannel&);
	TEGEErrorChannel& operator=(const TEGEErrorChannel&) {};

	// state variables
	pq_pair m_error_rate;	// global error rate

	// random generator
	TEGEErrorChannel::NumberDistribution m_distribution;
	TEGEErrorChannel::RandomNumberGenerator m_generator;
	TEGEErrorChannel::Generator m_numberGenerator;

};

#endif 
