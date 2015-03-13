/*

* Author:       Rick Candell (rick.candell@nist.gov)
*               Tim Zimmerman (timothy.zimmerman@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

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

void TEController::initialize(double tstep, double tscan)
{
	// Create the "Manipulated Vairable" array
	m_xmv = new double[TEPlant::NU];
    double u0[NU] = {  62.8070,    // Initial D Feed Rate
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
	// Fill in the manipulated variables with the associated values
	for (int ii = 0; ii < TEPlant::NU; ii++) m_xmv[ii] = u0[ii];

	// Set the initial Production Rate
	Fp = 100.;
	// Create the ratio array with its initial conditions
	r = new double[7];
	double r_0[7] = {   0.251,   // 
                        3664,    // 
                        4509,    // 
                        9.35,    // 
                        0.337,   // 
                        25.16,   // 
                        22.95 }; // 
	for (int ii = 0; ii < 7; ii++) r[ii] = (r_0[ii] / Fp);

	// Create the control loops
	pctG_inProduct           = new PercentG_inProduct(0.6323, -0.5067, (0.3*22.95 / 24.), (-0.3*22.95 / 24.), -0.4, neg_inf, pos_inf, tstep, (100. / 60.));
	yA_ControlLoop           = new SimpleVelocityPILoop(0.1568, 2.0e-4, 0.1, 1.);
	yAC_ControlLoop          = new SimpleVelocityPILoop(-0.1936, 3.0e-4, 0.1, 2.);
	A_FeedRateLoop           = new SimpleDiscretePILoop(u0[2], 0.0027, 0.01, 0., 100., tstep, (0.001 / 60.));
	D_FeedRateLoop           = new SimpleDiscretePILoop(u0[0], 62.4296, 1.6e-6, 0., 100., tstep, (0.001 / 60.));
	E_FeedRateLoop           = new SimpleDiscretePILoop(u0[1], 9.7234, 1.8e-6, 0., 100., tstep, (0.001 / 60.));
	C_FeedRateLoop           = new SimpleDiscretePILoop(u0[3], 0.0835, 0.003, 0., 100., tstep, (0.001 / 60.));
	PurgeRateLoop            = new SimpleDiscretePILoop(u0[5], -0.0064, 0.01, 0., 100., tstep, (0.001 / 60.));
	productionRate           = new ProductionRate(-0.4489, -0.1407, (0.3*22.95 / 24.), (-0.3*22.95 / 24.), 3.2, -30., 30., tstep, (120. / 60.));
	ReactorLevelLoop         = new SimpleDiscretePILoop(92.02, 0.0408, 0.8, 0., 120., tstep, (60. / 60.));
	ReactorPressureLoop      = new SimpleDiscretePILoop(r_0[4], 1.0409, -1.0e-4, 0., 100., tstep, (20. / 60.));
	ReactorTemperatureLoop   = new SimpleDiscretePILoop(u0[9], 0.008, -8.0, 0., 100., tstep, (7.5 / 60.));
	SeparatorFlowLoop        = new SimpleDiscretePILoop(u0[6], 0.0481, 4.0e-4, 0., 100., tstep, (0.001 / 60.));
	SeparatorLevelLoop       = new SimpleDiscretePILoop(r_0[5], -1.0418, -1.0e-3, 0., 100., tstep, (200. / 60.));
	SeparatorTemperatureLoop = new SimpleDiscretePILoop(u0[10], -0.1416, -4.0, 0., 100., tstep, (15. / 60.));
	StripperFlowLoop         = new SimpleDiscretePILoop(u0[7], 0.0774, 4.0e-4, 0., 100., tstep, (0.001 / 60.));
	StripperLevelLoop        = new SimpleDiscretePILoop(r_0[6], 0.9492, -2.0e-4, 0., 100., tstep, (200. / 60.));
}

// run the controller one scan interval
double* TEController::increment(double t, double dt, double* new_xmeas)
{
	//
	dbl prodSP = 0, E_Adj, loop14 = 0, loop15 = 0;

	// Update the % G in product
	dbl pctG_SP;
	if (t < 24. || t > 100.) pctG_SP = 53.8;
	else pctG_SP = 65.;
	E_Adj = pctG_inProduct->increment(pctG_SP, new_xmeas[39], t, dt);

	// Update the A and C Measurements
	yAC = new_xmeas[22] + new_xmeas[24];
	yA = (new_xmeas[22] * 100.) / yAC;

	// Update the ratio trimming and the yA and yAC loops (every 0.1 of t)
	dbl tMod = fmod(t, 0.1);
	if ((last_tMod > tMod) || t == 0) {

		loop14 = yA_ControlLoop->increment(63.1372, yA, t, dt);
		loop15 = yAC_ControlLoop->increment(51., yAC, t, dt);

		if (t == 0)	{
			r0_unitDelay_out = 0.0027;
			r3_unitDelay_out = 0.0922;
			r0_unitDelay_store = loop14 + r0_unitDelay_out;
			r3_unitDelay_store = loop15 + r3_unitDelay_out;
		}
		else {
			r0_unitDelay_out = r0_unitDelay_store;
			r3_unitDelay_out = r3_unitDelay_store;
			r0_unitDelay_store = loop14 + r0_unitDelay_out;
			r3_unitDelay_store = loop15 + r3_unitDelay_out;
		}
		r[0] = loop14 + r0_unitDelay_out;
		r[3] = (-1. * loop14) + loop15 + r3_unitDelay_out;
	}
	last_tMod = tMod;

	// Update the feed rate loops AFTER the production rate loop!
	// Update the feed rate loops AFTER the ratio trimming loop!
	m_xmv[2] = A_FeedRateLoop->increment((r[0] * Fp), new_xmeas[0], t, dt);
	m_xmv[0] = D_FeedRateLoop->increment((r[1] * Fp), new_xmeas[1], t, dt);
	m_xmv[1] = E_FeedRateLoop->increment((r[2] * Fp), new_xmeas[2], t, dt);
	m_xmv[3] = C_FeedRateLoop->increment((r[3] * Fp), new_xmeas[3], t, dt);

	// Update the production rate
	Fp = productionRate->increment(22.89, new_xmeas[16], t, dt);

	m_xmv[5] = PurgeRateLoop->increment((r[4] * Fp), new_xmeas[9], t, dt);

	// Update the feedforward
	mP_G_SP = pctG_inProduct->getPctGsp();
	r[1] = (double)((1.5192e-3 * pow(mP_G_SP, 2.)) + (5.9446e-1 * mP_G_SP) + 2.7690e-1) - ((E_Adj * 32.) / Fp);
	r[2] = (double)((-1.1377e-3 * pow(mP_G_SP, 2.)) + (-8.0893e-1 * mP_G_SP) + 9.1060e1) + ((E_Adj * 46.) / Fp);

	// Update the Reactor
	dbl sepTempSP = ReactorLevelLoop->increment(65., new_xmeas[7], t, dt);
	r[4] = ReactorPressureLoop->increment(2800., new_xmeas[6], t, dt);
	m_xmv[9] = ReactorTemperatureLoop->increment(122.9, new_xmeas[8], t, dt);

	// Update the Separator
	m_xmv[6] = SeparatorFlowLoop->increment((r[5] * Fp), new_xmeas[13], t, dt);
	r[5] = SeparatorLevelLoop->increment(50., new_xmeas[11], t, dt);
	m_xmv[10] = SeparatorTemperatureLoop->increment(sepTempSP, new_xmeas[10], t, dt);

	// Update the Stripper
	m_xmv[7] = StripperFlowLoop->increment((r[6] * Fp), new_xmeas[16], t, dt);
	r[6] = StripperLevelLoop->increment(50., new_xmeas[14], t, dt);

	return m_xmv;
}

std::ostream& operator<< (std::ostream& lhs, const TEController& rhs)
{
	for (int ii = 0; ii < TEPlant::NU; ii++)
	{
		lhs << rhs.m_xmv[ii] << "\t";
	}

	for (int ii = 0; ii < 7; ii++)
	{
		lhs << rhs.r[ii] << "\t";
	}

	lhs << rhs.mP_G_SP << "\t" << rhs.productionRate->getProductionRate();
	return lhs;
}