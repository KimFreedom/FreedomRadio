#pragma once
#include "HTTPRequest.hpp"

class CFreedomRadioPlayer
{
public:
    CFreedomRadioPlayer();
    virtual ~CFreedomRadioPlayer();

    void SetRadioInfo(std::string strName, std::string strURL) { m_strName = strName; m_strURL = strURL; };
    void SetRadioSubName(std::string strValue) { m_strSubName = strValue; };
    void SetRadioGenre(std::string strValue) { m_strGenre = strValue; };
    void SetRadioBitRate(int nValue) { m_nBitRate = nValue; };
    void SetRadioNotice(std::string strValue) { m_strNotice = strValue; };
    void SetRadioNowPlaying(std::string strValue) { m_strNowPlaying = strValue; };

    std::string GetRadioName() { return m_strName; };
    std::string GetRadioURL() { return m_strURL; };
    std::string GetRadioSubName() { return m_strSubName; };
    std::string GetRadioGenre() { return m_strGenre; };
    int         GetRadioBitRate() { return m_nBitRate; };
    std::string GetRadioNotice() { return m_strNotice; };
    std::string GetRadioNowPlaying() { return m_strNowPlaying; };

    void InitRadioInfo();
    void PlayRadio();
    void StopRadio();
    bool IsNewSong(std::string strSongTitle);

    bool   m_bIsPlayingRadio;
    HANDLE m_hRadioThread;
    HWND   m_hParentHandle;

private:
    std::string m_strName;
    std::string m_strURL;
    std::string m_strSubName;
    std::string m_strGenre;
    int         m_nBitRate;
    std::string m_strNotice;
    std::string m_strNowPlaying;
};

