/*

* Author:       Rick Candell (rick.candell@nist.gov)

* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include <iostream>

#ifndef __TENAMES_H__
#define __TENAMES_H__

class TENames
{
public:
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

};

#endif
