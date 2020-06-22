#include "pch.h"
#include "RadioChannelView.h"

CRadioChannelView::CRadioChannelView()
{
    RegisterWindowClass();
    //InitControl();
}


CRadioChannelView::~CRadioChannelView()
{
    ;
}


void CRadioChannelView::InitControl()
{
    m_stcChannelName.Create(_T("NAME"),     WS_CHILD | WS_VISIBLE, CRect(5, 5, 25, 20), this);
    m_stcChannelArtist.Create(_T("ARTIST"), WS_CHILD | WS_VISIBLE, CRect(5, 22, 25, 37), this);
    m_stcChannelTitle.Create(_T("TITLE"),   WS_CHILD | WS_VISIBLE, CRect(5, 39, 25, 54), this);
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

BOOL CRadioChannelView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    InitControl();

    return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
