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
    m_fontKey.DeleteObject();
    m_fontValue.DeleteObject();
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


void CRadioChannelView::InitControl()
{
    BOOL bResult = FALSE;
    const DWORD dwKeyStyle = WS_CHILD | WS_VISIBLE | SS_RIGHT | WS_CLIPCHILDREN;
    const DWORD dwValueStyle = WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN;
    CRect rcChannelView;
    GetClientRect(&rcChannelView);

    m_fontKey.CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("¸¼Àº °íµñ"));
    m_fontValue.CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, 0, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("¸¼Àº °íµñ"));

    const CRect rcNameKey(CHANNEL_CONTROL_PADDING, CHANNEL_CONTROL_PADDING, 
        CHANNEL_CONTROL_PADDING + CHANNEL_STATIC_WIDTH, CHANNEL_CONTROL_PADDING + CHANNEL_STATIC_HEIGHT);
    const CRect rcArtistKey(CHANNEL_CONTROL_PADDING, rcNameKey.bottom + CHANNEL_CONTROL_MARGIN,
        rcNameKey.right, rcNameKey.bottom + CHANNEL_CONTROL_MARGIN + CHANNEL_STATIC_HEIGHT);
    const CRect rcTitleKey(CHANNEL_CONTROL_PADDING, rcArtistKey.bottom + CHANNEL_CONTROL_MARGIN,
        rcNameKey.right, rcArtistKey.bottom + CHANNEL_CONTROL_MARGIN + CHANNEL_STATIC_HEIGHT);

    const CRect rcNameValue(rcNameKey.right + CHANNEL_CONTROL_PADDING, rcNameKey.top,
        rcChannelView.right - CHANNEL_CONTROL_PADDING, rcNameKey.bottom);
    const CRect rcArtistValue(rcNameValue.left, rcArtistKey.top,
        rcNameValue.right, rcArtistKey.bottom);
    const CRect rcTitleValue(rcNameValue.left, rcTitleKey.top,
        rcNameValue.right, rcTitleKey.bottom);

    m_stcNameKey.Create(_T("NAME"), dwKeyStyle, rcNameKey, this);
    m_stcArtistKey.Create(_T("ARTIST"), dwKeyStyle, rcArtistKey, this);
    m_stcTitleKey.Create(_T("TITLE"), dwKeyStyle, rcTitleKey, this);
    m_stcNameValue.Create(_T("-"), dwValueStyle, rcNameValue, this);
    m_stcArtistValue.Create(_T("-"), dwValueStyle, rcArtistValue, this);
    m_stcTitleValue.Create(_T("-"), dwValueStyle, rcTitleValue, this);

    m_stcNameKey.SetFont(&m_fontKey, TRUE);
    m_stcArtistKey.SetFont(&m_fontKey, TRUE);
    m_stcTitleKey.SetFont(&m_fontKey, TRUE);
    m_stcNameValue.SetFont(&m_fontValue, TRUE);
    m_stcArtistValue.SetFont(&m_fontValue, TRUE);
    m_stcTitleValue.SetFont(&m_fontValue, TRUE);
}


void CRadioChannelView::SetName(CString strValue)
{
    m_stcNameValue.SetWindowText(strValue);
}


void CRadioChannelView::SetArtist(CString strValue)
{
    m_stcArtistValue.SetWindowText(strValue);
}


void CRadioChannelView::SetTitle(CString strValue)
{
    m_stcTitleValue.SetWindowText(strValue);
}


