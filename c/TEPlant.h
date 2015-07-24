/* 

* Author: Rick Candell (rick.candell@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License: Public Domain

*/

#ifndef __TEPLANT_H__
#define __TEPLANT_H__

//#define NIST_DEBUG_ON (true)
//#define NIST_DEBUG(x) do { \
//  if (NIST_DEBUG_ON) { std::cerr << x << std::endl; } \
//} while (0)

// Singleton class of the TE simulator
// Calling convention:
//	1. getInstance()
//  2. initialize()  This will initialize memory, load ic's, and increment plant and control one time step
//        The simulator is now ready to provide values to the network simulator
//  3. Fetch xmv and xmeas using get methods for each, and run network for one time step
//  4. update TEPlant with xmeas and xmv using set methods for each
//	5. increment the simulator
//  6. repeat from Step 3.
   
#include <fstream>  
#include <exception>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "TELogging.h"

class TEPlant
{
public:

	class ShutdownException : public std::exception
	{
		public:
			ShutdownException(int sd_code, char* sd_msg)
				: m_sd_code(sd_code), m_sd_msg(sd_msg) {}

			char* m_sd_msg;
			int m_sd_code;

			std::string sdcode_log_str() const
			{
				std::string logstr;
				std::ostringstream logss;
				logss << m_sd_code;
				logstr = logss.str();
				return logstr;
			}

			friend std::ostream& operator<< (std::ostream& lhs, const ShutdownException& rhs)
			{
				lhs << "plant shutdown" << std::endl <<
					"code: " << rhs.m_sd_code << std::endl <<
					"msg: " << rhs.m_sd_msg << std::endl;
				return lhs;
			}
	};

    static TEPlant* getInstance();
	virtual ~TEPlant();

	// constants
	static const long int NX;			// number of states
	static const long int NU;			// number of xmv
	static const long int NY;			// number of xmeas
	static const long int NIDV;			// number of disturbance types

	// initialize the simulation
	void initialize();
		
	// run the plant one time step
	// returns the measured variables
	double* increment(double t, double dt, double* xmv, int* shutdown);
		
	// set and get for xmeas
	const double* get_xmeas() const;
	const double get_xmeas(unsigned idx) const { return m_xmeas[idx];	}
		
	// set disturbance
	// 20 different disturbance types as defined by Downs
	// Set value in array to activate disturbance
	void idv(unsigned idx, int v = 1) { m_idv[idx] = v;  }
	const int* get_idv() const { return m_idv; }
	const int get_idv(unsigned idx) const { return m_idv[idx]; }

	// get shutdown condition
	const int get_shutdown() const;
	char* shutdown_msg() const;

	// overloaded output stream for TEPlant
	friend std::ostream& operator<< (std::ostream&, const TEPlant&);
		
private:
    TEPlant() {}; 
    TEPlant(TEPlant const&);    // Singleton: Don't Implement
    TEPlant& operator=(const TEPlant&); // Singleton: Don't implement

	void updateHourlyCost(double* xmeas, double* xmv);

	// variables
	double 		m_ts_ode;			// time step for the ode solver
	double 		m_ts_plant;			// time step for the plant
	double 		m_ts_ctlr;			// time step for the controller
	unsigned 	m_ode_per_tstep;
	unsigned 	m_ode_per_tscan;

	static TEPlant*    instance; 

	// process variables memory
	double*		m_x;		// the states
	double*		m_dxdt;		// the state derivatives
	double		m_t;			// current time

	// input/output memory
	double*		m_xmeas;	// measured
	double*		m_xmv;		// manipulated
	int*		m_idv;		// dist. vector

	double		m_hourlyCost;

	static void euler(int nn, double t, double dt, double *yy, double *yp);

};

#endif

