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

	enum Setpoints
	{
		PROD_RATE,
		REACTOR_PRESS,
		REACTOR_LEVEL,
		REACTOR_TEMP,
		PCTG,
		SEP_LEVEL,
		STRIP_LEVEL
	};

	static TEController* getInstance();
	virtual ~TEController();

	void initialize(double tscan);

	// run the controller one scan interval
	// returns new manipulated variables
	double* increment(double t, double dt, double* xmeas);

	// set and get for xmv
	const double* get_xmv() const { return m_xmv; }
	const double  get_xmv(unsigned idx) const { return m_xmv[idx]; }
	void set_xmv(const unsigned idx, const double xmv) { m_xmv[idx] = xmv; }

	// overloaded output stream for the controller
	friend std::ostream& operator<< (std::ostream&, const TEController&);

	// setpoint accessors
	void prod_rate_sp(double prod_rate_sp)		{ _prod_rate_sp = prod_rate_sp; }
	const double* prod_rate_sp() const					{ return &_prod_rate_sp; }
	
	void reactor_pressure_sp(double sp)			{ _reactor_pressure_sp = sp; }
	const double* reactor_pressure_sp() const			{ return &_reactor_pressure_sp; }

	void reactor_level_sp(double sp)			{ _reactor_level_sp = sp; }
	const double* reactor_level_sp() const				{ return &_reactor_level_sp; }
	
	void reactor_temp_sp(double sp)				{ _reactor_temp_sp = sp;  }
	const double* reactor_temp_sp() const				{ return &_reactor_temp_sp; }
	
	void sep_level_sp(double sp)				{ _sep_level_sp = sp; }
	const double* sep_level_sp() const					{ return &_sep_level_sp; }
	
	void strip_level_sp(double sp)				{ _stripper_level_sp = sp; }
	const double* strip_level_sp() const				{ return &_stripper_level_sp; }
	
	void pctg_sp(double sp)						{ _pctg_sp = sp; }
	const double* pctg_sp() const						{ return &_pctg_sp; }

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
	double *m_xmeas;
	double *m_xmv;
	double *r;
	double Fp;

	//set points
	double _prod_rate_sp, _reactor_pressure_sp, _reactor_level_sp, _reactor_temp_sp,
		_pctg_sp, _sep_level_sp, _stripper_level_sp;

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
