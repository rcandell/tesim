/*
* Author:       Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain
*/

#ifndef __PI_H__
#define __PI_H__

#define dbl double

/////////////////
// Velocity PI //
/////////////////

class velocityPI
{
public:
	dbl increment(dbl sp, dbl meas);
	velocityPI(dbl v_ic, dbl kc_in, dbl ts_in, dbl ti_in);
private:
	double err, out, kc, ti, ts, lastErr = 0;
};

/////////////////
// Discrete PI //
/////////////////

class discretePI
{
private:
	double	saturation(dbl hi, dbl lo, dbl val);
	velocityPI vpi;

public:
	double	tstep, tscan, prevDelta, ts, ti, kc, lo, hi, ic, lastErr;
	double	increment(dbl sp, dbl meas, dbl t, dbl dt);
	discretePI(dbl ic, dbl v_ic, dbl kc, dbl lo, dbl hi, dbl ts, dbl ti);
};

#endif
