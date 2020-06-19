
// FreedomRadio_VS2019Dlg.h: 헤더 파일
//

#pragma once
#include "FreedomRadioPlayer.h"
#include "ChannelManager.h"
#include "RadioChannelPanelWnd.h"


// CFreedomRadioVS2019Dlg 대화 상자
class CFreedomRadioVS2019Dlg : public CDialogEx
{
// 생성입니다.
public:
	CFreedomRadioVS2019Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	void PlayRadio();
	void StopRadio();

	void OpenChannelListFile();
	void OpenChannelList(CString strPathChannelList);
	void RefreshRadioList();
	void ProgramEnd();

	CString GetProcessPath();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FREEDOMRADIO_VS2019_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	afx_msg LRESULT OnSendRadioSubInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSendRadioNowPlaying(WPARAM wParam, LPARAM lParam);

private:
	void InitControls();
	void RefreshRadioInfo();
	void RefreshRadioNowPlaying();
	void AddLog(CString csMsg);

	std::wstring ConvertCStringToString(CString csValue);
	

	CFreedomRadioPlayer		m_objRadioPlayer;
	CChannelManager			m_objChannelManager;
	CRadioChannelPanelWnd	m_wndRadioChannelPanel;


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstRadioList;
	CListCtrl m_lstRadioInfo;
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnStop();
	CEdit m_edtLog;
    afx_msg void OnNMDblclkLstRadioList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
