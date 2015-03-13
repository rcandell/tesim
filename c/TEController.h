/*

* Author:       Rick Candell (rick.candell@nist.gov)
*               Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#ifndef __TE_CONTROLLER_H__
#define __TE_CONTROLLER_H__

#include "TEPlant.h"
#include <math.h>
#include "ControlLoops.h"

#define pos_inf 1.79769+308
#define neg_inf (pos_inf * -1)

class TEController
{
public:

	static TEController* getInstance();
	virtual ~TEController();

	void initialize(double tstep, double tscan);

	// run the controller one scan interval
	// returns new manipulated variables
	double* increment(double t, double dt, double* xmeas);

	friend std::ostream& operator<< (std::ostream&, const TEController&);

private:

	TEController() {};
	TEController(const TEController&);
	TEController& operator=(const TEController&);

	static TEController*    instance;

	double      yA, yAC, mP_G_SP,
                r0_unitDelay_out, 
                r0_unitDelay_store, 
                r1_unitDelay, 
                r3_unitDelay_out, 
                r3_unitDelay_store, 
                r4_unitDelay, 
                last_tMod;

	// input/output variables
	double *m_xmv;
	double *r;
	double Fp;

	// Loop Pointers
	SimpleDiscretePILoop* A_FeedRateLoop;
	SimpleDiscretePILoop* D_FeedRateLoop;
	SimpleDiscretePILoop* E_FeedRateLoop;
	SimpleDiscretePILoop* C_FeedRateLoop;
	SimpleDiscretePILoop* PurgeRateLoop;

	SimpleDiscretePILoop* SeparatorFlowLoop;
	SimpleDiscretePILoop* SeparatorLevelLoop;
	SimpleDiscretePILoop* SeparatorTemperatureLoop;

	SimpleDiscretePILoop* StripperFlowLoop;
	SimpleDiscretePILoop* StripperLevelLoop;

	SimpleDiscretePILoop* ReactorLevelLoop;
	SimpleDiscretePILoop* ReactorPressureLoop;
	SimpleDiscretePILoop* ReactorTemperatureLoop;

	SimpleVelocityPILoop* yA_ControlLoop;
	SimpleVelocityPILoop* yAC_ControlLoop;

	PercentG_inProduct*	  pctG_inProduct;
	ProductionRate*	      productionRate;
};


#endif
