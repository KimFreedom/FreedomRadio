#pragma once
#include <afxwin.h>
#include <vector>
#include "RadioChannelView.h"

class CRadioChannelPanelWnd :
    public CWnd
{
public:
    CRadioChannelPanelWnd();
    virtual ~CRadioChannelPanelWnd();
    void InitControl();

    BOOL Create(long windowStyle, CRect rect, CWnd* pParent, UINT windowID);
    BOOL Create(CRect rect, CWnd* pParent, UINT windowID);

    //std::vector<CRadioChannelView> m_vtChannelView;
    CRadioChannelView m_pChannelView[3];

protected:
    BOOL RegisterWindowClass();
//public:
    //virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};

