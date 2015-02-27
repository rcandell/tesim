/*

* Author: Rick Candell (rick.candell@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License: Public Domain

*/

#ifndef __TE_CONTROLLER_RICKER_H__
#define __TE_CONTROLLER_RICKER_H__

#include "TEPlant.h"
#include "TEController.h"
#include "TEControllerRicker.h"

class TEControllerRicker : public TEController
{
public:
	static TEController* getInstance();
	virtual ~TEControllerRicker();

private:

	TEControllerRicker();
	TEControllerRicker(const TEControllerRicker&);
	TEControllerRicker& operator=(const TEControllerRicker&);

	// run the controller one scan interval
	// returns new manipulated variables
	double* increment(double t, double dt, double* xmeas);

};

#endif