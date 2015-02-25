/*

* Author: Rick Candell (rick.candell@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License: Public Domain

*/

#include "TEController.h"
#include "teprob.h"

TEController* TEController::instance = 0;

TEController* TEController::getInstance()
{
	if (TEController::instance == 0)
	{
		TEController::instance = new TEController;
	}
	return TEController::instance;
}

TEController::~TEController()
{
	delete TEController::instance;
	TEController::instance = 0;
}

void TEController::initialize()
{
	m_xmv = new double[TEPlant::NU];	// manipulated
	double u0[NU] = { 63.053, 53.98, 24.644, 61.302, 22.21, 
					  40.064, 38.10, 46.534, 47.446, 41.106, 
					  18.114, 100};  // TODO: verify with simulink model

	for (int ii = 0; ii < TEPlant::NU; ii++)
	{
		m_xmv[ii] = u0[ii];
	}


	// set the initial conditions of feedback controllers

}

// run the controller one scan interval
double* TEController::increment(double t, double dt, double* new_xmeas)
{
	// todo: run the controller
	// ... (port the Ricker controller to code here)
	// get xmv's
	// ...
	return m_xmv;
}