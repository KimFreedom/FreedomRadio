#pragma once
#include <afxwin.h>

class CRadioChannelView :
    public CWnd
{
public:
    CRadioChannelView();
    virtual ~CRadioChannelView();

    void InitControl();
    void SetName(CString strValue);
    void SetArtist(CString strValue);
    void SetTitle(CString strValue);
    void SetNowPlaying();

    CFont m_fontKey, m_fontValue;

    CStatic m_stcNameKey;
    CStatic m_stcArtistKey;
    CStatic m_stcTitleKey;
    CStatic m_stcNameValue;
    CStatic m_stcArtistValue;
    CStatic m_stcTitleValue;

protected:
    BOOL RegisterWindowClass();
public:
    BOOL Create(long windowStyle, CRect rect, CWnd* pParent, UINT windowID);
    BOOL Create(CRect rect, CWnd* pParent, UINT windowID);
};

