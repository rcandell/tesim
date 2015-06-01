/*
* Author:       Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain
*/

#include "ControlLoops.h"

////////////////////////////////
//      DISCRETE PI LOOP      //
////////////////////////////////

SimpleDiscretePILoop::SimpleDiscretePILoop(double ic, double v_ic, double kc, double lo, double hi, double ts, double ti) :
	discretePI_Object(ic, v_ic, kc, lo, hi, ts, ti)
{
}

double SimpleDiscretePILoop::increment(double sp, double meas, double t, double dt)
{
	double value = discretePI_Object.increment(sp, meas, t, dt);
	return value;
}


////////////////////////////////
//      VELOCITY PI LOOP      //
////////////////////////////////

SimpleVelocityPILoop::SimpleVelocityPILoop(double v_ic, double kc, double ts, double ti) :
	velocityPI_Object(v_ic, kc, ts, ti)
{
}

double SimpleVelocityPILoop::increment(double sp, double meas, double t, double dt)
{
	double value = velocityPI_Object.increment(sp, meas);
	return value;
}

//////////////////////////////
//      % G IN PRODUCT      //
//////////////////////////////

PercentG_inProduct::PercentG_inProduct(double ic, double v_ic, double pos_R, double neg_R, double kc, double lo, double hi, double ts, double ti) :
	discretePI_Object(ic, v_ic, kc, lo, hi, ts, ti),
	pos_R(pos_R), neg_R(neg_R)
{
}

double PercentG_inProduct::increment(double sp, double meas, double t, double dt)
{
	double input;
	input = rateLimiter(sp, t, dt);
	pctGsp = input;	// Store for the users' viewing pleasure
	return discretePI_Object.increment(input, meas, t, dt);
}

double PercentG_inProduct::rateLimiter(double sp, double t, double dt)
{
	double out, rate;
	if (t != 0.) {
		rate = (sp - lastOut) / (t - lastTime);
		if (rate > pos_R) out = (t - lastTime) * pos_R + lastOut;
		else if (rate < neg_R) out = (t - lastTime) * neg_R + lastOut;
		else out = sp;
	}
	else out = sp;	// If the time is 0, we need to output the current input
	// Store our state variables
	lastTime = t;
	lastOut = out;

	return out;
}

///////////////////////////////
//      PRODUCTION RATE      //
///////////////////////////////

ProductionRate::ProductionRate(double ic, double v_ic, double pos_R, double neg_R, double kc, double lo, double hi, double ts, double ti) :
	discretePI_Object(ic, v_ic, kc, lo, hi, ts, ti),
	pos_R(pos_R), neg_R(neg_R)
{
}

double ProductionRate::increment(double sp, double meas, double t, double dt)
{
	double input;
	input = rateLimiter(sp, t, dt);
	prodRate = input;	// Store for the users' viewing pleasure
	return ((input * (100. / 22.89)) + discretePI_Object.increment(input, meas, t, dt));
}

double ProductionRate::rateLimiter(double sp, double t, double dt)
{
	double out, rate;
	if (t != 0.) {
		rate = (sp - lastOut) / (t - lastTime);
		if (rate > pos_R) out = (t - lastTime) * pos_R + lastOut;
		else if (rate < neg_R) out = (t - lastTime) * neg_R + lastOut;
		else out = sp;
	}
	else out = sp;	// If the time is 0, we need to output the current input
	// Store our state variables
	lastTime = t;
	lastOut = out;

	return out;
}

