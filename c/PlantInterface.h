/*

* Author: Rick Candell (rick.candell@nist.gov)
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License: Public Domain

*/

#ifndef __PLANTIF_H__
#define __PLANTIF_H__

template <class T> 
class PlantSignalData
{
public:
	PlantSignalData(const T&); 
	virtual ~PlantSignalData();
	PlantSignalData& operator=(const PlantSignalData&);
	T operator() { return this->m_dat; }
	
private:
	PlantSignalData() { return m_dat;  };
	PlantSignalData(const PlantSignalData&);

	T m_dat;
};

class PlantInterface
{
public:
	PlantInterface();
	virtual ~PlantInterface();

	// Transmit the data
	virtual void transmitData(const PlantSignalData<double>) = 0;
	virtual void receiveData(const PlantSignalData<double>) = 0;

private:
	PlantInterface(const PlantInterface&);
	PlantInterface& operator=(const PlantInterface&);
};


#endif
