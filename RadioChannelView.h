#pragma once
#include "FreedomRadioPlayer.h"
#include <afxwin.h>

class CRadioChannelView :
    public CWnd
{
public:
    CRadioChannelView();
    virtual ~CRadioChannelView();

    void InitControl();
    void SetRadioInfo(std::wstring strName, std::wstring strURL);
    void SetName(CString csValue);
    void SetURL(CString csValue);
    void SetArtist(CString csValue);
    void SetTitle(CString csValue);
    void RefreshNowPlaying();
    void StopRefreshing();
    BOOL GetRadioAvailable();

    std::wstring ConvertCStringToString(CString csValue);
    void TraceLog(TCHAR* szLog, ...);

    CFreedomRadioPlayer m_objRadioPlayer;
    CString m_csName;
    CString m_csURL;
    BOOL    m_bIsVisible;
    BOOL    m_bActiveThread;
    HANDLE  m_hRadioTimerThread;
    int     m_nThreadState;

    CFont m_fontKey, m_fontValue;

    CStatic m_stcNameKey;
    CStatic m_stcArtistKey;
    CStatic m_stcTitleKey;
    CStatic m_stcNameValue;
    CStatic m_stcArtistValue;
    CStatic m_stcTitleValue;

protected:
    BOOL RegisterWindowClass();
    afx_msg LRESULT OnSendRadioSubInfo(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnSendRadioNowPlaying(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
public:
    BOOL Create(long windowStyle, CRect rect, CWnd* pParent, UINT windowID);
    BOOL Create(CRect rect, CWnd* pParent, UINT windowID);
};

