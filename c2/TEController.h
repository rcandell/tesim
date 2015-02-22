/*

* Author: Rick Candell (rick.candell@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License: Public Domain

*/

#ifndef __TE_CONTROLLER_H__
#define __TE_CONTROLLER_H__

#include "TEPlant.h"

class TEController
{
public:

	static TEController* getInstance();
	virtual ~TEController();

	void initialize();

	// run the controller one scan interval
	// returns new manipulated variables
	double* increment(double t, double dt, double* xmeas);

private:

	TEController() {};
	TEController(const TEController&);
	TEController& operator=(const TEController&);

	static TEController*    instance;

	// input/output variables
	double *m_xmv;

};


#endif
