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


void CChannelManager::GetChannelInfo(int iChannel, std::wstring& strName, std::wstring& strURL)
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