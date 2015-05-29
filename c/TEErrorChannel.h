/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

// TEErrorChannel is used to sync simulation time with wall clock time

#ifndef __TEERRORCHANNEL_H__
#define __TEERRORCHANNEL_H__

#include <boost/random.hpp>
#include <iostream>

typedef std::pair<double, double> per_pair;
namespace std {
	std::istream& operator>>(std::istream& in, per_pair& ss);
	std::ostream& operator<<(std::ostream& in, const per_pair& ss);
}

class TEErrorChannel
{

	typedef boost::uniform_real<> NumberDistribution;
	typedef boost::mt19937 RandomNumberGenerator;
	typedef boost::variate_generator<RandomNumberGenerator&, NumberDistribution> Generator;

public:
	// Construct a channel object with global packet error rate, 
	// length of the data, and initial values.
	TEErrorChannel(per_pair error_rate, unsigned dlen, const double* init_values, int seed = 17);

	// destroys all memory associate with object and deletes object
	virtual ~TEErrorChannel();

	// This method will apply the random channel to the input data
	// and return the data with channel applied.  If packets are dropped,
	// the last know value is returned.
	double* operator+(double* data);

	// overloaded output stream for channel
	friend std::ostream& operator<< (std::ostream&, const TEErrorChannel&);

	// sample random value from distribution
	double operator()();

	// accessors
	unsigned dlen() const { return m_dlen; }
	const double* previous() const { return m_previous; }
	per_pair error_rate() const { return m_error_rate; }
	const bool* chan_state() const { return m_chan_state; }

private:
	TEErrorChannel();
	TEErrorChannel(const TEErrorChannel&);
	TEErrorChannel& operator=(const TEErrorChannel&) {};

	// state variables
	per_pair m_error_rate;	// global error rate
	double* m_previous;		// previous elements
	unsigned m_dlen;		// number of elements
	bool* m_chan_state;		// channel state of last increment, true means channel is up

	// random generator
	TEErrorChannel::NumberDistribution m_distribution;
	TEErrorChannel::RandomNumberGenerator m_generator;
	TEErrorChannel::Generator m_numberGenerator;

};

#endif 
