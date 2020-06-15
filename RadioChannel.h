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

    std::wstring GetName() { return m_strName; }
    std::wstring GetURL()  { return m_strURL;  }

private:
    std::wstring m_strName;
    std::wstring m_strURL;
};

