#pragma once
#include <string>

class CRadioChannel
{

public:
    CRadioChannel();
    CRadioChannel(std::string strName, std::string strURL) { m_strName = strName; m_strURL = strURL; }
    virtual ~CRadioChannel();

    void SetName(std::string strValue) { m_strName = strValue; }
    void SetURL(std::string strValue)  { m_strURL  = strValue; }

    std::string GetName() { return m_strName; }
    std::string GetURL()  { return m_strURL;  }

private:
    std::string m_strName;
    std::string m_strURL;
};

