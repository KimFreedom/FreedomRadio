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
    CRadioChannel *GetChannelInfo(int iChannel);
    void SetNowPlaying(int iChannel, std::wstring strArtist, std::wstring strTitle);
    void ClearAllChannel();

private:
    std::vector<CRadioChannel> m_vtChannel;
};

