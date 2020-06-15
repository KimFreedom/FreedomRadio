
// FreedomRadio_VS2019Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FreedomRadio_VS2019.h"
#include "FreedomRadio_VS2019Dlg.h"
#include "afxdialogex.h"
#include "HTTPRequest.hpp"

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


// CFreedomRadioVS2019Dlg 대화 상자



CFreedomRadioVS2019Dlg::CFreedomRadioVS2019Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FREEDOMRADIO_VS2019_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFreedomRadioVS2019Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_RADIO_LIST, m_lstRadioList);
	DDX_Control(pDX, IDC_LST_RADIO_INFO, m_lstRadioInfo);
	DDX_Control(pDX, IDC_EDT_LOG, m_edtLog);
}

BEGIN_MESSAGE_MAP(CFreedomRadioVS2019Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_TEST_1, &CFreedomRadioVS2019Dlg::OnBnClickedBtnTest1)
	ON_BN_CLICKED(IDC_BTN_PLAY, &CFreedomRadioVS2019Dlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, &CFreedomRadioVS2019Dlg::OnBnClickedBtnStop)
	ON_MESSAGE(UM_SEND_RADIO_SUB_INFO, &CFreedomRadioVS2019Dlg::OnSendRadioSubInfo)
	ON_MESSAGE(UM_SEND_RADIO_NOW_PLAYING, &CFreedomRadioVS2019Dlg::OnSendRadioNowPlaying)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_RADIO_LIST, &CFreedomRadioVS2019Dlg::OnNMDblclkLstRadioList)
END_MESSAGE_MAP()


// CFreedomRadioVS2019Dlg 메시지 처리기

BOOL CFreedomRadioVS2019Dlg::OnInitDialog()
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

	InitControls();

	m_objRadioPlayer.m_hParentHandle = this->m_hWnd;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFreedomRadioVS2019Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFreedomRadioVS2019Dlg::OnPaint()
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
HCURSOR CFreedomRadioVS2019Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CFreedomRadioVS2019Dlg::OnSendRadioSubInfo(WPARAM wParam, LPARAM lParam)
{
	RefreshRadioInfo();

	return 0;
}


LRESULT CFreedomRadioVS2019Dlg::OnSendRadioNowPlaying(WPARAM wParam, LPARAM lParam)
{
	RefreshRadioNowPlaying();

	return 0;
}



void CFreedomRadioVS2019Dlg::OnBnClickedBtnTest1()
{
	//std::string url = "http://nonamepiano.saycast.com";
	std::string url = "http://yes1959.saycast.com";
	std::string method = "GET";
	std::string arguments;
	CString strMsg = _T("");
	http::InternetProtocol protocol = http::InternetProtocol::V4;

	try
	{
		http::Request request(url, protocol);
		const http::Response response = request.send(method, arguments, {
			//"Content-Type: application/x-www-form-urlencoded",
				"Accept: */*",
				"Icy-MetaData: 1",
				"Connection: close",
				"User-Agent: FreedomRadio/0.1",
			});

		if (response.status == http::Response::Ok)
		{
			for each (uint8_t i in response.body)
			{
				strMsg.Format(_T("%s %d"), strMsg, i);
			}
			AfxMessageBox(strMsg);

			//             std::ofstream outfile(output, std::ofstream::binary);
			//             outfile.write(reinterpret_cast<const char*>(response.body.data()),
			//                 static_cast<std::streamsize>(response.body.size()));
		}
		else if (response.status == http::Response::Found)
		{
			std::string strServerURL = "";
			for each (std::string strHeader in response.headers)
			{
				std::size_t found = strHeader.find("Location:");
				if (found == std::string::npos)
				{
					continue;
				}

				strServerURL = strHeader.substr(10);
				break;
			}
			if (strServerURL != "")
			{
				http::Request serverRequest(strServerURL, protocol);
				const http::Response serverResponse = serverRequest.send_server_request(method, arguments, {
					//"Content-Type: application/x-www-form-urlencoded",
						"Accept: */*",
						"Icy-MetaData: 1",
						"Connection: close",
						"User-Agent: FreedomRadio/0.1",
					});

				if (serverResponse.status == http::Response::Ok)
				{
					for each (std::string strHeader in serverResponse.headers)
					{
						strMsg.Format(_T("%s %s"), strMsg, strHeader);
					}
					AfxMessageBox(strMsg);
				}
				else
				{
					for each (uint8_t i in serverResponse.body)
					{
						strMsg.Format(_T("%s %d"), strMsg, i);
					}
					AfxMessageBox(strMsg);
				}
			}
		}
		else
		{
			for each (uint8_t i in response.body)
			{
				strMsg.Format(_T("%s %d"), strMsg, i);
			}
			AfxMessageBox(strMsg);
		}
	}
	catch (const std::exception& e)
	{
		strMsg.Format(_T("Request failed, error: % s\n"), e.what());
		AfxMessageBox(strMsg);
		return;
	}
}


void CFreedomRadioVS2019Dlg::InitControls()
{
	// Init radio list
	m_lstRadioList.SetExtendedStyle(m_lstRadioList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CRect rcRadioList;
	m_lstRadioList.GetClientRect(&rcRadioList);
	int nWidthRadioList = rcRadioList.Width() - 20; // 20 : Scroll bar size

	m_lstRadioList.InsertColumn(0, _T("Radio Name"), LVCFMT_CENTER, static_cast<int>(nWidthRadioList * 0.25));
	m_lstRadioList.InsertColumn(1, _T("URL"), LVCFMT_LEFT, static_cast<int>(nWidthRadioList * 0.75));

	// Test data
	m_lstRadioList.InsertItem(0, _T("이름 없는 음악실"));
	m_lstRadioList.SetItemText(0, 1, _T("http://nonamepiano.saycast.com"));

	m_lstRadioList.InsertItem(1, _T("맑은 빗소리 클래식 방송"));
	m_lstRadioList.SetItemText(1, 1, _T("http://yes1959.saycast.com"));

	////// Init radio info
	m_lstRadioInfo.SetExtendedStyle(m_lstRadioList.GetExtendedStyle() | LVS_EX_GRIDLINES);

	CRect rcRadioInfo;
	m_lstRadioInfo.GetClientRect(&rcRadioInfo);
	int nWidthRadioInfo = rcRadioInfo.Width();

	m_lstRadioInfo.InsertColumn(0, _T("Category"), LVCFMT_CENTER, static_cast<int>(nWidthRadioList * 0.25));
	m_lstRadioInfo.InsertColumn(1, _T("Value"), LVCFMT_LEFT, static_cast<int>(nWidthRadioList * 0.75));

	// Name
	m_lstRadioInfo.InsertItem(0, _T("Radio name"));

	// Genre
	m_lstRadioInfo.InsertItem(1, _T("Genre"));

	// Bit rate
	m_lstRadioInfo.InsertItem(2, _T("Bit rate"));

	// Notice
	m_lstRadioInfo.InsertItem(3, _T("Notice"));

	// Now playing
	m_lstRadioInfo.InsertItem(4, _T("Now playing"));
}

void CFreedomRadioVS2019Dlg::OnBnClickedBtnPlay()
{
	PlayRadio();
}


void CFreedomRadioVS2019Dlg::OnBnClickedBtnStop()
{
	StopRadio();
}


void CFreedomRadioVS2019Dlg::PlayRadio()
{
	int iRadio = m_lstRadioList.GetSelectionMark();
	if (iRadio == -1)
	{
		return;
	}

	// Set radio info
	CString csRadioName = m_lstRadioList.GetItemText(iRadio, 0);
	CString csRadioURL  = m_lstRadioList.GetItemText(iRadio, 1);
	std::string strRadioName = ConvertCStringToString(csRadioName);
	std::string strRadioURL  = ConvertCStringToString(csRadioURL);

	m_objRadioPlayer.SetRadioInfo(strRadioName, strRadioURL);

	// Add log
	CString csMsg = _T("");
	csMsg.Format(_T("Start play <%s>(%s)"), csRadioName, csRadioURL);
	AddLog(csMsg);
	
	// Let's play!
	m_objRadioPlayer.PlayRadio();
}


void CFreedomRadioVS2019Dlg::StopRadio()
{
	m_objRadioPlayer.StopRadio();
}


void CFreedomRadioVS2019Dlg::RefreshRadioInfo()
{
	// Sub name
	CString csValue(m_objRadioPlayer.GetRadioSubName().c_str());
	m_lstRadioInfo.SetItemText(0, 1, csValue);

	// Genre
	CString csSubName(m_objRadioPlayer.GetRadioGenre().c_str());
	m_lstRadioInfo.SetItemText(1, 1, csSubName);

	// Bit rate
	csValue.Format(_T("%d"), m_objRadioPlayer.GetRadioBitRate());
	m_lstRadioInfo.SetItemText(2, 1, csValue);

	// Notice
	CString csNotice(m_objRadioPlayer.GetRadioNotice().c_str());
	m_lstRadioInfo.SetItemText(3, 1, csNotice);
}


void CFreedomRadioVS2019Dlg::RefreshRadioNowPlaying()
{
	// Now playing
	CString csValue(m_objRadioPlayer.GetRadioNowPlaying().c_str());
	m_lstRadioInfo.SetItemText(4, 1, csValue);

	// Add log
	CString csMsg = _T("");
	csMsg.Format(_T("Now playing <%s>"), csValue);
	AddLog(csMsg);
}


void CFreedomRadioVS2019Dlg::AddLog(CString csMsg)
{
	CTime currentTime = CTime::GetCurrentTime();
	CString csLog = _T("");
	csLog.Format(_T("[%04d-%02d-%02d %02d:%02d:%02d] %s\r\n"),
		currentTime.GetYear(), currentTime.GetMonth(),  currentTime.GetDay(),
		currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(),
		csMsg);
	
	m_edtLog.SetSel(-2, -1);
	m_edtLog.ReplaceSel(csLog);
}


std::string CFreedomRadioVS2019Dlg::ConvertCStringToString(CString csValue)
{
	CT2CA pszConvertedAnsiString(csValue);
	std::string strValue(pszConvertedAnsiString);
	return strValue;
}


void CFreedomRadioVS2019Dlg::OpenChannelListFile()
{
	CFileDialog dlgOpen(TRUE, _T(".asx"), NULL, OFN_EXPLORER | OFN_HIDEREADONLY, _T("ASX file(*.asx)|*.asx|모든파일(*.*)|*.*||"));
	CString strPathProcess = GetProcessPath();

	dlgOpen.GetOFN().lpstrInitialDir = strPathProcess;
	int nResult = dlgOpen.DoModal();
	if (nResult == IDCANCEL)
	{
		return;
	}
	
	CString strPathChannelList = dlgOpen.GetPathName();
	if (strPathChannelList.IsEmpty() == TRUE)
	{
		return;
	}

	OpenChannelList(strPathChannelList);
}


void CFreedomRadioVS2019Dlg::OpenChannelList(CString strPathChannelList)
{
	m_objChannelManager.ClearAllChannel();

	std::string strPath = ConvertCStringToString(strPathChannelList);
	std::ifstream fileChannelList(strPath);
	
	std::string strLine;
	bool bIsOpenEntry = false;
	while (std::getline(fileChannelList, strLine))
	{
		// rtrim
		strLine = strLine.erase(strLine.find_last_not_of(' ') + 1);

		// ltrim
		strLine = strLine.erase(0, strLine.find_last_not_of(' '));

		if (bIsOpenEntry == true)
		{
			if (strLine.find("<title>") == 0)
			{
				;
			}
			else if (strLine.find("<ref href = ") == 0)
			{
				;
			}
			else if (strLine == "</entry>")
			{
				;
			}
		}
		else
		{
			;
		}
	}

	fileChannelList.close();
	
}


void CFreedomRadioVS2019Dlg::ProgramEnd()
{
	AfxGetMainWnd()->PostMessageW(WM_COMMAND, ID_APP_EXIT, 0);
}


void CFreedomRadioVS2019Dlg::OnNMDblclkLstRadioList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if (pNMItemActivate->iItem != -1)
	{
		PlayRadio();
	}

	*pResult = 0;
}


BOOL CFreedomRadioVS2019Dlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int nMenu = LOWORD(wParam);

	switch (nMenu)
	{
	case ID_MAIN_OPENCHANNELLIST:
		OpenChannelList();
		break;

	case ID_MAIN_EXIT:
		ProgramEnd();
		break;

	default:
		break;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
