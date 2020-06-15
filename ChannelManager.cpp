#include "pch.h"
#include "ChannelManager.h"

void CChannelManager::AddChannel(std::string strName, std::string strURL)
{
    CRadioChannel objChannel(strName, strURL);
    m_vtChannel.push_back(objChannel);
}


int CChannelManager::CountChannelInfo()
{
    return static_cast<int>(m_vtChannel.size());
}


void CChannelManager::GetChannelInfo(int iChannel, std::string& strName, std::string& strURL)
{
    if ((iChannel < 0) || (iChannel >= CountChannelInfo()))
    {
        return;
    }
    strName = m_vtChannel[iChannel].GetName();
    strURL  = m_vtChannel[iChannel].GetURL();
}


void CChannelManager::ClearAllChannel()
{
    m_vtChannel.clear();
}