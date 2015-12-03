/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

// TEIIDErrorChannel is used to sync simulation time with wall clock time

#ifndef __TEIIDERRORCHANNEL_H__
#define __TEIIDERRORCHANNEL_H__

#include <boost/random.hpp>
#include <iostream>

#include "TETypes.h"
#include "TEChannel.h"

class TEIIDErrorChannel : public TEChannel
{

	typedef boost::uniform_real<> NumberDistribution;
	typedef boost::mt19937 RandomNumberGenerator;
	typedef boost::variate_generator<RandomNumberGenerator&, NumberDistribution> Generator;

public:
	// Construct a channel object with global packet error rate, 
	// length of the data, and initial values.
	TEIIDErrorChannel(double error_rate, unsigned dlen, const double* init_values, const unsigned link_id = -1, int seed = 17);

	// destroys all memory associate with object and deletes object
	virtual ~TEIIDErrorChannel();

	// This method will apply the random channel to the input data
	// and return the data with channel applied.  If packets are dropped,
	// the last known value is returned.
	virtual double* operator+(double* data);

	// overloaded output stream for channel
	virtual std::ostream& print(std::ostream& os) const;

	// sample random value from distribution
	double operator()();

	// accessors
	double error_rate() const { return m_error_rate; }

private:
	TEIIDErrorChannel();
	TEIIDErrorChannel(const TEIIDErrorChannel&);
	TEIIDErrorChannel& operator=(const TEIIDErrorChannel&) {};

	// state variables
	double m_error_rate;	// global error rate

	// random generator
	TEIIDErrorChannel::NumberDistribution m_distribution;
	TEIIDErrorChannel::RandomNumberGenerator m_generator;
	TEIIDErrorChannel::Generator m_numberGenerator;

};

#endif 
