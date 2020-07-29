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
    void RefreshRadio();
    void ClearChannelView();

    BOOL Create(long windowStyle, CRect rect, CWnd* pParent, UINT windowID);
    BOOL Create(CRect rect, CWnd* pParent, UINT windowID);

    std::vector<CRadioChannelView *> m_vtChannelView;
    int m_iVisibleChannelIndexFirst;
    int m_iVisibleChannelIndexLast;

    CButton m_btnScrollUp;
    CButton m_btnScrollDown;

protected:
    BOOL RegisterWindowClass();
public:
    DECLARE_MESSAGE_MAP()
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};

