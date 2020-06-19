#pragma once
#include <afxwin.h>

#define CRADIOCHANNELVIEW_CLASSNAME _T("MFCCRadioChannelViewCtrl")

class CRadioChannelView :
    public CWnd
{
public:
    CRadioChannelView();
    virtual ~CRadioChannelView();

protected:
    BOOL RegisterWindowClass();
};

