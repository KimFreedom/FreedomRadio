#pragma once
#include <afxwin.h>

#define CRADIOCHANNELPANELWND_CLASSNAME _T("MFCCRadioChannelPanelWndCtrl")

class CRadioChannelPanelWnd :
    public CWnd
{
public:
    CRadioChannelPanelWnd();
    virtual ~CRadioChannelPanelWnd();

protected:
    BOOL RegisterWindowClass();
};

