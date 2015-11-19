/*

* Author:       Rick Candell (rick.candell@nist.gov)

* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include <iostream>

#include "TETypes.h"

#ifndef __TENAMES_H__
#define __TENAMES_H__

class TENames
{
public:

	enum name_domain { PLANT, CONTROLLER};

	static name_domain _domain;
	static std::string _domain_str;
	static void set_to_plant();
	static void set_to_controller();

	struct xmeas_pq
	{
		static const char * names[];
		friend std::ostream& operator<< (std::ostream& lhs, const xmeas_pq& rhs);
	};
	struct xmv_pq
	{
		static const char * names[];
		friend std::ostream& operator<< (std::ostream& lhs, const xmv_pq& rhs);
	};
	struct time
	{
		static const char * names[];
		friend std::ostream& operator<< (std::ostream& lhs, const time& rhs);
	};

	struct xmeas
	{
		static const char * names[];
		friend std::ostream& operator<< (std::ostream& lhs, const xmeas& rhs);
	};

	struct xmv
	{
		static const char * names[];
		friend std::ostream& operator<< (std::ostream& lhs, const xmv& rhs);
	};

	struct idv
	{
		friend std::ostream& operator<< (std::ostream& lhs, const idv& rhs);
	};

	struct non_process
	{
		friend std::ostream& operator<< (std::ostream& lhs, const non_process& rhs);
	};

	struct shutdown
	{
		friend std::ostream& operator<< (std::ostream& lhs, const shutdown& rhs);
	};

	struct plant_all
	{
		friend std::ostream& operator<< (std::ostream& lhs, const plant_all& rhs);
	};

	struct r_states
	{
		friend std::ostream& operator<< (std::ostream& lhs, const r_states& rhs);
	};

	struct 
	{
		friend std::ostream& operator<< (std::ostream& lhs, const r_states& rhs);
	};

	struct controller_all
	{
		friend std::ostream& operator<< (std::ostream& lhs, const controller_all& rhs);
	};

	struct simlog_all
	{
		friend std::ostream& operator<< (std::ostream& lhs, const simlog_all& rhs);
	};
};

#endif
