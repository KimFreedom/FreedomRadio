#include "pch.h"
#include "RadioChannelPanelWnd.h"

CRadioChannelPanelWnd::CRadioChannelPanelWnd()
{
    RegisterWindowClass();
    //InitControl();
}


CRadioChannelPanelWnd::~CRadioChannelPanelWnd()
{
    ;
}


void CRadioChannelPanelWnd::InitControl()
{
    for (int i = 0; i < 3; i++)
    {
        const int LEFT_POS = ((CHANNEL_MARGIN + CHANNEL_WIDTH) * i) + CHANNEL_MARGIN;

        //BOOL bResult = m_pChannelView[i].Create(NULL, _T(""), WS_BORDER, CRect(LEFT_POS, CHANNEL_MARGIN, LEFT_POS + CHANNEL_WIDTH, CHANNEL_MARGIN + CHANNEL_HEIGHT), this, 10000 + i);
        BOOL bResult = m_pChannelView[i].Create(CRADIOCHANNELVIEW_CLASSNAME, _T(""), WS_BORDER, 
            CRect(LEFT_POS, CHANNEL_MARGIN, LEFT_POS + CHANNEL_WIDTH, CHANNEL_MARGIN + CHANNEL_HEIGHT), this, 10000 + i);
        m_pChannelView[i].EnableWindow(TRUE);
        m_pChannelView[i].ShowWindow(SW_SHOW);

        /*
        CRadioChannelView objChannelView;
        objChannelView.Create(NULL, _T(""), WS_BORDER, CRect(LEFT_POS, CHANNEL_MARGIN, LEFT_POS + CHANNEL_WIDTH, CHANNEL_MARGIN + CHANNEL_HEIGHT), this, 10000 + i);
        m_vtChannelView.push_back(objChannelView);
        */
    }
}


BOOL CRadioChannelPanelWnd::RegisterWindowClass()
{
    WNDCLASS    wndcls;
    HINSTANCE   hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, CRADIOCHANNELPANELWND_CLASSNAME, &wndcls)))
    {
        wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc = ::DefWindowProc;
        wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
        wndcls.hInstance = hInst;
        wndcls.hIcon = NULL;
        wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
        wndcls.lpszMenuName = NULL;
        wndcls.lpszClassName = CRADIOCHANNELPANELWND_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CRadioChannelPanelWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    InitControl();

    return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
