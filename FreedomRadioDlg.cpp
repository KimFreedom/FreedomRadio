
// FreedomRadioDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FreedomRadio.h"
#include "FreedomRadioDlg.h"
#include "afxdialogex.h"
#include "HTTPRequest.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFreedomRadioDlg 대화 상자



CFreedomRadioDlg::CFreedomRadioDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FREEDOMRADIO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFreedomRadioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFreedomRadioDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TEST1, &CFreedomRadioDlg::OnBnClickedBtnTest1)
END_MESSAGE_MAP()


// CFreedomRadioDlg 메시지 처리기

BOOL CFreedomRadioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	ShowWindow(SW_MINIMIZE);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFreedomRadioDlg::OnPaint()
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
HCURSOR CFreedomRadioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFreedomRadioDlg::OnBnClickedBtnTest1()
{
	std::string url = "http://nonamepiano.saycast.com";
	std::string method = "GET";
	std::string arguments;
	CString strMsg = _T("");
	http::InternetProtocol protocol = http::InternetProtocol::V4;

	try
	{
		http::Request request(url, protocol);
		const http::Response response = request.send(method, arguments, {
			"Content-Type: application/x-www-form-urlencoded",
				"User-Agent: FreedomRadio/0.1"
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
				const http::Response serverResponse = serverRequest.send(method, arguments, {
					"Content-Type: application/x-www-form-urlencoded",
						"User-Agent: FreedomRadio/0.1"
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
