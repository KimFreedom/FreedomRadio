#pragma once
#include <afxwin.h>

class CRadioChannelView :
    public CWnd
{
public:
    CRadioChannelView();
    virtual ~CRadioChannelView();

    void InitControl();

    CStatic m_stcChannelName;
    CStatic m_stcChannelArtist;
    CStatic m_stcChannelTitle;

protected:
    BOOL RegisterWindowClass();
public:
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};

