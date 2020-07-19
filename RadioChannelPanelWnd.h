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
    CRadioChannelView m_pChannelView[20];

protected:
    BOOL RegisterWindowClass();
};

