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
    //RegisterWindowClass();
    //InitControl();
}


CRadioChannelPanelWnd::~CRadioChannelPanelWnd()
{
    ;
}


void CRadioChannelPanelWnd::InitControl()
{
    CRect rcPanelWnd;
    GetClientRect(&rcPanelWnd);
    const int CHANNEL_WIDTH = (rcPanelWnd.Width() - (CHANNEL_MARGIN * 3)) / 2;
    const int LEFT_CHANNEL_LEFT_POS = CHANNEL_MARGIN;
    const int RIGHT_CHANNEL_LEFT_POS = (CHANNEL_MARGIN * 2) + CHANNEL_WIDTH;
    for (int i = 0; i < 20; i++)
    {
        CRect rcChannelView(0, ((CHANNEL_MARGIN + CHANNEL_HEIGHT) * (i / 2)) + CHANNEL_MARGIN, 
            0, ((CHANNEL_MARGIN + CHANNEL_HEIGHT) * (i / 2)) + CHANNEL_MARGIN + CHANNEL_HEIGHT);
        if ((i % 2) == 0)
        {
            rcChannelView.left  = LEFT_CHANNEL_LEFT_POS;
            rcChannelView.right = LEFT_CHANNEL_LEFT_POS + CHANNEL_WIDTH;
        }
        else
        {
            rcChannelView.left = LEFT_CHANNEL_LEFT_POS + CHANNEL_WIDTH + CHANNEL_MARGIN;
            rcChannelView.right = rcPanelWnd.right - CHANNEL_MARGIN;
        }

        BOOL bResult = m_pChannelView[i].Create(rcChannelView, this, 10000 + i);
        m_pChannelView[i].EnableWindow(TRUE);
        m_pChannelView[i].ShowWindow(SW_SHOW);
        

        /*
        CRadioChannelView objChannelView;
        objChannelView.Create(NULL, _T(""), WS_BORDER, CRect(LEFT_POS, CHANNEL_MARGIN, LEFT_POS + CHANNEL_WIDTH, CHANNEL_MARGIN + CHANNEL_HEIGHT), this, 10000 + i);
        m_vtChannelView.push_back(objChannelView);
        */
    }
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
*/