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
	m_idv   = new int[TEPlant::NIDV]();	    // idv's

	// initialize states
	m_x[0] = (float)11.9522;
	m_x[1] = (float)7.9503;
	m_x[2] = (float)4.8685;
	m_x[3] = (float)0.2730;
	m_x[4] = (float)18.1710;
	m_x[5] = (float)6.0757;
	m_x[6] = (float)138.7725;
	m_x[7] = (float)136.1281;
	m_x[8] = (float)2.5241;
	m_x[9] = (float)62.6271;
	m_x[10] = (float)41.6563;
	m_x[11] = (float)25.5223;
	m_x[12] = (float)0.1517;
	m_x[13] = (float)10.7462;
	m_x[14] = (float)3.5934;
	m_x[15] = (float)52.2791;
	m_x[16] = (float)41.1175;
	m_x[17] = (float)0.6488;
	m_x[18] = (float)0.4248;
	m_x[19] = (float)0.0079;
	m_x[20] = (float)0.8934;
	m_x[21] = (float)0.0096;
	m_x[22] = (float)0.5151;
	m_x[23] = (float)0.1668;
	m_x[24] = (float)48.1831;
	m_x[25] = (float)39.4205;
	m_x[26] = (float)0.3802;
	m_x[27] = (float)113.6842;
	m_x[28] = (float)52.6117;
	m_x[29] = (float)66.6613;
	m_x[30] = (float)21.2122;
	m_x[31] = (float)58.9420;
	m_x[32] = (float)14.3367;
	m_x[33] = (float)17.6491;
	m_x[34] = (float)8.5122;
	m_x[35] = (float)1.1380;
	m_x[36] = (float)102.4800;
	m_x[37] = (float)92.2625;
	m_x[38] = (float)62.8066;
	m_x[39] = (float)53.2860;
	m_x[40] = (float)26.6613;
	m_x[41] = (float)60.4848;
	m_x[42] = (float)4.4e-323;
	m_x[43] = (float)24.2347;
	m_x[44] = (float)37.2091;
	m_x[45] = (float)46.4309;
	m_x[46] = (float)8.2000e-322;
	m_x[47] = (float)35.9446;
	m_x[48] = (float)12.2095;
	m_x[49] = (float)100.;
	std::memset(m_dxdt, 0, TEPlant::NX*sizeof m_x);

	// initialize the plant inputs for good measure
    double u0[NU] = { 62.8070,    // Initial D Feed Rate
                      53.2867,    // Initial E Feed Rate
                      26.6622,    // Initial A Feed Rate
                      60.4829,    // Initial C Feed Rate
                      0.,         // Recycle Valve Position
                      24.2293,    // Initial Purge Rate
                      37.2082,    // Initial Separator Flow Rate
                      46.4305,    // Initial Stripper Flow Rate
                      0.,         // Steam Valve Position
                      35.8653,    // Initial Reactor Temperature
                      12.9306,    // Initial Separator Temperature
                      100. };     // Agitator Setting

	std::memcpy(m_xmv, u0, NU*sizeof(double));

	//int teinit(const integer *nn, doublereal *time, doublereal *yy, doublereal *yp);
	m_t = 0.0;
	teinit(&TEPlant::NX, &m_t, m_x, m_dxdt);
}

// run the plant one time step
// reference: http://www.mathworks.com/help/simulink/sfg/how-s-functions-work.html
//  the plant incrementation operates like simulink solver with euler method
double* TEPlant::increment(double t, double dt, double* new_xmv)
{
	// update time
	m_t = t;

	// copy new xmv to the stored xmv
	std::memcpy(m_xmv, new_xmv, TEPlant::NU*sizeof(double));

	// apply the new the inputs
	set_curr_xmv(m_xmv);

	// update the model outputs and compute derivatives
	tefunc(&TEPlant::NX, &t, m_x, m_dxdt);

	// integrate
	TEPlant::euler(TEPlant::NX, t, dt, m_x, m_dxdt);

	// fetch the next outputs
	get_curr_xmeas(m_xmeas);
	return m_xmeas;
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

std::ostream& operator<< (std::ostream& lhs, const TEPlant& rhs)
{
	for (int ii = 0; ii < TEPlant::NY; ii++)
	{
		lhs << rhs.m_xmeas[ii] << "\t";
	}
	return lhs;
}

