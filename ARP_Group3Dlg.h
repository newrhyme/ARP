
// ARP_Group3Dlg.h: 헤더 파일
//

#pragma once

#include "LayerManager.h"
#include "NILayer.h"
#include "EthernetLayer.h"
#include "IPLayer.h"
#include "ARPLayer.h"
#include <chrono>
#include <unordered_map>

// CARPGroup3Dlg 대화 상자
class CARPGroup3Dlg : public CDialogEx, public CBaseLayer
{
// 생성입니다.
public:
	CARPGroup3Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARP_GROUP3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnComboEnetAddr();

	DECLARE_MESSAGE_MAP()

public:

	unsigned char* MacAddrToHexInt(CString ether);

	BOOL Receive(unsigned char* ppayload);
	
	CListCtrl m_arpCacheList;
	CComboBox m_ComboEnetName;
	CString m_unSrcEnetAddr;
	CIPAddressCtrl m_srcIPAddr;
	CIPAddressCtrl m_dstIPAddr;

	std::unordered_map<int, std::chrono::steady_clock::time_point> time_table;

private:
	CLayerManager m_LayerMgr;
	enum {
		IPC_INITIALIZING,
		CFT_COMBO_SET,
		IPC_WAITFORACK,
		IPC_ERROR,
		IPC_READY_TO_SEND,
		IPC_IP_SELECT,
	};

	void SetDlgState(int state);
	inline void EndofProcess();
	void PaintARPCacheListBox();

	BOOL m_bSendReady;

	//Object Layer
	CNILayer* m_NI;
	CEthernetLayer* m_Eth;
	CARPLayer* m_ARP;
	CIPLayer* m_IP;

public:
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnCbnSelchangeComboEth();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonItemDelete();
	afx_msg void OnBnClickedButtonAllDelete();

	void CacheUpdate();
	void UpdateCacheList(unsigned char* ip, unsigned char* mac, bool status);
};
