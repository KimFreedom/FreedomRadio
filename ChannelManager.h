#pragma once
#include "RadioChannel.h"
#include <string>
#include <vector>

class CChannelManager
{
    CChannelManager() {}
    virtual ~CChannelManager() {}

public:
    void AddChannel(std::string strName, std::string strURL);
    int  CountChannelInfo();
    void GetChannelInfo(int iChannel, std::string& strName, std::string& strURL);
    void ClearAllChannel();

private:
    std::vector<CRadioChannel> m_vtChannel;
};

