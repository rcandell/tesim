/*

* Author:       Rick Candell (rick.candell@nist.gov)
*
* Organization: National Institute of Standards and Technology
*               U.S. Department of Commerce
* License:      Public Domain

*/

#ifdef USE_ADS_IF

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
	strncpy(szVar, (char*)varName.c_str(), 256);
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
	strncpy(szVar, (char*)varName.c_str(), 256);
	nErr = AdsSyncReadWriteReq(m_pAmsAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(m_hVarHandle), &m_hVarHandle, sizeof(szVar), szVar);
	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsSyncReadWriteReq");
		throw(err);
	}
}

void 
TEADSInterface::write(const double* x)
{
	
	// Reset the value of the PLC variable to 0 
	long nErr = 0;
	double x_tmp[41];
	memcpy(x_tmp, x, 41 * sizeof(double));
	nErr = AdsSyncWriteReq(m_pAmsAddr, ADSIGRP_SYM_VALBYHND, m_hVarHandle, 41*sizeof(double), &x_tmp);
	if (nErr)
	{
		TEADSInterface::ADSError err(nErr, "AdsSyncWriteReq");
		throw(err);
	}
}

void
TEADSInterface::read(double* x)
{
}

#endif //USE_ADS_IF