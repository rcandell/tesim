/*

* Author:       Rick Candell (rick.candell@nist.gov)

* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#include "TEChannel.h"


std::ostream& TEChannel::print(std::ostream& os) const
{
	unsigned dlen = this->dlen();
	for (unsigned ii = 0; ii < dlen - 1; ii++)
	{
		os << this->chan_state()[ii] << "\t" << this->data()[ii] << "\t";
	}
	os << this->chan_state()[dlen - 1] << "\t" << this->data()[dlen - 1];
	return os;
}
std::ostream& operator<< (std::ostream& lhs, const TEChannel& rhs)
{
	return rhs.print(lhs);
}

