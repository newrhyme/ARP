// EthernetLayer.cpp: implementation of the CEthernetLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pch.h"
#include "IPLayer.h"
#include "ARPLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIPLayer::CIPLayer(char* pName)
    : CBaseLayer(pName)
{
    ResetHeader();
}

CIPLayer::~CIPLayer()
{
}

void CIPLayer::ResetHeader()
{
    memset(m_sHeader.srcIP, 0, 4);
    memset(m_sHeader.dstIP, 0, 4);
    memset(m_sHeader.ip_data, 0, IP_DATA_SIZE);
}

BOOL CIPLayer::Send(unsigned char* ppayload, int nlength)
{
    memcpy(m_sHeader.ip_data, ppayload, nlength);

    BOOL bSuccess = FALSE;
    bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength + IP_HEADER_SIZE);

    return bSuccess;
}

BOOL CIPLayer::Receive(unsigned char* ppayload)
{

    BOOL bSuccess = FALSE;

    CARPLayer::PARP_HEADER pFrame = (CARPLayer::PARP_HEADER)ppayload;
    bSuccess = mp_aUpperLayer[0]->Receive((unsigned char*)pFrame);
    
    return bSuccess;
}

void CIPLayer::SetSrcIPAddress(unsigned char* srcIP)
{
    memcpy(m_sHeader.srcIP, srcIP, 4);
}

void CIPLayer::SetDstIPAddress(unsigned char* dstIP)
{
    memcpy(m_sHeader.dstIP, dstIP, 4);
}



