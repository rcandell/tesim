/* 

* Author:       Rick Candell (rick.candell@nist.gov)
*               Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include <iostream>     // std::cout, std::ostream, std::ios
#include <cstring>

#include "teprob.h"
#include "TEPlant.h"

const long int TEPlant::NX = 50;			// number of states
const long int TEPlant::NU = 12;			// number of xmv
const long int TEPlant::NY = 41;			// number of xmeas
const long int TEPlant::NIDV = 20;			// number of disturbance types

TEPlant* TEPlant::instance = 0;

TEPlant* TEPlant::getInstance()
{
	if (TEPlant::instance == 0)
	{
		TEPlant::instance = new TEPlant;
	}
    return TEPlant::instance;
}

TEPlant::~TEPlant()
{
	delete TEPlant::instance;
	TEPlant::instance = 0;
}

// initialize the simulation
void TEPlant::initialize()
{
	double t = 0.0;
	m_x		= new double[TEPlant::NX]();	// the states
	m_dxdt	= new double[TEPlant::NX]();	// the state derivatives
	m_xmeas = new double[TEPlant::NY]();	// measured
	m_xmv	= new double[TEPlant::NU]();	// manipulated
	m_idv   = new int[TEPlant::NIDV]();	    // idv"s

	m_t = 0.0;

	// Turn off disturbances
	set_curr_idv(m_idv);

	teinit(&TEPlant::NX, &m_t, m_x, m_dxdt);

	// initialize states
	m_x[0] = (float)11.952176106184915;
	m_x[1] = (float)7.950300220587021;
	m_x[2] = (float)4.868453724881999;
	m_x[3] = (float)0.272972868512175;
	m_x[4] = (float)18.170957937566698;
	m_x[5] = (float)6.075685760535071;
	m_x[6] = (float)1.387724744623962e+02;
	m_x[7] = (float)1.361280597442423e+02;
	m_x[8] = (float)2.524050242106403;
	m_x[9] = (float)62.627138395126366;
	m_x[10] = (float)41.656312838317460;
	m_x[11] = (float)25.522301888401437;
	m_x[12] = (float)0.151685102518388;
	m_x[13] = (float)10.746156544195594;
	m_x[14] = (float)3.593371216635020;
	m_x[15] = (float)52.279082275951870;
	m_x[16] = (float)41.117503515415950;
	m_x[17] = (float)0.648822831791654;
	m_x[18] = (float)0.424789724873510;
	m_x[19] = (float)0.007882706366504;
	m_x[20] = (float)0.893372204888619;
	m_x[21] = (float)0.009602793856226;
	m_x[22] = (float)0.515068583569485;
	m_x[23] = (float)0.166817153416254;
	m_x[24] = (float)48.183122896125950;
	m_x[25] = (float)39.420524571989105;
	m_x[26] = (float)0.380153203870486;
	m_x[27] = (float)1.136841584633928e+02;
	m_x[28] = (float)52.611730377082860;
	m_x[29] = (float)66.661346315892740;
	m_x[30] = (float)21.212165336364368;
	m_x[31] = (float)58.941968290602770;
	m_x[32] = (float)14.336659127811398;
	m_x[33] = (float)17.649098880625942;
	m_x[34] = (float)8.512173265498266;
	m_x[35] = (float)1.137989657795421;
	m_x[36] = (float)1.024800282325948e+02;
	m_x[37] = (float)92.262530705397240;
	m_x[38] = (float)62.806559460324160;
	m_x[39] = (float)53.286026966911585;
	m_x[40] = (float)26.661254009105340;
	m_x[41] = (float)60.484801083141220;
	m_x[42] = (float)0.0;
	m_x[43] = (float)24.234688815797390;
	m_x[44] = (float)37.209102766947350;
	m_x[45] = (float)46.430892344071620;
	m_x[46] = (float)0.0;
	m_x[47] = (float)35.944588369409736;
	m_x[48] = (float)12.209547880014606;
	m_x[49] = (float)100.;
	std::memset(m_dxdt, 0, TEPlant::NX*sizeof m_x);

	// initialize the plant inputs for good measure
    double u0[NU] = { 62.8069839011592,    // Initial D Feed Rate
                      53.2867088613521,    // Initial E Feed Rate
                      26.6621725633624,    // Initial A Feed Rate
                      60.4828539928762,    // Initial C Feed Rate
                      0.,         // Recycle Valve Position
                      24.2293009065651,    // Initial Purge Rate
                      37.2081981280914,    // Initial Separator Flow Rate
                      46.4305263951036,    // Initial Stripper Flow Rate
                      0.,         // Steam Valve Position
                      35.8653215123278,    // Initial Reactor Temperature
                      12.9306420475438,    // Initial Separator Temperature
                      100. };     // Agitator Setting

	std::memcpy(m_xmv, u0, NU*sizeof(double));

	// We have to run the plant one iteration with the m_x values above in order to
	// obtain the correct xmeas values for the controller"s first iteration.
	tefunc(&TEPlant::NX, &m_t, m_x, m_dxdt);
}

// run the plant one time step
// reference: http://www.mathworks.com/help/simulink/sfg/how-s-functions-work.html
//  the plant incrementation operates like simulink solver with euler method
double* TEPlant::increment(double t, double dt, double* new_xmv, int* shutdown)
{
	// update time
	m_t = t;

	// copy new xmv to the stored xmv
	std::memcpy(m_xmv, new_xmv, TEPlant::NU*sizeof(double));

	// apply the new the inputs
	set_curr_xmv(m_xmv);
	set_curr_idv(m_idv);

	// update the model outputs and compute derivatives
	tefunc(&TEPlant::NX, &t, m_x, m_dxdt);

	// integrate
	TEPlant::euler(TEPlant::NX, t, dt, m_x, m_dxdt);

	// fetch the next outputs
	get_curr_xmeas(m_xmeas);

	// Calculate dxdt
	tefunc(&TEPlant::NX, &t, m_x, m_dxdt);

	// update cost
	updateHourlyCost(m_xmeas, m_xmv);

	// check if plant has shutdown
	*shutdown = get_curr_shutdown();
	if (*shutdown > 0)
	{
		ShutdownException shutdown_ex(*shutdown, get_shutdown_msg());
		throw shutdown_ex;
	}

	return m_xmeas;
}

void TEPlant::updateHourlyCost(double* xmeas, double* xmv)
{
	// stage 0
	static const double gain0[] = { 0.0318, 0.0536 };
	// stage 1
	static const double gain1 = 0.44791;
	// stage 2
	static const double gain2[] = { 2.209, 6.177, 22.06, 14.56, 17.89, 30.44, 22.94 };
	// stage 3
	static const double gain3[] = {0.2206, 0.1456, 0.1789};
	// stage 4
	static const double gain4 = 4.541;

	m_hourlyCost =
	
		// cost to produce steam and deliver it to stripper
		(gain0[0]*m_xmeas[18] + gain0[1]*m_xmeas[19]) +

		// cost from purge of unused materials
		gain1*m_xmeas[9] * (gain2[0]*m_xmeas[28] + gain2[1]*m_xmeas[30] + gain2[2]*m_xmeas[31] + gain2[3]*m_xmeas[32] + gain2[4]*m_xmeas[33] + gain2[5]*m_xmeas[34] + gain2[6]*m_xmeas[35]) +

		// cost due to flow of desired product materials from the system
		(gain3[0]*m_xmeas[36] + gain3[1]*m_xmeas[37] + gain3[2]*m_xmeas[38]) * gain4*m_xmv[7]; 
}

// get for xmeas
const double* TEPlant::get_xmeas() const
{
	get_curr_xmeas(m_xmeas);
	return m_xmeas;
}

void TEPlant::euler(int nn, double t, double dt, double* yy, double* yp)
{
	for (int ii = 0; ii < nn; ii++)
	{
		yy[ii] = yy[ii] + yp[ii] * dt;
	}
}

const int TEPlant::get_shutdown() const
{
	return get_curr_shutdown();
}

char* TEPlant::shutdown_msg() const
{
	return get_shutdown_msg();
}

std::ostream& operator<< (std::ostream& lhs, const TEPlant& rhs)
{
	// manipulated variables
	for (int ii = 0; ii < TEPlant::NU-1; ii++)
	{
		lhs << rhs.m_xmv[ii] << "\t";
	}
	lhs << rhs.m_xmv[TEPlant::NU - 1] << "\t";

	// measured variables
	for (int ii = 0; ii < TEPlant::NY - 1; ii++)
	{
		lhs << rhs.m_xmeas[ii] << "\t";
	}
	lhs << rhs.m_xmeas[TEPlant::NY - 1] << "\t";

	// hourly cost
	lhs << rhs.m_hourlyCost << "\t";

	// disturbance vector
	unsigned idv = 0;
	for (int ii = 0; ii < TEPlant::NIDV; ii++)
	{
		idv += static_cast<unsigned>(std::pow(2, ii)*rhs.m_idv[ii]);
	}
	lhs << idv;

	return lhs;
}

