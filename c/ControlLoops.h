/*
* Author:       Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain
*/

#ifndef __CONTROLLOOPS_H__
#define __CONTROLLOOPS_H__

#include "PI.h"

class SimpleDiscretePILoop
{
private:
	discretePI discretePI_Object;									// Create the discrete PI loop object
	SimpleDiscretePILoop();											// Forces a compiler error if you do not enter parameters
	SimpleDiscretePILoop(const SimpleDiscretePILoop&);				// ''
	SimpleDiscretePILoop& operator=(const SimpleDiscretePILoop&);	// Cannot call the assignment operator

public:
	double increment(double sp, double meas, double t, double dt);
	SimpleDiscretePILoop(double ic, double v_ic, double kc, double lo, double hi, double ts, double ti);
	//virtual ~SimpleRateLoop();
};


class SimpleVelocityPILoop
{
private:
	double ts, ti;
	velocityPI velocityPI_Object;									// Create the discrete PI loop object
	SimpleVelocityPILoop();											// Forces a compiler error if you do not enter parameters
	SimpleVelocityPILoop(const SimpleVelocityPILoop&);				// ''
	SimpleVelocityPILoop& operator=(const SimpleVelocityPILoop&);	// Cannot call the assignment operator

public:
	double increment(double sp, double meas, double t, double dt);
	SimpleVelocityPILoop(double v_ic, double kc, double ts, double ti);
	//virtual ~SimpleRateLoop();
};

class PercentG_inProduct
{
	double pos_R, neg_R, lastTime = 0., lastOut, pctGsp;
	discretePI discretePI_Object;									// Create the discrete PI loop object
	double rateLimiter(double sp, double t, double dt);
public:
	double increment(double sp, double meas, double t, double dt);
	double getPctGsp(void) { return pctGsp; }
	PercentG_inProduct(double ic, double v_ic, double pos_R, double neg_R, double kc, double lo, double hi, double ts, double ti);
};

class ProductionRate
{
private:
	double pos_R, neg_R, lastTime = 0., lastOut, prodRate;
	discretePI discretePI_Object;									// Create the discrete PI loop object
	double rateLimiter(double sp, double t, double dt);
public:
	double increment(double sp, double meas, double t, double dt);
	double getProductionRate(void) { return prodRate; }
	ProductionRate(double ic, double v_ic, double pos_R, double neg_R, double kc, double lo, double hi, double ts, double ti);
};

#endif
