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

void TEController::initialize(double tscan)
{
	// default setpoints 
	_prod_rate_sp = 22.89;
	_reactor_pressure_sp = 2800.0;
	_reactor_level_sp = 65.0;
	_reactor_temp_sp = 122.9;
	_pctg_sp = 53.8;
	_sep_level_sp = 50.0;
	_stripper_level_sp = 50.0;

	// measured variables
	m_xmeas = new double[TEPlant::NY]();
	get_curr_xmeas(m_xmeas);

	// Create the "Manipulated Vairable" array
	m_xmv = new double[TEPlant::NU];
	double u0[NU] = {   62.8069839011592,    // Initial D Feed Rate
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

	// Fill in the manipulated variables with the associated values
	for (int ii = 0; ii < TEPlant::NU; ii++) m_xmv[ii] = u0[ii];

	// Set the initial Production Rate
	Fp = 100.;
	// Create the ratio array with its initial conditions
	r = new double[7];
	double r_0[7] = {   0.0027228362442929,  // 
                        36.452848893533,     // 
                        44.538708121546,     // 
                        0.092303538366845,   // 
						0.001915509802465,   // 
						0.253705005583181,   // 
						0.228711771174511 }; // 
	for (int ii = 0; ii < 7; ii++) r[ii] = r_0[ii];

	// Create the control loops
	pctG_inProduct           = new PercentG_inProduct(0.632250244508519, -0.506656747709997, (0.3*22.95 / 24.), (-0.3*22.95 / 24.), -0.4, neg_inf, pos_inf, tscan, (100. / 60.));
	yA_ControlLoop           = new SimpleVelocityPILoop(0.156799893651460, 2.0e-4, 0.1, 1.);
	yAC_ControlLoop          = new SimpleVelocityPILoop(-0.193592358293763, 3.0e-4, 0.1, 2.);
	A_FeedRateLoop           = new SimpleDiscretePILoop(u0[2], 0.002694050999489, 0.01, 0., 100., tscan, (0.001 / 60.));
	D_FeedRateLoop           = new SimpleDiscretePILoop(u0[0], 62.429570964992760, 1.6e-6, 0., 100., tscan, (0.001 / 60.));
	E_FeedRateLoop           = new SimpleDiscretePILoop(u0[1], 9.723386274665245, 1.8e-6, 0., 100., tscan, (0.001 / 60.));
	C_FeedRateLoop           = new SimpleDiscretePILoop(u0[3], 0.083514558990949, 0.003, 0., 100., tscan, (0.001 / 60.));
	PurgeRateLoop            = new SimpleDiscretePILoop(u0[5], -0.006360143307706, 0.01, 0., 100., tscan, (0.001 / 60.));
	productionRate           = new ProductionRate(-0.448883361482739, -0.140664345075038, (0.3*22.95 / 24.), (-0.3*22.95 / 24.), 3.2, -30., 30., tscan, (120. / 60.));
	ReactorLevelLoop         = new SimpleDiscretePILoop(92.020048977831070, 0.040751615684570, 0.8, 0., 120., tscan, (60. / 60.));
	ReactorPressureLoop      = new SimpleDiscretePILoop(r_0[4], 1.040853861903997, -1.0e-4, 0., 100., tscan, (20. / 60.));
	ReactorTemperatureLoop   = new SimpleDiscretePILoop(u0[9], 0.007961389832360, -8.0, 0., 100., tscan, (7.5 / 60.));
	SeparatorFlowLoop        = new SimpleDiscretePILoop(u0[6], 0.048094569533532, 4.0e-4, 0., 100., tscan, (0.001 / 60.));
	SeparatorLevelLoop       = new SimpleDiscretePILoop(r_0[5], -1.041770047054492, -1.0e-3, 0., 100., tscan, (200. / 60.));
	SeparatorTemperatureLoop = new SimpleDiscretePILoop(u0[10], -0.141566616033629, -4.0, 0., 100., tscan, (15. / 60.));
	StripperFlowLoop         = new SimpleDiscretePILoop(u0[7], 0.077381965807636, 4.0e-4, 0., 100., tscan, (0.001 / 60.));
	StripperLevelLoop        = new SimpleDiscretePILoop(r_0[6], 0.949174713615896, -2.0e-4, 0., 100., tscan, (200. / 60.));
}

// run the controller one scan interval
double* TEController::increment(double t, double dt, double* new_xmeas)
{
	// store the measures vars locally  
	//  TODO: these will eventually be used for the basis of calculation
	std::memcpy(m_xmeas, new_xmeas, TEPlant::NY*sizeof(double));

	//
	double E_Adj, loop14 = 0, loop15 = 0;

	// Update the % G in product
	E_Adj = pctG_inProduct->increment(_pctg_sp, new_xmeas[39], t, dt);

	// Update the A and C Measurements
	yAC = new_xmeas[22] + new_xmeas[24];
	yA = (new_xmeas[22] * 100.) / yAC;

	// Update the ratio trimming and the yA and yAC loops (every 0.1 of t)
	double tMod = fmod(t, 0.1);
	if ((tMod < 0.00005) || (0.1 - tMod) < 0.00005 || t == 0) {

		// TODO: Verify how these constants are determined and 
		//       how changing other setpoints may affect them
		loop14 = yA_ControlLoop->increment(63.1372, yA, t, dt);
		loop15 = yAC_ControlLoop->increment(51., yAC, t, dt);

		if (t == 0)	{
			r0_unitDelay_out = 0.002722836244293;
			r3_unitDelay_out = 0.092236609900323;
			r0_unitDelay_store = loop14 + r0_unitDelay_out;
			r3_unitDelay_store = (-1. * loop14) + loop15 + r3_unitDelay_out;
		}
		else {
			r0_unitDelay_out = r0_unitDelay_store;
			r3_unitDelay_out = r3_unitDelay_store;
			r0_unitDelay_store = loop14 + r0_unitDelay_out;
			r3_unitDelay_store = (-1. * loop14) + loop15 + r3_unitDelay_out;
		}
		r[0] = loop14 + r0_unitDelay_out;
		r[3] = (-1. * loop14) + loop15 + r3_unitDelay_out;
	}
	last_tMod = tMod;

	// Update the production rate
	Fp = productionRate->increment(_prod_rate_sp, new_xmeas[16], t, dt);

	// Update the feedforward
	mP_G_SP = pctG_inProduct->getPctGsp();
	r[1] = (double)((1.5192e-3 * pow(mP_G_SP, 2.)) + (5.9446e-1 * mP_G_SP) + 2.7690e-1) - ((E_Adj * 32.) / Fp);
	r[2] = (double)((-1.1377e-3 * pow(mP_G_SP, 2.)) + (-8.0893e-1 * mP_G_SP) + 9.1060e1) + ((E_Adj * 46.) / Fp);

	// Update the feed rate loops AFTER the production rate loop!
	// Update the feed rate loops AFTER the ratio trimming loop!
	m_xmv[2] = A_FeedRateLoop->increment((r[0] * Fp), new_xmeas[0], t, dt);
	m_xmv[0] = D_FeedRateLoop->increment((r[1] * Fp), new_xmeas[1], t, dt);
	m_xmv[1] = E_FeedRateLoop->increment((r[2] * Fp), new_xmeas[2], t, dt);
	m_xmv[3] = C_FeedRateLoop->increment((r[3] * Fp), new_xmeas[3], t, dt);

	r[4] = ReactorPressureLoop->increment(_reactor_pressure_sp, new_xmeas[6], t, dt);
	m_xmv[5] = PurgeRateLoop->increment((r[4] * Fp), new_xmeas[9], t, dt);
	double sepTempSP = ReactorLevelLoop->increment(_reactor_level_sp, new_xmeas[7], t, dt);
	m_xmv[9] = ReactorTemperatureLoop->increment(_reactor_temp_sp, new_xmeas[8], t, dt);

	r[5] = SeparatorLevelLoop->increment(_sep_level_sp, new_xmeas[11], t, dt);
	m_xmv[6] = SeparatorFlowLoop->increment((r[5] * Fp), new_xmeas[13], t, dt);
	m_xmv[10] = SeparatorTemperatureLoop->increment(sepTempSP, new_xmeas[10], t, dt);

	r[6] = StripperLevelLoop->increment(_stripper_level_sp, new_xmeas[14], t, dt);
	m_xmv[7] = StripperFlowLoop->increment((r[6] * Fp), new_xmeas[16], t, dt);

	return m_xmv;
}

std::ostream& operator<< (std::ostream& lhs, const TEController& rhs)
{
	// manipulated variables
	for (int ii = 0; ii < TEPlant::NU; ii++)
	{
		lhs << rhs.m_xmv[ii] << "\t";
	}

	// measured variables
	for (int ii = 0; ii < TEPlant::NY; ii++)
	{
		lhs << rhs.m_xmeas[ii] << "\t";
	}

	// r state variables
	for (int ii = 0; ii < 7; ii++)
	{
		lhs << rhs.r[ii] << "\t";
	}

	// production rate and percentGSP
	lhs << rhs.mP_G_SP << "\t" << rhs.productionRate->getProductionRate();

	return lhs;
}


