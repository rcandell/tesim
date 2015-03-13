/*
* Author:		Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:		Public Domain
*/

#ifndef __CONTROLLOOPS_H__
#define __CONTROLLOOPS_H__

#define dbl double

#include "PI.h"

class SimpleDiscretePILoop
{
private:
	discretePI discretePI_Object;									// Create the discrete PI loop object
	SimpleDiscretePILoop();											// Forces a compiler error if you do not enter parameters
	SimpleDiscretePILoop(const SimpleDiscretePILoop&);				// ''
	SimpleDiscretePILoop& operator=(const SimpleDiscretePILoop&);	// Cannot call the assignment operator

public:
	dbl increment(dbl sp, dbl meas, dbl t, dbl dt);
	SimpleDiscretePILoop(dbl ic, dbl v_ic, dbl kc, dbl lo, dbl hi, dbl ts, dbl ti);
	//virtual ~SimpleRateLoop();
};


class SimpleVelocityPILoop
{
private:
	dbl ts, ti;
	velocityPI velocityPI_Object;									// Create the discrete PI loop object
	SimpleVelocityPILoop();											// Forces a compiler error if you do not enter parameters
	SimpleVelocityPILoop(const SimpleVelocityPILoop&);				// ''
	SimpleVelocityPILoop& operator=(const SimpleVelocityPILoop&);	// Cannot call the assignment operator

public:
	dbl increment(dbl sp, dbl meas, dbl t, dbl dt);
	SimpleVelocityPILoop(dbl v_ic, dbl kc, dbl ts, dbl ti);
	//virtual ~SimpleRateLoop();
};

class PercentG_inProduct
{
	double pos_R, neg_R, lastTime = 0., lastOut, pctGsp;
	discretePI discretePI_Object;									// Create the discrete PI loop object
	double rateLimiter(dbl sp, dbl t, dbl dt);
public:
	dbl increment(dbl sp, dbl meas, dbl t, dbl dt);
	dbl getPctGsp(void) { return pctGsp; }
	PercentG_inProduct(dbl ic, dbl v_ic, dbl pos_R, dbl neg_R, dbl kc, dbl lo, dbl hi, dbl ts, dbl ti);
};

class ProductionRate
{
private:
	dbl pos_R, neg_R, lastTime = 0., lastOut, prodRate;
	discretePI discretePI_Object;									// Create the discrete PI loop object
	double rateLimiter(dbl sp, dbl t, dbl dt);
public:
	dbl increment(dbl sp, dbl meas, dbl t, dbl dt);
	dbl getProductionRate(void) { return prodRate; }
	ProductionRate(dbl ic, dbl v_ic, dbl pos_R, dbl neg_R, dbl kc, dbl lo, dbl hi, dbl ts, dbl ti);
};

#endif
