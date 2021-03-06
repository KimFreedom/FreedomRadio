﻿#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


#define UM_SEND_RADIO_SUB_INFO      (WM_USER + 1000)
#define UM_SEND_RADIO_NOW_PLAYING   (WM_USER + 1001)



///////////// Consts
#define CRADIOCHANNELPANELWND_CLASSNAME _T("MFCCRadioChannelPanelWndCtrl")
#define CRADIOCHANNELVIEW_CLASSNAME _T("MFCCRadioChannelViewCtrl")

#define CHANNEL_STATIC_WIDTH    50
#define CHANNEL_STATIC_HEIGHT   15
#define CHANNEL_EDIT_WIDTH_MIN  100
#define CHANNEL_EDIT_HEIGHT     15
#define CHANNEL_CONTROL_MARGIN  2
#define CHANNEL_CONTROL_PADDING 5

#define CHANNEL_MARGIN          5
//#define CHANNEL_WIDTH           (CHANNEL_CONTROL_PADDING * 2) + CHANNEL_STATIC_WIDTH + CHANNEL_CONTROL_MARGIN + CHANNEL_EDIT_WIDTH_MIN
#define CHANNEL_HEIGHT          (CHANNEL_CONTROL_PADDING * 2) + (CHANNEL_STATIC_HEIGHT * 3) + (CHANNEL_CONTROL_MARGIN * 2)

#define SCROLL_WIDTH            50

#define THREAD_STATE_END        0
#define THREAD_STATE_RUNNING    1

#define TIMER_RADIO_REFRESH     WM_USER + 1100


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
