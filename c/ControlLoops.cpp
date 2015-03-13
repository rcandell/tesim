/*
* Author:		Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:		Public Domain
*/

#include "ControlLoops.h"

////////////////////////////////
//      DISCRETE PI LOOP      //
////////////////////////////////

SimpleDiscretePILoop::SimpleDiscretePILoop(dbl ic, dbl v_ic, dbl kc, dbl lo, dbl hi, dbl ts, dbl ti) :
	discretePI_Object(ic, v_ic, kc, lo, hi, ts, ti)
{
}

double SimpleDiscretePILoop::increment(dbl sp, dbl meas, dbl t, dbl dt)
{
	dbl value = discretePI_Object.increment(sp, meas, t, dt);
	return value;
}


////////////////////////////////
//      VELOCITY PI LOOP      //
////////////////////////////////

SimpleVelocityPILoop::SimpleVelocityPILoop(dbl v_ic, dbl kc, dbl ts, dbl ti) :
	velocityPI_Object(v_ic, kc, ts, ti)
{
}

double SimpleVelocityPILoop::increment(dbl sp, dbl meas, dbl t, dbl dt)
{
	dbl value = velocityPI_Object.increment(sp, meas);
	return value;
}

//////////////////////////////
//      % G IN PRODUCT      //
//////////////////////////////

PercentG_inProduct::PercentG_inProduct(dbl ic, dbl v_ic, dbl pos_R, dbl neg_R, dbl kc, dbl lo, dbl hi, dbl ts, dbl ti) :
	discretePI_Object(ic, v_ic, kc, lo, hi, ts, ti),
	pos_R(pos_R), neg_R(neg_R)
{
}

double PercentG_inProduct::increment(dbl sp, dbl meas, dbl t, dbl dt)
{
	dbl input;
	input = rateLimiter(sp, t, dt);
	pctGsp = input;	// Store for the users' viewing pleasure
	return discretePI_Object.increment(input, meas, t, dt);
}

double PercentG_inProduct::rateLimiter(dbl sp, dbl t, dbl dt)
{
	dbl out, rate;
	if (t != 0.) {
		rate = (sp - lastOut) / (t - lastTime);
		if (rate > pos_R) out = (t - lastTime) * pos_R + lastOut;
		else if (rate < neg_R) out = (t - lastTime) * neg_R + lastOut;
		else out = rate + lastOut;
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

ProductionRate::ProductionRate(dbl ic, dbl v_ic, dbl pos_R, dbl neg_R, dbl kc, dbl lo, dbl hi, dbl ts, dbl ti) :
	discretePI_Object(ic, v_ic, kc, lo, hi, ts, ti),
	pos_R(pos_R), neg_R(neg_R)
{
}

double ProductionRate::increment(dbl sp, dbl meas, dbl t, dbl dt)
{
	dbl input;
	input = rateLimiter(sp, t, dt);
	prodRate = input;	// Store for the users' viewing pleasure
	return ((input * (100. / 22.89)) + discretePI_Object.increment(input, meas, t, dt));
}

double ProductionRate::rateLimiter(dbl sp, dbl t, dbl dt)
{
	dbl out, rate;
	if (t != 0.) {
		rate = (sp - lastOut) / (t - lastTime);
		if (rate > pos_R) out = (t - lastTime) * pos_R + lastOut;
		else if (rate < neg_R) out = (t - lastTime) * neg_R + lastOut;
		else out = rate + lastOut;
	}
	else out = sp;	// If the time is 0, we needb to output the current input
	// Store our state variables
	lastTime = t;
	lastOut = out;

	return out;
}

