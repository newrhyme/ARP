
// ARP_Group3Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ARP_Group3.h"
#include "ARP_Group3Dlg.h"
#include "afxdialogex.h"
#include "ARPLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CARPGroup3Dlg 대화 상자



CARPGroup3Dlg::CARPGroup3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ARP_GROUP3_DIALOG, pParent)
	, CBaseLayer("App Layer")
	, m_bSendReady(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_LayerMgr.AddLayer(this);
	m_LayerMgr.AddLayer(new CNILayer("NI"));
	m_LayerMgr.AddLayer(new CEthernetLayer("Ethernet"));
	m_LayerMgr.AddLayer(new CARPLayer("ARP"));
	m_LayerMgr.AddLayer(new CIPLayer("IP"));
	
	m_NI = (CNILayer*)m_LayerMgr.GetLayer("NI");
	m_Eth = (CEthernetLayer*)m_LayerMgr.GetLayer("Ethernet");
	m_ARP = (CARPLayer*)m_LayerMgr.GetLayer("ARP");
	m_IP = (CIPLayer*)m_LayerMgr.GetLayer("IP");

	m_IP->SetUpperUnderLayer(this);
	m_IP->SetUnderLayer(m_ARP);
	m_Eth->SetUpperUnderLayer(m_ARP);
	m_Eth->SetUpperLayer(m_IP);
	m_NI->SetUpperUnderLayer(m_Eth);


}

void CARPGroup3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ARP_CACHE, m_arpCacheList);
	DDX_Control(pDX, IDC_COMBO_ETH, m_ComboEnetName);
	DDX_Text(pDX, IDC_EDIT_ETH_SRC, m_unSrcEnetAddr);
	DDX_Control(pDX, IDC_IPADDRESS_SELECT, m_srcIPAddr);
	DDX_Control(pDX, IDC_IPADDRESS_SEND, m_dstIPAddr);
}



BEGIN_MESSAGE_MAP(CARPGroup3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_ETH, &CARPGroup3Dlg::OnCbnSelchangeComboEth)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CARPGroup3Dlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CARPGroup3Dlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_ALL_DELETE, &CARPGroup3Dlg::OnBnClickedButtonAllDelete)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_DELETE, &CARPGroup3Dlg::OnBnClickedButtonItemDelete)
END_MESSAGE_MAP()


// CARPGroup3Dlg 메시지 처리기

BOOL CARPGroup3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetDlgState(IPC_INITIALIZING);
	SetDlgState(CFT_COMBO_SET);
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CARPGroup3Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
}



void CARPGroup3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}



// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CARPGroup3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}



// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CARPGroup3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// select 버튼을 눌렀을 때
void CARPGroup3Dlg::OnBnClickedButtonSelect()
{
	UpdateData(TRUE);

	if (!m_unSrcEnetAddr || !m_srcIPAddr)
	{
		MessageBox(_T("주소를 설정 오류발생"),
			_T("경고"),
			MB_OK | MB_ICONSTOP);

		return;
	}
	int nIndex = m_ComboEnetName.GetCurSel();
	m_NI->SetAdapterNumber(nIndex);

	CString inNicName = m_NI->GetAdapterObject(nIndex)->name;

	CEdit* pSrcEdit = (CEdit*)GetDlgItem(IDC_EDIT_ETH_SRC);

	pSrcEdit->SetWindowTextA(m_NI->GetNICardAddress((char*)inNicName.GetString()));

	//선택한 adapter에 맞는 ethernet주소

	// Ethernet Frame의 Source Address 설정
	m_Eth->SetEnetSrcAddress(MacAddrToHexInt(m_unSrcEnetAddr));
	// ARP Frame의 Source MAC Address 설정
	m_ARP->SetEnetSrcAddress(MacAddrToHexInt(m_unSrcEnetAddr));
	
	// ARP Frame의 Source IP Address 설정
	unsigned char srcIP[4];
	m_srcIPAddr.GetAddress(srcIP[0], srcIP[1], srcIP[2], srcIP[3]);
	m_ARP->SetSrcIPAddress(srcIP);


	SetDlgState(IPC_IP_SELECT);
	SetDlgState(IPC_READY_TO_SEND);

	// 5초마다 Cache List 업데이트를 위한 함수
	SetTimer(1333, 5000, NULL);

	// 패킷 시작
	m_NI->PacketStartDriver();
}


void CARPGroup3Dlg::OnBnClickedButtonSend()
{
	UpdateData(TRUE);

	// ARP Frame의 Target IP Address 설정
	unsigned char dstIP[4];
	m_dstIPAddr.GetAddress(dstIP[0], dstIP[1], dstIP[2], dstIP[3]);
	m_ARP->SetDstIPAddress(dstIP);
	
	// Ethernet Frame의 Destination MAC Address를 BROADCAST로 설정
	unsigned char BROADCAST[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	m_Eth->SetEnetDstAddress(BROADCAST);
	
	// 사용자에게 00:00:00:00:00:00을 보여주기 위해 할당
	unsigned char* NOTHING[6] = { 0x00, };
	unsigned char* ipCopy = (unsigned char*)malloc(4);
	unsigned char* macCopy = (unsigned char*)malloc(6);
	memcpy(ipCopy, dstIP, 4);
	memcpy(macCopy, NOTHING, 6);

	// arp_table에 IP Address를 기준으로 이미 존재하는지 검사
	bool duplicate = false;
	for (const auto& entry : m_ARP->arp_table) {
		if (memcmp(get<0>(entry), dstIP, 4) == 0) {
			duplicate = true;
			break;
		}
	}

	// arp_table에 해당 항목이 없을 경우 arp_table에 추가 후 업데이트
	if (!duplicate) {
		m_ARP->arp_table.push_back(make_tuple(ipCopy, macCopy, FALSE));
	}
	CacheUpdate();

	// 현재 시간을 time_table에 저장
	int new_index = m_ARP->arp_table.size() - 1;
	time_table[new_index] = std::chrono::steady_clock::now();

	BOOL bSuccess = FALSE;
	unsigned char dumpPayload = '\0';
	bSuccess = mp_UnderLayer->Send(&dumpPayload, 1);

	UpdateData(FALSE);
}

// Delete All 버튼 눌렀을 경우
void CARPGroup3Dlg::OnBnClickedButtonAllDelete()
{
	UpdateData(TRUE);
	// arp_table, List, time_table 의 전체 내용 삭제
	m_ARP->arp_table.clear();
	m_arpCacheList.DeleteAllItems();
	time_table.clear();
	UpdateData(FALSE);
}

// Item Delete 버튼 눌렀을 경우
void CARPGroup3Dlg::OnBnClickedButtonItemDelete()
{
	// 현재 선택한 행을 저장
	int selectedItem = m_arpCacheList.GetSelectionMark();
	if (selectedItem == -1)
	{
		MessageBox(_T("삭제할 항목을 선택하세요."), _T("알림"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	// 선택한 부분의 arp_table, time_table 삭제
	if (selectedItem >= 0 && selectedItem < m_ARP->arp_table.size()) {
		m_ARP->arp_table.erase(m_ARP->arp_table.begin() + selectedItem);
		time_table.erase(selectedItem);
	}
	// List의 내용도 삭제
	m_arpCacheList.DeleteItem(selectedItem);

}


void CARPGroup3Dlg::SetDlgState(int state)
{
	UpdateData(TRUE);
	
	int i;

	CString device_description;

	CButton* pSendButton = (CButton*)GetDlgItem(IDC_BUTTON_SEND);
	CButton* pSelectButton = (CButton*)GetDlgItem(IDC_BUTTON_SELECT);
	CButton* pItemDeleteBtn = (CButton*)GetDlgItem(IDC_BUTTON_ITEM_DELETE);
	CButton* pAllDeleteBtn = (CButton*)GetDlgItem(IDC_BUTTON_ALL_DELETE);

	CEdit* pSendIPEdit = (CEdit*)GetDlgItem(IDC_IPADDRESS_SEND);
	CEdit* pSelectIPEdit = (CEdit*)GetDlgItem(IDC_IPADDRESS_SELECT);

	CComboBox* pEnetNameCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ETH);

	switch (state)
	{
	case IPC_INITIALIZING:
		PaintARPCacheListBox();
		pSendButton->EnableWindow(FALSE);
		pItemDeleteBtn->EnableWindow(FALSE);
		pAllDeleteBtn->EnableWindow(FALSE);
		pSendIPEdit->EnableWindow(FALSE);
		m_arpCacheList.EnableWindow(FALSE);
		break;
	case CFT_COMBO_SET:
		for (i = 0; i < NI_COUNT_NIC; i++) {
			if (!m_NI->GetAdapterObject(i))
				break;
			device_description = m_NI->GetAdapterObject(i)->description;
			device_description.Trim();
			pEnetNameCombo->AddString(device_description);
			pEnetNameCombo->SetCurSel(0);
		}
		break;
	case IPC_WAITFORACK:	break;
	case IPC_ERROR:		break;
	case IPC_READY_TO_SEND:
		pSendButton->EnableWindow(TRUE);
		pItemDeleteBtn->EnableWindow(TRUE);
		pAllDeleteBtn->EnableWindow(TRUE);
		pSendIPEdit->EnableWindow(TRUE);
		m_arpCacheList.EnableWindow(TRUE);
		break;
	case IPC_IP_SELECT:
		pSelectIPEdit->EnableWindow(FALSE);
		pEnetNameCombo->EnableWindow(FALSE);
		break;
	}
	
	UpdateData(FALSE);
}


// 초기의 Cache List 상태 표현
void CARPGroup3Dlg::PaintARPCacheListBox()
{
	m_arpCacheList.InsertColumn(0, _T("IP Address"), LVCFMT_LEFT, 150);
	m_arpCacheList.InsertColumn(1, _T("Ethernet Address"), LVCFMT_LEFT, 150);
	m_arpCacheList.InsertColumn(2, _T("Status"), LVCFMT_LEFT, 100);
	// 선택할 때 행 전체를 선택
	m_arpCacheList.SetExtendedStyle(m_arpCacheList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
}



void CARPGroup3Dlg::EndofProcess() {
	m_LayerMgr.DeAllocLayer();
}



void CARPGroup3Dlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
//	m_ListChat.AddString( ">> The last message was time-out.." ) ;
//	SetTimer(1,2000,NULL);
	
    // 1333번 아이디
	if (nIDEvent == 1333) {

		// timeout 코드
		auto now = std::chrono::steady_clock::now();

		for (auto it = time_table.begin(); it != time_table.end(); ) {
			int index = it->first;
			auto timestamp = it->second;

			// arp_table의 status가 TRUE일 때 -> Complete 상태일 때 실행
			if (std::get<2>(m_ARP->arp_table[index])) {
				// 20분 timeout 검사
				if (std::chrono::duration_cast<std::chrono::minutes> (now - timestamp).count() >= 20) {
					free(get<0>(m_ARP->arp_table[index]));
					free(get<1>(m_ARP->arp_table[index]));
					m_ARP->arp_table.erase(m_ARP->arp_table.begin() + index);
					it = time_table.erase(it);
				}
				else {
					++it;
				}
			}
			// arp table의 status가 FALSE일 때 -> Incomplete 상태
			else {
				// 3분 timeout 검사
				if (std::chrono::duration_cast<std::chrono::minutes>(now - timestamp).count() >= 3) {
					free(get<0>(m_ARP->arp_table[index]));
					free(get<1>(m_ARP->arp_table[index]));
					m_ARP->arp_table.erase(m_ARP->arp_table.begin() + index);

					it = time_table.erase(it);
				}
				else {
					++it;
				}
			}
		}
		CacheUpdate();
	}
	CDialog::OnTimer(nIDEvent);
}


void CARPGroup3Dlg::CacheUpdate()
{
	// List에 있는 내용 전부 삭제
	m_arpCacheList.DeleteAllItems();

	// arp_table의 크기
	int length = m_ARP->arp_table.size();
	for (int i = 0; i < length; i++)
	{
		LV_ITEM lvItem;
		lvItem.iItem = m_arpCacheList.GetItemCount();
		lvItem.mask = LVIF_TEXT;
		
		// arp_table의 1열 -> IP Address 출력
		unsigned char* currentIP = get<0>(m_ARP->arp_table[i]);
		CString ip;
		ip.Format("%d.%d.%d.%d", currentIP[0], currentIP[1], currentIP[2], currentIP[3]);
		lvItem.iSubItem = 0;
		lvItem.pszText = ip.GetBuffer();
		m_arpCacheList.InsertItem(&lvItem);
		ip.ReleaseBuffer();

		// arp_table의 2열 -> MAC Address 출력
		unsigned char* currentMAC = get<1>(m_ARP->arp_table[i]);
		CString mac;
		mac.Format("%02x:%02x:%02x:%02x:%02x:%02x", currentMAC[0], currentMAC[1], currentMAC[2], currentMAC[3], currentMAC[4], currentMAC[5]);
		m_arpCacheList.SetItemText(i, 1, mac);

		// arp_table의 status 출력 -> TRUE일 경우 complete, FALSE일 경우 incomplete
		CString status;
		status.Format("%s", get<2>(m_ARP->arp_table[i]) ? "complete" : "incomplete");
		m_arpCacheList.SetItemText(i, 2, status);
	}
}


BOOL CARPGroup3Dlg::Receive(unsigned char* ppayload)
{
	CARPLayer::PARP_HEADER pFrame = (CARPLayer::PARP_HEADER)ppayload;

    unsigned char* srcIP = pFrame->arp_srcIPAddr.addrs;
    unsigned char* srcMAC = pFrame->arp_srcEthAddr.addrs;

	for (auto& entry : m_ARP->arp_table) {
		if (memcmp(get<0>(entry), srcIP, 4) == 0) {

			unsigned char* macCopy = (unsigned char*)malloc(6);
			memcpy(macCopy, srcMAC, 6);
			// 기존에 있던 내용 삭제 후 저장
			free(get<1>(entry));
			get<1>(entry) = macCopy;
			get<2>(entry) = TRUE;

			// status의 값이 TRUE로 됐을 때 time_table의 시간도 다시 할당
			int index = &entry - &m_ARP->arp_table[0];
			time_table[index] = std::chrono::steady_clock::now();
			break;
		}
	}
	CacheUpdate();
	return TRUE;
}


void CARPGroup3Dlg::OnComboEnetAddr()
{
	UpdateData(TRUE);

	int nIndex = m_ComboEnetName.GetCurSel();
	m_NI->GetAdapterObject(nIndex)->name;

	UpdateData(FALSE);
}



void CARPGroup3Dlg::OnCbnSelchangeComboEth()
{
	int nIndex = m_ComboEnetName.GetCurSel();
	m_NI->SetAdapterNumber(nIndex);

	CString inNicName = m_NI->GetAdapterObject(nIndex)->name;

	CEdit* pSrcEdit = (CEdit*)GetDlgItem(IDC_EDIT_ETH_SRC);

	pSrcEdit->SetWindowTextA(m_NI->GetNICardAddress((char*)inNicName.GetString()));

}



unsigned char* CARPGroup3Dlg::MacAddrToHexInt(CString ether)
{
	// 콜론(:)으로 구분 되어진 Ethernet 주소를
	// 콜론(:)을 토큰으로 한 바이트씩 값을 가져와서 Ethernet배열에 넣어준다.
	CString cstr;
	unsigned char* arp_ether = (u_char*)malloc(sizeof(u_char) * 6);

	for (int i = 0; i < 6; i++) {
		AfxExtractSubString(cstr, ether, i, ':');
		// strtoul -> 문자열을 원하는 진수로 변환 시켜준다.
		arp_ether[i] = (unsigned char)strtoul(cstr.GetString(), NULL, 16);
	}
	arp_ether[6] = '\0';

	return arp_ether;
}