/*

* Author:       Rick Candell (rick.candell@nist.gov)

* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include  "TENames.h"
#include "TEPlant.h"

const char * TENames::xmeas_pq::names[] = { "xmeas_p", "xmeas_q" };

const char * TENames::xmv_pq::names[] = { "xmv_p", "xmv_q" };

const char * TENames::xmeas::names[] = {
	"A Feed (kscmh)",
	"D Feed (kg/hr)",
	"E Feed (kg/hr)",
	"A+C Feed (kscmh)",
	"Recycle Flow (kscmh)",
	"Reactor Feed (kscmh)",
	"Reactor Pressure (kPa)",
	"Reactor Level (%)",
	"Reactor Temp. (C)",
	"Purge Rate (kscmh)",
	"Sep Temp. (°C)",
	"Sep Level (%)",
	"Sep Pressure (kPa)",
	"Sep Underflow (m3/hr)",
	"Stripper Level (%)",
	"Stripper Pressure (kPa)",
	"Stripper Underflow (m3/hr)",
	"Stripper Temp. (C)",
	"Steam Flow (kg/hr)",
	"Compressor Work (kW)",
	"React. Cool. Temp. (C)",
	"Cond. Cool. Temp. (C)",
	"Feed A (mole %)",
	"Feed B (mole %)",
	"Feed C (mole %)",
	"Feed D (mole %)",
	"Feed E (mole %)",
	"Feed F (mole %)",
	"Purge     % A (mole %)",
	"Purge     % B (mole %)",
	"Purge     % C (mole %)",
	"Purge     % D (mole %)",
	"Purge     % E (mole %)",
	"Purge     % F (mole %)",
	"Purge     % G (mole %)",
	"Purge     % H (mole %)",
	"Product D (mole %)",
	"Product E (mole %)",
	"Product F (mole %)",
	"Product G (mole %)",
	"Product H (mole %)"
};

const char * TENames::xmv::names[] = {
	"D Feed",
	"E Feed",
	"A Feed",
	"A+C Feed",
	"Recycle Valve",
	"Purge Valve",
	"Separator Valve",
	"Stripper Valve",
	"Steam valve",
	"Reactor Coolant",
	"Condenser Coolant",
	"Agitator Speed"
};

std::ostream& operator<< (std::ostream& lhs, const TENames::xmeas& rhs)
{
	// measured variables
	for (int ii = 0; ii < TEPlant::NY - 1; ii++)
	{
		lhs << TENames::xmeas::names[ii] << "\t";
	}
	lhs << TENames::xmeas::names[TEPlant::NY - 1];
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::xmv& rhs)
{
	// manipulated variables
	for (int ii = 0; ii < TEPlant::NU - 1; ii++)
	{
		lhs << TENames::xmv::names[ii] << "\t";
	}
	lhs << TENames::xmv::names[TEPlant::NU - 1];
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::idv& rhs)
{
	for (int ii = 1; ii < TEPlant::NIDV; ii++)
	{
		lhs << "IDV" << ii << "\t";
	}
	lhs << "IDV" << (TEPlant::NIDV);
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::time& rhs)
{
	lhs << "time (hrs)";
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::xmeas_pq& rhs)
{
	lhs << TENames::xmeas_pq::names[0] << "\t" << TENames::xmeas_pq::names[1];
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::xmv_pq& rhs)
{
	lhs << TENames::xmv_pq::names[0] << "\t" << TENames::xmv_pq::names[1];
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::shutdown& rhs)
{
	lhs << "SD_Code";
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::plant_all& rhs)
{
	lhs << TENames::xmeas_pq() << "\t" 
		<< TENames::xmv_pq() << "\t"
		<< TENames::time() << "\t" 
		<< TENames::xmv() << "\t" 
		<< TENames::xmeas()
		<< "\t" << TENames::idv() 
		<< "\t" << TENames::shutdown();

	return lhs;
}
