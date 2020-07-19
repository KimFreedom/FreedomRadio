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
    BOOL Create(long windowStyle, CRect rect, CWnd* pParent, UINT windowID);
    BOOL Create(CRect rect, CWnd* pParent, UINT windowID);
};

