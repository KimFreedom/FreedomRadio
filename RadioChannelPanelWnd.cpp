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
    m_iVisibleChannelIndexFirst = -1;
    m_iVisibleChannelIndexLast = -1;
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
    ShowWindow(SW_HIDE);
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
        //CString csName(pChannel->GetName().c_str());
        bResult = objChannel->Create(rcChannelView, this, 10000 + i);
        //objChannel->SetName(csName);
        objChannel->SetRadioInfo(pChannel->GetName(), pChannel->GetURL());
        objChannel->EnableWindow(TRUE);
        objChannel->ShowWindow(SW_SHOW);
        m_vtChannelView.push_back(objChannel);
    }
    ShowWindow(SW_SHOW);

    if (nChannelCount > 0)
    {
        m_iVisibleChannelIndexFirst = 0;
        m_iVisibleChannelIndexLast = min((DWORD)nChannelCount - 1, 13);
        for (int i = m_iVisibleChannelIndexFirst; i <= m_iVisibleChannelIndexLast; i++)
        {
            m_vtChannelView[i]->m_bIsVisible = TRUE;
        }
    }

    //SetTimer(TIMER_RADIO_REFRESH, 10000, NULL);
}


void CRadioChannelPanelWnd::RefreshRadio()
{
    if ((m_iVisibleChannelIndexFirst < 0) || (m_iVisibleChannelIndexLast < 0))
    {
        return;
    }
    for (int i = m_iVisibleChannelIndexFirst; i <= m_iVisibleChannelIndexLast; i++)
    {
        m_vtChannelView[i]->RefreshNowPlaying();
    }
}


void CRadioChannelPanelWnd::ClearChannelView()
{
    for (int i = 0; i < m_vtChannelView.size(); i++)
    {
        m_vtChannelView[i]->StopRefreshing();
        //m_vtChannelView[i]->DestroyWindow();
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

BEGIN_MESSAGE_MAP(CRadioChannelPanelWnd, CWnd)
    ON_WM_SIZE()
    ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CRadioChannelPanelWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
    CRect rcItem;
    const int MOVE_POWER = CHANNEL_HEIGHT + CHANNEL_MARGIN;
    int nChannelCount = m_vtChannelView.size();
    HDWP hdwp = ::BeginDeferWindowPos(nChannelCount);

    switch (wParam)
    {
    case 11000: // UP
        if (m_iVisibleChannelIndexFirst <= 0)
        {
            break;
        }
        for (int i = 0; i < nChannelCount; i++)
        {
            m_vtChannelView[i]->GetWindowRect(&rcItem);
            ScreenToClient(rcItem);
            rcItem.top += MOVE_POWER;
            rcItem.bottom += MOVE_POWER;
            ::DeferWindowPos(hdwp, m_vtChannelView[i]->GetSafeHwnd(), HWND_TOP,
                rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(),
                SWP_NOZORDER);
        }
        m_iVisibleChannelIndexFirst -= 2;
        m_iVisibleChannelIndexLast = m_iVisibleChannelIndexFirst + 13;
        if (m_iVisibleChannelIndexLast + 1 < nChannelCount)
        {
            m_vtChannelView[m_iVisibleChannelIndexLast + 1]->m_bIsVisible = FALSE;
        }
        if (m_iVisibleChannelIndexLast + 2 < nChannelCount)
        {
            m_vtChannelView[m_iVisibleChannelIndexLast + 2]->m_bIsVisible = FALSE;
        }
        m_vtChannelView[m_iVisibleChannelIndexFirst]->m_bIsVisible = TRUE;
        m_vtChannelView[m_iVisibleChannelIndexFirst + 1]->m_bIsVisible = TRUE;
        break;

    case 11001: // DOWN
        if ((m_iVisibleChannelIndexLast <= 0) || (m_iVisibleChannelIndexLast == nChannelCount - 1))
        {
            break;
        }
        for (int i = 0; i < nChannelCount; i++)
        {
            m_vtChannelView[i]->GetWindowRect(&rcItem);
            ScreenToClient(rcItem);
            rcItem.top -= MOVE_POWER;
            rcItem.bottom -= MOVE_POWER;
            ::DeferWindowPos(hdwp, m_vtChannelView[i]->GetSafeHwnd(), HWND_TOP,
                rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(),
                SWP_NOZORDER);
        }
        m_iVisibleChannelIndexFirst += 2;
        m_iVisibleChannelIndexLast = min(m_iVisibleChannelIndexFirst + 13, nChannelCount - 1);
        m_vtChannelView[m_iVisibleChannelIndexFirst - 2]->m_bIsVisible = FALSE;
        m_vtChannelView[m_iVisibleChannelIndexFirst - 1]->m_bIsVisible = FALSE;
        m_vtChannelView[m_iVisibleChannelIndexLast]->m_bIsVisible = TRUE;
        m_vtChannelView[m_iVisibleChannelIndexLast - 1]->m_bIsVisible = TRUE;
        break;

    default:
        break;
    }

    ::EndDeferWindowPos(hdwp);

    return CWnd::OnCommand(wParam, lParam);
}



void CRadioChannelPanelWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    CRect rcItem, rcPanelWnd;
    int nChannelCount = m_vtChannelView.size();
    HDWP hdwp = ::BeginDeferWindowPos(nChannelCount + 2);
    GetClientRect(&rcPanelWnd);
    const int CHANNEL_WIDTH = (rcPanelWnd.Width() - (CHANNEL_MARGIN * 3) - SCROLL_WIDTH) / 2;
    const int LEFT_CHANNEL_LEFT_POS = CHANNEL_MARGIN;
    const int RIGHT_CHANNEL_LEFT_POS = (CHANNEL_MARGIN * 2) + CHANNEL_WIDTH;
    const CRect rcScrollUp(rcPanelWnd.right - SCROLL_WIDTH + CHANNEL_CONTROL_PADDING, CHANNEL_CONTROL_PADDING,
        rcPanelWnd.right - CHANNEL_CONTROL_PADDING, CHANNEL_CONTROL_PADDING + SCROLL_WIDTH);
    const CRect rcScrollDown(rcScrollUp.left, rcPanelWnd.bottom - CHANNEL_CONTROL_MARGIN - SCROLL_WIDTH,
        rcScrollUp.right, rcPanelWnd.bottom - CHANNEL_CONTROL_MARGIN);

    for (int i = 0; i < nChannelCount; i++)
    {
        m_vtChannelView[i]->GetWindowRect(&rcItem);
        ScreenToClient(rcItem);
        if ((i % 2) == 0)
        {
            rcItem.left = LEFT_CHANNEL_LEFT_POS;
            rcItem.right = LEFT_CHANNEL_LEFT_POS + CHANNEL_WIDTH;
        }
        else
        {
            rcItem.left = LEFT_CHANNEL_LEFT_POS + CHANNEL_WIDTH + CHANNEL_MARGIN;
            rcItem.right = rcItem.left + CHANNEL_WIDTH;
        }
        ::DeferWindowPos(hdwp, m_vtChannelView[i]->GetSafeHwnd(), HWND_TOP,
            rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(),
            SWP_NOZORDER);
    }
    ::DeferWindowPos(hdwp, m_btnScrollUp.GetSafeHwnd(), HWND_TOP,
        rcScrollUp.left, rcScrollUp.top, rcScrollUp.Width(), rcScrollUp.Height(),
        SWP_NOZORDER | SWP_NOSIZE);
    ::DeferWindowPos(hdwp, m_btnScrollDown.GetSafeHwnd(), HWND_TOP,
        rcScrollDown.left, rcScrollDown.top, rcScrollDown.Width(), rcScrollDown.Height(),
        SWP_NOZORDER | SWP_NOSIZE);

    ::EndDeferWindowPos(hdwp);
}

void CRadioChannelPanelWnd::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case TIMER_RADIO_REFRESH:
        //RefreshRadio();
        break;
    }

    CWnd::OnTimer(nIDEvent);
}
