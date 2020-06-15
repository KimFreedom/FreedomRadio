#pragma once
#include "RadioChannel.h"
#include <string>
#include <vector>

class CChannelManager
{
public:
    CChannelManager() {}
    virtual ~CChannelManager() {}

    void AddChannel(std::wstring strName, std::wstring strURL);
    int  CountChannelInfo();
    void GetChannelInfo(int iChannel, std::wstring& strName, std::wstring& strURL);
    void ClearAllChannel();

private:
    std::vector<CRadioChannel> m_vtChannel;
};

