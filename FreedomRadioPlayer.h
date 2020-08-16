#pragma once
#include "HTTPRequest.hpp"

class CFreedomRadioPlayer
{
public:
    CFreedomRadioPlayer();
    virtual ~CFreedomRadioPlayer();

    void SetRadioInfo(std::wstring strName, std::wstring strURL) { m_strName = strName; m_strURL = strURL; };
    void SetRadioSubName(std::wstring strValue) { m_strSubName = strValue; };
    void SetRadioGenre(std::wstring strValue) { m_strGenre = strValue; };
    void SetRadioBitRate(int nValue) { m_nBitRate = nValue; };
    void SetRadioNotice(std::wstring strValue) { m_strNotice = strValue; };
    void SetRadioNowPlaying(std::wstring strValue) { m_strNowPlaying = strValue; };

    std::wstring GetRadioName() { return m_strName; };
    std::wstring GetRadioURL() { return m_strURL; };
    std::wstring GetRadioSubName() { return m_strSubName; };
    std::wstring GetRadioGenre() { return m_strGenre; };
    int         GetRadioBitRate() { return m_nBitRate; };
    std::wstring GetRadioNotice() { return m_strNotice; };
    std::wstring GetRadioNowPlaying() { return m_strNowPlaying; };

    void InitRadioInfo();
    void PlayRadio();
    void StopRadio();
    BOOL IsNewSong(std::wstring strSongTitle);
    void TraceLog(const TCHAR* szLog, ...);

    BOOL   m_bIsPlayingRadio;
    HANDLE m_hRadioThread;
    HWND   m_hParentHandle;
    int    m_nThreadState;
    BOOL   m_bIsRadioAvailable;

private:
    std::wstring m_strName;
    std::wstring m_strURL;
    std::wstring m_strSubName;
    std::wstring m_strGenre;
    int         m_nBitRate;
    std::wstring m_strNotice;
    std::wstring m_strNowPlaying;
};

