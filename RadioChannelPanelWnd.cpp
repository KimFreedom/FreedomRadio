#include "pch.h"
#include "RadioChannelPanelWnd.h"


ATOM RegisterClassRadioChannelPanelWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    wcex.lpfnWndProc = (WNDPROC)DefWindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    LOGBRUSH brush;
    brush.lbStyle = BS_HOLLOW;
    brush.lbColor = RGB(255, 255, 255);
    brush.lbHatch = HS_CROSS;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = CRADIOCHANNELPANELWND_CLASSNAME;
    wcex.hIconSm = NULL;
    return RegisterClassEx(&wcex);
}


CRadioChannelPanelWnd::CRadioChannelPanelWnd()
{
    ;
}


CRadioChannelPanelWnd::~CRadioChannelPanelWnd()
{
    ClearChannelView();
}


void CRadioChannelPanelWnd::InitControl()
{
    BOOL bResult = FALSE;
    CRect rcPanelWnd;
    GetClientRect(&rcPanelWnd);
    const DWORD dwStaticStyle = WS_CHILD | WS_VISIBLE;
    const CRect rcScrollUp(rcPanelWnd.right - SCROLL_WIDTH + CHANNEL_CONTROL_PADDING, CHANNEL_CONTROL_PADDING,
        rcPanelWnd.right - CHANNEL_CONTROL_PADDING, CHANNEL_CONTROL_PADDING + SCROLL_WIDTH);
    const CRect rcScrollDown(rcScrollUp.left, rcPanelWnd.bottom - CHANNEL_CONTROL_MARGIN - SCROLL_WIDTH,
        rcScrollUp.right, rcPanelWnd.bottom - CHANNEL_CONTROL_MARGIN);

    bResult = m_btnScrollUp.Create(_T("UP"), dwStaticStyle, rcScrollUp, this, 11000);
    bResult = m_btnScrollDown.Create(_T("DOWN"), dwStaticStyle, rcScrollDown, this, 11001);
}


void CRadioChannelPanelWnd::RefreshChannelList(CChannelManager& objChannelManager)
{
    ClearChannelView();

    int nChannelCount = objChannelManager.CountChannelInfo();
    BOOL bResult = FALSE;
    CRect rcPanelWnd;
    GetClientRect(&rcPanelWnd);
    const int CHANNEL_WIDTH = (rcPanelWnd.Width() - (CHANNEL_MARGIN * 3) - SCROLL_WIDTH) / 2;
    const int LEFT_CHANNEL_LEFT_POS = CHANNEL_MARGIN;
    const int RIGHT_CHANNEL_LEFT_POS = (CHANNEL_MARGIN * 2) + CHANNEL_WIDTH;
    for (int i = 0; i < nChannelCount; i++)
    {
        CRect rcChannelView(0, ((CHANNEL_MARGIN + CHANNEL_HEIGHT) * (i / 2)) + CHANNEL_MARGIN,
            0, ((CHANNEL_MARGIN + CHANNEL_HEIGHT) * (i / 2)) + CHANNEL_MARGIN + CHANNEL_HEIGHT);
        if ((i % 2) == 0)
        {
            rcChannelView.left = LEFT_CHANNEL_LEFT_POS;
            rcChannelView.right = LEFT_CHANNEL_LEFT_POS + CHANNEL_WIDTH;
        }
        else
        {
            rcChannelView.left = LEFT_CHANNEL_LEFT_POS + CHANNEL_WIDTH + CHANNEL_MARGIN;
            rcChannelView.right = rcChannelView.left + CHANNEL_WIDTH;
        }
        
        CRadioChannelView *objChannel = new CRadioChannelView();
        CRadioChannel* pChannel = objChannelManager.GetChannelInfo(i);
        CString csName(pChannel->GetName().c_str());
        bResult = objChannel->Create(rcChannelView, this, 10000 + i);
        objChannel->SetName(csName);
        objChannel->EnableWindow(TRUE);
        objChannel->ShowWindow(SW_SHOW);
        m_vtChannelView.push_back(objChannel);
    }
}


void CRadioChannelPanelWnd::ClearChannelView()
{
    for (int i = 0; i < m_vtChannelView.size(); i++)
    {
        m_vtChannelView[i]->DestroyWindow();
        delete m_vtChannelView[i];
    }
    m_vtChannelView.clear();
}


BOOL CRadioChannelPanelWnd::Create(long windowStyle, CRect rect, CWnd* pParent, UINT windowID)
{
    WNDCLASS WndClass;
    if (GetClassInfo(AfxGetInstanceHandle(), CRADIOCHANNELPANELWND_CLASSNAME, &WndClass) == FALSE)
    {
        RegisterClassRadioChannelPanelWindow(AfxGetInstanceHandle());
    }
    return CreateEx(0, CRADIOCHANNELPANELWND_CLASSNAME, CRADIOCHANNELPANELWND_CLASSNAME, windowStyle, rect, pParent, windowID);
}


BOOL CRadioChannelPanelWnd::Create(CRect rect, CWnd* pParent, UINT windowID)
{
    LONG lws = WS_VISIBLE | WS_BORDER | WS_CHILD;
    BOOL bRet = Create(lws, rect, pParent, windowID);
    //BOOL bRet = Create(lws, CRect(0, 0, rect.Width(), rect.Height()), pParent, windowID);
    if (bRet == TRUE)
    {
        InitControl();
        ShowWindow(SW_SHOW);
    }

    return bRet;
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

/*
BOOL CRadioChannelPanelWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    InitControl();

    return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
*/BEGIN_MESSAGE_MAP(CRadioChannelPanelWnd, CWnd)
ON_WM_KEYDOWN()
END_MESSAGE_MAP()


void CRadioChannelPanelWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    /*
    CRect rcItem;

    switch (nChar)
    {
    case VK_UP:
        for (int i = 0; i < 20; i++)
        {
            m_pChannelView[i].GetWindowRect(&rcItem);
            ScreenToClient(rcItem);
            rcItem.top    -= 1;
            rcItem.bottom -= 1;
            m_pChannelView[i].MoveWindow(rcItem);
        }
        break;

    case VK_DOWN:
        for (int i = 0; i < 20; i++)
        {
            m_pChannelView[i].GetWindowRect(&rcItem);
            ScreenToClient(rcItem);
            rcItem.top += 1;
            rcItem.bottom += 1;
            m_pChannelView[i].MoveWindow(rcItem);
        }
        break;

    default:
        break;
    }
    */

    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CRadioChannelPanelWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
    CRect rcItem, rcPanelWnd;
    const int MOVE_POWER = CHANNEL_HEIGHT + CHANNEL_MARGIN;
    int nChannelCount = m_vtChannelView.size();
    GetClientRect(&rcPanelWnd);

    switch (wParam)
    {
    case 11000: // UP
        for (int i = 0; i < nChannelCount; i++)
        {
            m_vtChannelView[i]->GetWindowRect(&rcItem);
            ScreenToClient(rcItem);
            rcItem.top += MOVE_POWER;
            rcItem.bottom += MOVE_POWER;
            m_vtChannelView[i]->MoveWindow(rcItem, FALSE);
        }
        RedrawWindow(rcPanelWnd);
        break;

    case 11001: // DOWN
        for (int i = 0; i < nChannelCount; i++)
        {
            m_vtChannelView[i]->GetWindowRect(&rcItem);
            ScreenToClient(rcItem);
            rcItem.top -= MOVE_POWER;
            rcItem.bottom -= MOVE_POWER;
            m_vtChannelView[i]->MoveWindow(rcItem, FALSE);
        }
        RedrawWindow(rcPanelWnd);
        break;

    default:
        break;
    }

    return CWnd::OnCommand(wParam, lParam);
}

