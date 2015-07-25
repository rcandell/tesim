/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/


#include <utility>
#include <iomanip>
#include <string>

#include "TETypes.h"

namespace std {

	// pq pair
	std::istream& operator>>(std::istream& in, pq_pair& ss) {
		std::string s;
		in >> s;
		const size_t sep = s.find(':');
		if (sep == std::string::npos) {
			ss.first = 0.0;
			ss.second = std::stod(s);
		}
		else {
			ss.first = std::stod(s.substr(0, sep));
			ss.second = std::stod(s.substr(sep + 1));
		}
		return in;
	}

	std::ostream& operator<<(std::ostream& lhs, const pq_pair& rhs)
	{
		lhs << rhs.first << "\t" << rhs.second;
		return lhs;
	}

	// int double pair for setpoints
	std::istream& operator>>(std::istream& in, sp_pair& ss) {
		std::string s;
		in >> s;
		const size_t sep = s.find(':');
		if (sep == std::string::npos) {
			ss.first = -1;
			ss.second = std::stod(s);
		}
		else {
			ss.first = std::stoi(s.substr(0, sep));
			ss.second = std::stod(s.substr(sep + 1));
		}
		return in;
	}

	std::ostream& operator<<(std::ostream& lhs, const sp_pair& rhs)
	{
		lhs << rhs.first << "\t" << rhs.second;
		return lhs;
	}
}