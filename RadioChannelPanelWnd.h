#pragma once
#include <afxwin.h>
#include <vector>
#include "RadioChannelView.h"
#include "ChannelManager.h"

class CRadioChannelPanelWnd :
    public CWnd
{
public:
    CRadioChannelPanelWnd();
    virtual ~CRadioChannelPanelWnd();
    void InitControl();
    void RefreshChannelList(CChannelManager& objChannelManager);
    void ClearChannelView();

    BOOL Create(long windowStyle, CRect rect, CWnd* pParent, UINT windowID);
    BOOL Create(CRect rect, CWnd* pParent, UINT windowID);

    std::vector<CRadioChannelView *> m_vtChannelView;
    //CRadioChannelView m_pChannelView[20];

    CButton m_btnScrollUp;
    CButton m_btnScrollDown;

protected:
    BOOL RegisterWindowClass();
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

