/* 

* Author: Rick Candell (rick.candell@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License: Public Domain

*/

#include <iostream>     // std::cout, std::ostream, std::ios

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
	m_x		= new double[TEPlant::NX];	// the states
	m_dxdt	= new double[TEPlant::NX];	// the state derivatives
	m_xmeas = new double[TEPlant::NY];	// measured
	m_xmv	= new double[TEPlant::NU];	// manipulated
	m_idv = new int[TEPlant::NIDV];	// idv's

	// idv vector
	for (int ii = 0; ii < TEPlant::NIDV; ii++)
	{
		m_idv[ii] = 0;
	}

	// initialize states
	m_x[0] = (float)10.40491389;
	m_x[1] = (float)4.363996017;
	m_x[2] = (float)7.570059737;
	m_x[3] = (float).4230042431;
	m_x[4] = (float)24.15513437;
	m_x[5] = (float)2.942597645;
	m_x[6] = (float)154.3770655;
	m_x[7] = (float)159.186596;
	m_x[8] = (float)2.808522723;
	m_x[9] = (float)63.75581199;
	m_x[10] = (float)26.74026066;
	m_x[11] = (float)46.38532432;
	m_x[12] = (float).2464521543;
	m_x[13] = (float)15.20484404;
	m_x[14] = (float)1.852266172;
	m_x[15] = (float)52.44639459;
	m_x[16] = (float)41.20394008;
	m_x[17] = (float).569931776;
	m_x[18] = (float).4306056376;
	m_x[19] = .0079906200783;
	m_x[20] = (float).9056036089;
	m_x[21] = .016054258216;
	m_x[22] = (float).7509759687;
	m_x[23] = .088582855955;
	m_x[24] = (float)48.27726193;
	m_x[25] = (float)39.38459028;
	m_x[26] = (float).3755297257;
	m_x[27] = (float)107.7562698;
	m_x[28] = (float)29.77250546;
	m_x[29] = (float)88.32481135;
	m_x[30] = (float)23.03929507;
	m_x[31] = (float)62.85848794;
	m_x[32] = (float)5.546318688;
	m_x[33] = (float)11.92244772;
	m_x[34] = (float)5.555448243;
	m_x[35] = (float).9218489762;
	m_x[36] = (float)94.59927549;
	m_x[37] = (float)77.29698353;
	m_x[38] = (float)63.05263039;
	m_x[39] = (float)53.97970677;
	m_x[40] = (float)24.64355755;
	m_x[41] = (float)61.30192144;
	m_x[42] = (float)22.21;
	m_x[43] = (float)40.06374673;
	m_x[44] = (float)38.1003437;
	m_x[45] = (float)46.53415582;
	m_x[46] = (float)47.44573456;
	m_x[47] = (float)41.10581288;
	m_x[48] = (float)18.11349055;
	m_x[49] = (float)50.;

	//int teinit(const integer *nn, doublereal *time, doublereal *yy, doublereal *yp);
	t = 0.0;
	teinit(&TEPlant::NX, &t, m_x, m_dxdt);
}

// run the plant one time step
// reference: http://www.mathworks.com/help/simulink/sfg/how-s-functions-work.html
//  the plant incrementation operates like simulink solver with euler method
double* TEPlant::increment(double t, double dt, double* new_xmv)
{
	// apply the new the inputs
	set_curr_idv(m_idv);
	set_curr_xmv(m_xmv); 

	// update the model outputs and compute derivatives
	tefunc(&TEPlant::NX, &t, m_x, m_dxdt);

	// integrate
	euler(TEPlant::NX, t, dt, m_x, m_dxdt);

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