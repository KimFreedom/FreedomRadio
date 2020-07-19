#include "pch.h"
#include "RadioChannelView.h"

ATOM RegisterClassRadioChannelView(HINSTANCE hInstance)
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
    wcex.lpszClassName = CRADIOCHANNELVIEW_CLASSNAME;
    wcex.hIconSm = NULL;
    return RegisterClassEx(&wcex);
}

CRadioChannelView::CRadioChannelView()
{
    ;
}


CRadioChannelView::~CRadioChannelView()
{
    ;
}


void CRadioChannelView::InitControl()
{
    BOOL bResult = FALSE;
    const DWORD dwStaticStyle = WS_CHILD | WS_VISIBLE | SS_RIGHT;
    const CRect rcName(CHANNEL_CONTROL_PADDING, CHANNEL_CONTROL_PADDING, 
        CHANNEL_CONTROL_PADDING + CHANNEL_STATIC_WIDTH, CHANNEL_CONTROL_PADDING + CHANNEL_STATIC_HEIGHT);
    const CRect rcArtist(CHANNEL_CONTROL_PADDING, rcName.bottom + CHANNEL_CONTROL_MARGIN,
        rcName.right, rcName.bottom + CHANNEL_CONTROL_MARGIN + CHANNEL_STATIC_HEIGHT);
    const CRect rcTitle(CHANNEL_CONTROL_PADDING, rcArtist.bottom + CHANNEL_CONTROL_MARGIN,
        rcName.right, rcArtist.bottom + CHANNEL_CONTROL_MARGIN + CHANNEL_STATIC_HEIGHT);

    bResult = m_stcChannelName.Create(_T("NAME"), dwStaticStyle, rcName, this);
    bResult = m_stcChannelArtist.Create(_T("ARTIST"), dwStaticStyle, rcArtist, this);
    bResult = m_stcChannelTitle.Create(_T("TITLE"), dwStaticStyle, rcTitle, this);
}


BOOL CRadioChannelView::RegisterWindowClass()
{
    WNDCLASS    wndcls;
    HINSTANCE   hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, CRADIOCHANNELVIEW_CLASSNAME, &wndcls)))
    {
        wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc = ::DefWindowProc;
        wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
        wndcls.hInstance = hInst;
        wndcls.hIcon = NULL;
        wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
        wndcls.lpszMenuName = NULL;
        wndcls.lpszClassName = CRADIOCHANNELVIEW_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CRadioChannelView::Create(long windowStyle, CRect rect, CWnd* pParent, UINT windowID)
{
    WNDCLASS WndClass;
    if (GetClassInfo(AfxGetInstanceHandle(), CRADIOCHANNELVIEW_CLASSNAME, &WndClass) == FALSE)
    {
        RegisterClassRadioChannelView(AfxGetInstanceHandle());
    }
    return CreateEx(0, CRADIOCHANNELVIEW_CLASSNAME, CRADIOCHANNELVIEW_CLASSNAME, windowStyle, rect, pParent, windowID);
}


BOOL CRadioChannelView::Create(CRect rect, CWnd* pParent, UINT windowID)
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
