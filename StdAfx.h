// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_)
#define AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

//{{AFX_INSERT_LOCATION}}

#define MAX_LAYER_NUMBER		0xff

#define ETHER_MAX_SIZE			1514
#define ETHER_HEADER_SIZE		14
#define ETHER_MAX_DATA_SIZE		( ETHER_MAX_SIZE - ETHER_HEADER_SIZE )
#define ETHER_PROTO_TYPE_IP		0x0800
#define ETHER_PROTO_TYPE_ARP	0x0806
#define ETHER_PROTO_TYPE_RARP	0x0835

#define TCP_HEADER_SIZE			( sizeof(unsigned int) * 2 +				\
								  sizeof(unsigned short) * 5 +					\
								  sizeof(unsigned char) * 6 )
#define TCP_DATA_SIZE			( ETHER_MAX_SIZE - ( TCP_HEADER_SIZE +  IP_HEADER_SIZE + \
																		ETHER_HEADER_SIZE ) )

#define IP_HEADER_SIZE			( sizeof(unsigned short) * 4 +					\
								  sizeof(unsigned char) * 12  )
#define IP_DATA_SIZE			( ETHER_MAX_SIZE - ( IP_HEADER_SIZE  + \
														ETHER_HEADER_SIZE ))

#define APP_HEADER_SIZE			( sizeof(unsigned short) +					\
								  sizeof(unsigned char)  +				\
								  sizeof(unsigned long) )
#define APP_DATA_SIZE 			( ETHER_MAX_DATA_SIZE - ( APP_HEADER_SIZE +		\
												          TCP_HEADER_SIZE +		\
												          IP_HEADER_SIZE ) )

#define TCP_PORT_CHAT			0x2080
#define TCP_PORT_FILE			0x2090

#define ARP_HEADER_SIZE			28
#define ARP_HARDTYPE			0x0001
#define ARP_PROTTYPE_IP			0x0800
#define ARP_OP_REQUEST			0x0001
#define ARP_OP_REPLY			0x0002
#define RARP_OP_REQUEST			0x0003
#define RARP_OP_REPLY			0x0004

#define NI_COUNT_NIC	10

#define FILEAPP_HEADER_SIZE			12
#define FILEAPP_MAX_DATA			1488 //1448//( TCP_MAX_DATA - FILEAPP_HEADER_SIZE )

typedef struct _ETHERNET_ADDR
{
	union {
		struct { unsigned char e0, e1, e2, e3, e4, e5; } s_un_byte;
		unsigned char s_ether_addr[6];
	} S_un;

	#define addr0 S_un.s_un_byte.e0
	#define addr1 S_un.s_un_byte.e1
	#define addr2 S_un.s_un_byte.e2
	#define addr3 S_un.s_un_byte.e3
	#define addr4 S_un.s_un_byte.e4
	#define addr5 S_un.s_un_byte.e5

	#define addrs S_un.s_ether_addr

} ETHERNET_ADDR, * LPETHERNET_ADDR;

typedef struct _IP_ADDR
{
	union 
	{
		struct { unsigned char e0, e1, e2, e3; } s_un_byte;
		unsigned char s_ether_addr[4];
	} S_un;

	#define addr0 S_un.s_un_byte.e0
	#define addr0 S_un.s_un_byte.e1
	#define addr0 S_un.s_un_byte.e2
	#define addr0 S_un.s_un_byte.e3

	#define addrs S_un.s_ether_addr
} IP_ADDR, *LPIP_ADDR;


// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__119ECB1B_6E70_4662_A2A9_A20B5201CA81__INCLUDED_)
