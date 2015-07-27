/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/



#ifndef __TEETYPES_H__
#define __TEETYPES_H__

#include <utility>

typedef std::pair<double, double> pq_pair;
typedef std::pair<int, double> sp_pair;
typedef std::pair<bool, sp_pair> sp_override_pair;

namespace std {
	std::istream& operator>>(std::istream& in, pq_pair& ss);
	std::ostream& operator<<(std::ostream& in, const pq_pair& ss);

	std::istream& operator>>(std::istream& in, sp_pair& ss);
	std::ostream& operator<<(std::ostream& in, const sp_pair& ss);
}

#endif 
