/*

* Author:       Rick Candell (rick.candell@nist.gov)

* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include  "TENames.h"
#include "TEPlant.h"

TENames::name_domain TENames::_domain = TENames::name_domain::PLANT;
std::string TENames::_domain_str = "Plant";
void TENames::set_to_plant()
{
	TENames::_domain = TENames::name_domain::PLANT;
	TENames::_domain_str = "Plant";
}
void TENames::set_to_controller()
{
	TENames::_domain = TENames::name_domain::CONTROLLER;
	TENames::_domain_str = "Ctlr";
}

const char * TENames::xmeas_pq::names[] = { "xmeas_p", "xmeas_q" };

const char * TENames::xmv_pq::names[] = { "xmv_p", "xmv_q" };

const char * TENames::xmeas::names[] = {
	"A.Feed.kscmh",
	"D.Feed.kg/hr",
	"E.Feed.kg/hr",
	"A+C.Feed.kscmh",
	"Recycle.Flow.kscmh",
	"Reactor.Feed.kscmh",
	"Reactor.Pressure.kPa",
	"Reactor.Level.Pct",
	"Reactor.Temp.C",
	"Purge.Rate.kscmh",
	"Sep.Temp.°C",
	"Sep.Level.Pct",
	"Sep.Pressure.kPa",
	"Sep.Underflow.m3/hr",
	"Stripper.Level.Pct",
	"Stripper.Pressure.kPa",
	"Stripper.Underflow.m3/hr",
	"Stripper.Temp.C",
	"Steam.Flow.kg/hr",
	"Compressor.Work.kW",
	"React.Cool.Temp.C",
	"Cond.Cool.Temp.C",
	"Feed.A.mole.Pct",
	"Feed.B.mole.Pct",
	"Feed.C.mole.Pct",
	"Feed.D.mole.Pct",
	"Feed.E.mole.Pct",
	"Feed.F.mole.Pct",
	"Purge.Pct.A.mole.Pct",
	"Purge.Pct.B.mole.Pct",
	"Purge.Pct.C.mole.Pct",
	"Purge.Pct.D.mole.Pct",
	"Purge.Pct.E.mole.Pct",
	"Purge.Pct.F.mole.Pct",
	"Purge.Pct.G.mole.Pct",
	"Purge.Pct.H.mole.Pct",
	"Product.D.mole.Pct",
	"Product.E.mole.Pct",
	"Product.F.mole.Pct",
	"Product.G.mole.Pct",
	"Product.H.mole.Pct"
};

const char * TENames::xmv::names[] = {
	"D.Feed.Setting",
	"E.Feed.Setting",
	"A.Feed.Setting",
	"A+C.Feed.Setting",
	"Recycle.Valve",
	"Purge.Valve",
	"Separator.Flow.Setting",
	"Stripper.Valve",
	"Steam.Valve",
	"Reactor.Cooling.Flow.Setting",
	"Condenser.Cooling.Flow.Setting",
	"Agitator.Speed.Setting"
};

std::ostream& operator<< (std::ostream& lhs, const TENames::xmeas& rhs)
{
	// measured variables
	for (int ii = 0; ii < TEPlant::NY - 1; ii++)
	{
		lhs << TENames::_domain_str << "." << TENames::xmeas::names[ii] << "\t";
	}
	lhs << TENames::_domain_str << "." << TENames::xmeas::names[TEPlant::NY - 1];
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::xmv& rhs)
{
	// manipulated variables
	for (int ii = 0; ii < TEPlant::NU - 1; ii++)
	{
		lhs << TENames::_domain_str << "." << TENames::xmv::names[ii] << "\t";
	}
	lhs << TENames::_domain_str << "." << TENames::xmv::names[TEPlant::NU - 1];
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::idv& rhs)
{
	lhs << "IDV";
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::time& rhs)
{
	lhs << TENames::_domain_str << "." << "time";
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
	lhs << TENames::_domain_str << "." << "SD_Code";
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::non_process& rhs)
{
	lhs << TENames::_domain_str << "." << "Hourly Cost";
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::plant_all& rhs)
{
	lhs << TENames::xmeas_pq()		<< "\t" 
		<< TENames::xmv_pq()		<< "\t"
		<< TENames::time()			<< "\t" 
		<< TENames::xmv()			<< "\t" 
		<< TENames::xmeas()			<< "\t"
		<< TENames::non_process() << "\t"
		<< TENames::idv()			<< "\t"
		<< TENames::shutdown();

	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::r_states& rhs)
{
	// r state variables
	for (int ii = 0; ii < 6; ii++)
	{
		lhs << TENames::_domain_str << "." << "r_" << ii << "\t";
	}
	lhs << TENames::_domain_str << "." << "r_" << 6;

	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::controller_all& rhs)
{
	lhs << TENames::time() << "\t"
		<< TENames::xmv() << "\t"
		<< TENames::xmeas() << "\t"
		<< TENames::r_states() << "\t"
		<< "Pct_Gsp" << "\t"
		<< "Prod_rate";

	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, const TENames::simlog_all& rhs)
{
	// channel
	lhs << TENames::xmeas_pq() << "\t"
		<< TENames::xmv_pq() << "\t";

	// plant
	TENames::set_to_plant();
	lhs << TENames::time() << "\t"
		<< TENames::xmv() << "\t"
		<< TENames::xmeas() << "\t"
		<< TENames::non_process() << "\t"
		<< TENames::idv() << "\t"
		<< TENames::shutdown() << "\t";

	TENames::set_to_controller();
	lhs	<< TENames::time() << "\t"
		<< TENames::xmv() << "\t"
		<< TENames::xmeas() << "\t"
		<< TENames::r_states() << "\t"
		<< "Pct_Gsp" << "\t"
		<< "Prod_rate";

	TENames::set_to_plant();
	
	return lhs;
}