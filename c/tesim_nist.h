/* 

* Author: Rick Candell
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License: Public Domain

*/

#ifndef __TESIM_H__
#define __TESIM_H__

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
//  4. update tesimulator with xmeas and xmv using set methods for each
//	5. increment the simulator
//  6. repeat from Step 3.

class TESimulator
{		
    public:
        static TESimulator* getInstance();
		virtual ~TESimulator();

		// initialize the simulation
		void initialize(const double ts_ode, const double ts_plant, const double ts_ctlr);
		
		// increment both plant and controller one time step
		void increment();
		
		// set and get for xmeas
		const double* get_xmeas() const { return m_xmeas; };
		void set_xmeas(const double* xmeas);
		
		// set and get for xmv
		const double* get_xmv() const { return m_xmv; };
		void set_xmv(const double* xmv);		
		
    private:
        TESimulator() {}; 
        TESimulator(TESimulator const&);    // Singleton: Don't Implement
        void operator=(TESimulator const&); // Singleton: Don't implement
		
		// run the plant one time step
		void increment_plant();
		
		// run the controller one scan interval
		void increment_controller();
		
		// validates that tstep is integer multiple of ode time step
		void valid_times(const double ts_plant, const double ts_ctlr) throw (...);

		// variables
		double 		m_ts_ode;			// time step for the ode solver
		double 		m_ts_plant;			// time step for the plant
		double 		m_ts_ctlr;			// time step for the controller
		unsigned 	m_ode_per_tstep;
		unsigned 	m_ode_per_tscan;
		double*		m_xmeas;
		double*		m_xmv;

		static TESimulator*    instance; 
		
};

#endif
