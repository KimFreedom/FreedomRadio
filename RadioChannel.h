#pragma once
#include <string>

class CRadioChannel
{

public:
    CRadioChannel() {}
    CRadioChannel(std::wstring strName, std::wstring strURL) { m_strName = strName; m_strURL = strURL; }
    virtual ~CRadioChannel() {}

    void SetName(std::wstring strValue) { m_strName = strValue; }
    void SetURL(std::wstring strValue)  { m_strURL  = strValue; }
    void SetNowPlayingArtist(std::wstring strValue) { m_strNowPlayingArtist = strValue; }
    void SetNowPlayingTitle(std::wstring strValue)  { m_strNowPlayingTitle  = strValue; }

    std::wstring GetName() { return m_strName; }
    std::wstring GetURL()  { return m_strURL;  }
    std::wstring GetNowPlayingArtist() { return m_strNowPlayingArtist; }
    std::wstring GetNowPlayingTitle()  { return m_strNowPlayingTitle; }

private:
    std::wstring m_strName;
    std::wstring m_strURL;
    std::wstring m_strNowPlayingArtist;
    std::wstring m_strNowPlayingTitle;
};

