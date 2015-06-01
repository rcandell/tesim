/*
* Author:       Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain
*/

#ifndef __PI_H__
#define __PI_H__

/////////////////
// Velocity PI //
/////////////////

class velocityPI
{
public:
	double increment(double sp, double meas);
	velocityPI(double v_ic, double kc_in, double ts_in, double ti_in);
private:
	double err, out, kc, ti, ts, lastErr = 0;
};

/////////////////
// Discrete PI //
/////////////////

class discretePI
{
private:
	double	saturation(double hi, double lo, double val);
	velocityPI vpi;

public:
	double	tstep, tscan, prevDelta, ts, ti, kc, lo, hi, ic, lastErr;
	double	increment(double sp, double meas, double t, double dt);
	discretePI(double ic, double v_ic, double kc, double lo, double hi, double ts, double ti);
};

#endif
