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

	struct hourly_cost
	{
		friend std::ostream& operator<< (std::ostream& lhs, const hourly_cost& rhs);
	};

	struct shutdown
	{
		friend std::ostream& operator<< (std::ostream& lhs, const shutdown& rhs);
	};

	struct plant_all
	{
		friend std::ostream& operator<< (std::ostream& lhs, const plant_all& rhs);
	};
};

#endif
