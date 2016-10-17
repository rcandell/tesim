/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#ifdef USE_ADS_IF

#define DINT __int32

#include "TEPlant.h"
#include "TEADSInterface.h"
#include <string>
#include <iostream>
#include <exception>

TEADSInterface::TEADSInterface()
{
}

TEADSInterface::~TEADSInterface()
{
}

void
TEADSInterface::connect(std::string varName, const short nRemotePort)
{
	long nErr = 0;

	m_nRemotePort = nRemotePort;
	m_varName = varName;
	m_pAmsAddr = &m_AmsAddr;

	// open connection
	m_nLocalPort = (short)AdsPortOpen();

	nErr = AdsGetLocalAddress(m_pAmsAddr);
	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsGetLocalAddress");
		throw(err);
	}
	m_AmsAddr.port = m_nRemotePort;

	// retrieve handle for variable
	char szVar[256];
	strncpy_s(szVar, (char*)varName.c_str(), 256);
	nErr = AdsSyncReadWriteReq(m_pAmsAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(m_hVarHandle), &m_hVarHandle, sizeof(szVar), szVar);
	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsSyncReadWriteReq");
		throw(err);
	}
}

void 
TEADSInterface::connect(std::string varName, const PAmsAddr remote_addr)
{
	long nErr = 0;

	m_nRemotePort = remote_addr->port;
	m_varName = varName;
	m_pAmsAddr = &m_AmsAddr;

	// open connection
	m_nLocalPort = (short)AdsPortOpen();
	m_AmsAddr.netId.b[0] = remote_addr->netId.b[0];
	m_AmsAddr.netId.b[1] = remote_addr->netId.b[1];
	m_AmsAddr.netId.b[2] = remote_addr->netId.b[2];
	m_AmsAddr.netId.b[3] = remote_addr->netId.b[3];
	m_AmsAddr.netId.b[4] = remote_addr->netId.b[4];
	m_AmsAddr.netId.b[5] = remote_addr->netId.b[5];
	m_AmsAddr.port = m_nRemotePort;

	// retrieve handle for variable
	char szVar[256];
	strncpy_s(szVar, (char*)varName.c_str(), 256);
	nErr = AdsSyncReadWriteReq(m_pAmsAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(m_hVarHandle), &m_hVarHandle, sizeof(szVar), szVar);
	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsSyncReadWriteReq");
		std::cout << err.nErr << "\t" << err.errStr << std::endl;
		exit(0);
	}
}

template<typename T>
void
TEADSInterface::read_value(T* x, const long nel)
{
	// Reset the value of the PLC variable to 0 
	long nErr = 0;
	T x_tmp[128];  // NOTE this will need to be increased if more elements are requested
	nErr = AdsSyncReadReq(m_pAmsAddr, ADSIGRP_SYM_VALBYHND, m_hVarHandle, nel*sizeof(T), &x_tmp);

	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsSyncWriteReq");
		throw(err);
	}

	memcpy(x, x_tmp, sizeof(float)*nel);
}

// satisfy linker problems
template void TEADSInterface::read_value<float>(float*, const long);
template void TEADSInterface::read_value<double>(double*, const long);
template void TEADSInterface::read_value<int>(int*, const long);
template void TEADSInterface::read_value<bool>(bool*, const long);

template<typename T>
void 
TEADSInterface::write_value(const T* x, const long nel)
{
	// Reset the value of the PLC variable to 0 
	long nErr = 0;
	T x_tmp[128];
	memcpy(x_tmp, x, nel * sizeof(T));
	nErr = AdsSyncWriteReq(m_pAmsAddr, ADSIGRP_SYM_VALBYHND, m_hVarHandle, nel*sizeof(T), &x_tmp);
	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsSyncWriteReq");
		throw(err);
	}
}

// satisfy linker problems
template void TEADSInterface::write_value<float>(const float*, const long);
template void TEADSInterface::write_value<double>(const double*, const long);
template void TEADSInterface::write_value<int>(const int*, const long);
template void TEADSInterface::write_value<bool>(const bool*, const long);

/*
void 
TEADSInterface::write_lreal(const double* x, const long nel)
{
	// Reset the value of the PLC variable to 0 
	long nErr = 0;
	double x_tmp[128];
	memcpy(x_tmp, x, nel * sizeof(double));
	nErr = AdsSyncWriteReq(m_pAmsAddr, ADSIGRP_SYM_VALBYHND, m_hVarHandle, nel*sizeof(double), &x_tmp);
	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsSyncWriteReq");
		throw(err);
	}
}

void
TEADSInterface::write_real(const float* x, const long nel)
{
	// Reset the value of the PLC variable to 0 
	long nErr = 0;
	float x_tmp[128];
	memcpy(x_tmp, x, nel * sizeof(float));
	nErr = AdsSyncWriteReq(m_pAmsAddr, ADSIGRP_SYM_VALBYHND, m_hVarHandle, nel*sizeof(double), &x_tmp);
	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsSyncWriteReq");
		throw(err);
	}
}

void
TEADSInterface::read_real(float* x, const long nel)
{
	// Reset the value of the PLC variable to 0 
	long nErr = 0;
	float x_tmp[128];  // NOTE this will need to be increased if more elements are requested
	nErr = AdsSyncReadReq(m_pAmsAddr, ADSIGRP_SYM_VALBYHND, m_hVarHandle, nel*sizeof(float), &x_tmp);

	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsSyncWriteReq");
		throw(err);
	}

	memcpy(x, x_tmp, sizeof(float)*nel);
}

void
TEADSInterface::write_dint(const int* x, const long nel)
{
	// Reset the value of the PLC variable to 0 
	long nErr = 0;
	int x_tmp[128];
	memcpy(x_tmp, x, nel * sizeof(int));
	nErr = AdsSyncWriteReq(m_pAmsAddr, ADSIGRP_SYM_VALBYHND, m_hVarHandle, nel*sizeof(DINT), &x_tmp);
	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsSyncWriteReq");
		throw(err);
	}
}
*/

#endif //USE_ADS_IF