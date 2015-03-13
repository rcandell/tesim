/*
* Author:		Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:		Public Domain
*/

#include "PI.h"

/////////////////
// Velocity PI //
/////////////////

velocityPI::velocityPI(dbl v_ic, dbl kc_in, dbl ts_in, dbl ti_in) :
	kc(kc_in), ti(ti_in), ts(ts_in), lastErr(v_ic)
{
}

double velocityPI::increment(dbl sp, dbl meas)
{
	double err, out;
	err = sp - meas;								// Calculate the error
	out = (kc * (err + err*(ts / ti) - lastErr));	// Calculate the output
	lastErr = err;									// Store the error for the next iteration
	return out;										// Return the output
}

/////////////////
// Discrete PI //
/////////////////

discretePI::discretePI(dbl ic, dbl v_ic, dbl kc, dbl lo, dbl hi, dbl ts, dbl ti) : 
	vpi(v_ic, kc, ts, ti),
	ic(ic), kc(kc), lo(lo), hi(hi), ts(ts), ti(ti)
{
}

double discretePI::increment(dbl sp, dbl meas, dbl t, dbl dt)
{
	double out, delta;
	delta = vpi.increment(sp, meas);				// Calc velocity PI
	if (t == 0.)	prevDelta = ic;		//
	delta += prevDelta;								// Sum with last
	delta = saturation(hi, lo, delta);				// Test the limits
	out = prevDelta;								// Get the previous value (unit delay)
	prevDelta = delta;								// Store current to use on the next iteration
	return out;
}

double discretePI::saturation(double hi, double lo, double val)	{
	if (val < lo) return lo;
	else if (val > hi) return hi;
	else return val;
}