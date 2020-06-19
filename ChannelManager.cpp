#include "pch.h"
#include "ChannelManager.h"

void CChannelManager::AddChannel(std::wstring strName, std::wstring strURL)
{
    CRadioChannel objChannel(strName, strURL);
    m_vtChannel.push_back(objChannel);
}


int CChannelManager::CountChannelInfo()
{
    return static_cast<int>(m_vtChannel.size());
}


CRadioChannel *CChannelManager::GetChannelInfo(int iChannel)
{
    if ((iChannel < 0) || (iChannel >= CountChannelInfo()))
    {
        return NULL;
    }

    return &m_vtChannel[iChannel];
}


void CChannelManager::SetNowPlaying(int iChannel, std::wstring strArtist, std::wstring strTitle)
{
    if ((iChannel < 0) || (iChannel >= CountChannelInfo()))
    {
        return;
    }

    m_vtChannel[iChannel].SetNowPlayingArtist(strArtist);
    m_vtChannel[iChannel].SetNowPlayingTitle(strTitle);
}


void CChannelManager::ClearAllChannel()
{
    m_vtChannel.clear();
}