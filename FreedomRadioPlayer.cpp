#include "pch.h"
#include "FreedomRadioPlayer.h"

DWORD WINAPI ThreadRadio(LPVOID lpParam);
std::vector<std::string> TokenizeMetadata(std::vector<std::uint8_t> vtData, std::uint8_t separator);


DWORD WINAPI ThreadRadio(LPVOID lpParam)
{
    CFreedomRadioPlayer* pPlayer = (CFreedomRadioPlayer*)lpParam;
	std::wstring wURL = pPlayer->GetRadioURL();
	std::string url;
	std::string strServerURL = "";
	std::string method = "GET";
	std::string arguments;
	http::InternetProtocol protocol = http::InternetProtocol::V4;
	const int FRAME_SIZE = 4096;

	int nMetadataInterval = -1;
	int nStoredDataSize = 0;

	pPlayer->TraceLog(_T("RadioThread Start!\n"));
	pPlayer->m_nThreadState = THREAD_STATE_RUNNING;
	url.assign(wURL.begin(), wURL.end());

    // Get stream server
    if (pPlayer->m_bIsPlayingRadio == FALSE)
    {
		pPlayer->TraceLog(_T("Radio Stop 1!\n"));
        goto THREAD_END;
    }

	try
	{
		http::Request request(url, protocol);
		const http::Response response = request.send(method, arguments, {
				"Accept: */*",
				"Icy-MetaData: 1",
				"Connection: close",
				"User-Agent: FreedomRadio/0.1",
			});

		if ((response.status != http::Response::Found) && (response.status != http::Response::Ok))
		{
			pPlayer->TraceLog(_T("Response status error! status:%d\n"), response.status);
			pPlayer->m_bIsRadioAvailable = FALSE;
			goto THREAD_END;
		}

		for each (std::string strHeader in response.headers)
		{
			if (pPlayer->m_bIsPlayingRadio == FALSE)
			{
				break;
			}
			std::size_t found = strHeader.find("Location:");
			if (found == std::string::npos)
			{
				continue;
			}

			strServerURL = strHeader.substr(10);
			break;
		}
	}
	catch (const std::exception& e)
	{
		TCHAR szError[1024];
		MultiByteToWideChar(CP_ACP, 0, e.what(), -1, szError, 1024);
		pPlayer->TraceLog(_T("Exception while get stream server! err:%s\n"), szError);
		pPlayer->m_bIsRadioAvailable = FALSE;
		goto THREAD_END;
	}
	// Request metadata
	if (pPlayer->m_bIsPlayingRadio == FALSE)
	{
		pPlayer->TraceLog(_T("Radio Stop 2!\n"));
		goto THREAD_END;
	}
	
	if (strServerURL == "")
	{
		pPlayer->TraceLog(_T("Empty server URL!\n"));
		pPlayer->m_bIsRadioAvailable = FALSE;
		goto THREAD_END;
	}
	
	try
	{
		http::Request requestMetadata(strServerURL, protocol);
		http::Socket socketMetadata = requestMetadata.send_and_get_socket(method, arguments, {
				"Accept: */*",
				"Icy-MetaData: 1",
				"Connection: close",
				"User-Agent: FreedomRadio/0.1",
			});
		if (pPlayer->m_bIsPlayingRadio == FALSE)
		{
			pPlayer->TraceLog(_T("Radio Stop 3!\n"));
			goto THREAD_END;
		}

		std::uint8_t tempBuffer[FRAME_SIZE];
		constexpr std::uint8_t crlf[] = { '\r', '\n' };
		std::vector<std::uint8_t> responseData;

		const auto sizeMetaData = socketMetadata.recv(tempBuffer, sizeof(tempBuffer), http::noSignal);
		if (pPlayer->m_bIsPlayingRadio == FALSE)
		{
			pPlayer->TraceLog(_T("Radio Stop 4!\n"));
			goto THREAD_END;
		}

		if (sizeMetaData == 0)
		{
			pPlayer->TraceLog(_T("Failed to get Metadata!\n"));
			goto THREAD_END;
		}

		responseData.insert(responseData.end(), tempBuffer, tempBuffer + sizeMetaData);

		// Check first line - 'ICY 200 OK'
		const auto i = std::search(responseData.begin(), responseData.end(), std::begin(crlf), std::end(crlf));

		// didn't find a newline
		if (i == responseData.end())
		{
			pPlayer->TraceLog(_T("Failed to get Metadata - Empty ICY 200 OK\n"));
			goto THREAD_END;
		}

		const std::string line(responseData.begin(), i);
		responseData.erase(responseData.begin(), i + 2);

		if (line.compare("ICY 200 OK") != 0)
		{
			pPlayer->TraceLog(_T("Failed to get Metadata - Not matching ICY 200 OK\n"));
			goto THREAD_END;
		}

		while (true)
		{
			if (pPlayer->m_bIsPlayingRadio == FALSE)
			{
				pPlayer->TraceLog(_T("Radio Stop 5!\n"));
				goto THREAD_END;
			}
			const auto i = std::search(responseData.begin(), responseData.end(), std::begin(crlf), std::end(crlf));

			// didn't find a newline
			if (i == responseData.end()) 
			{
				break;
			}

			const std::string line(responseData.begin(), i);
			responseData.erase(responseData.begin(), i + 2);

			// End of header
			if (line.length() == 0)
			{
				break;
			}

			const auto pos = line.find(':');

			if (pos != std::string::npos)
			{
				std::string headerName = line.substr(0, pos);
				std::string headerValue = line.substr(pos + 1);
				std::wstring wName, wValue;
				wValue.assign(headerValue.begin(), headerValue.end());

				if (headerName.compare("icy-name") == 0)
				{
					pPlayer->SetRadioSubName(wValue);
				}
				else if (headerName.compare("icy-genre") == 0)
				{
					pPlayer->SetRadioGenre(wValue);
				}
				else if (headerName.compare("icy-br") == 0)
				{
					pPlayer->SetRadioBitRate(std::stoi(headerValue));
				}
				else if (headerName.compare("icy-metaint") == 0)
				{
					nMetadataInterval = std::stoi(headerValue);
				}
				else if (headerName.compare("icy-pub") == 0)
				{
					// Nothing to do
					;
				}
				else if (headerName.compare("icy-url") == 0)
				{
					// Nothing to do
					;
				}
				else if (headerName.find("icy-notice") != std::string::npos)
				{
					std::wstring strNotice = pPlayer->GetRadioNotice();
					wName.assign(headerName.begin(), headerName.end());
					strNotice += wName.substr(10) + L". " + wValue + L"\r\n";
					pPlayer->SetRadioNotice(strNotice);
				}
			}
		}

		// Send (Post) message to refresh radio info
		::PostMessage(pPlayer->m_hParentHandle, UM_SEND_RADIO_SUB_INFO, 0, 0);

		// Check metadata interval
		if (pPlayer->m_bIsPlayingRadio == FALSE)
		{
			pPlayer->TraceLog(_T("Radio Stop 6!\n"));
			goto THREAD_END;
		}
		if (nMetadataInterval == -1)
		{
			pPlayer->TraceLog(_T("Failed to get Metadata interval\n"));
			pPlayer->m_bIsRadioAvailable = FALSE;
			goto THREAD_END;
		}

		// Remaining responseData is MP3 data
		nStoredDataSize = responseData.size();
		responseData.clear();

		int nRemainMetadata = 0;
		while (pPlayer->m_bIsPlayingRadio == TRUE)
		{
			ZeroMemory(tempBuffer, sizeof(tempBuffer));
			const auto sizeReceivedFrame = socketMetadata.recv(tempBuffer, sizeof(tempBuffer), http::noSignal);
			if (pPlayer->m_bIsPlayingRadio == FALSE)
			{
				pPlayer->TraceLog(_T("Radio Stop 7!\n"));
				goto THREAD_END;
			}

			if (sizeReceivedFrame == 0)
			{
				pPlayer->TraceLog(_T("Receiving frame end!\n"));
				break;
			}

			if (nRemainMetadata > 0)
			{
				responseData.insert(responseData.end(), tempBuffer, tempBuffer + nRemainMetadata);
				////// Call metadata tokenize
				std::vector<std::string> vtMetaData = TokenizeMetadata(responseData, ';');

				for (const std::string& strMetadata : vtMetaData)
				{
					if (pPlayer->m_bIsPlayingRadio == FALSE)
					{
						pPlayer->TraceLog(_T("Radio Stop 8!\n"));
						goto THREAD_END;
					}
					const auto iEqual = line.find('=');

					if (iEqual != std::string::npos)
					{
						std::string metadataName = line.substr(0, iEqual);
						std::string metadataValue = line.substr(iEqual + 1);
						std::wstring wValue;
						wValue.assign(metadataValue.begin(), metadataValue.end());

						if (metadataName.compare("StreamTitle") == 0)
						{
							if (pPlayer->IsNewSong(wValue) == true)
							{
								pPlayer->SetRadioNowPlaying(wValue);
								::PostMessage(pPlayer->m_hParentHandle, UM_SEND_RADIO_NOW_PLAYING, 0, 0);
								pPlayer->m_bIsPlayingRadio = FALSE;
								break;
							}
						}
						else if (metadataName.compare("StreamUrl") == 0)
						{
							// Nothing to do
							;
						}
					}
				}
			}

			if (pPlayer->m_bIsPlayingRadio == FALSE)
			{
				pPlayer->TraceLog(_T("Radio Stop 9!\n"));
				goto THREAD_END;
			}

			int iStartFrame = nRemainMetadata;
			int nSizeFrame = sizeReceivedFrame - nRemainMetadata;
			int nBufferSpace = nMetadataInterval - nStoredDataSize;
			if (nBufferSpace >= nSizeFrame)
			{
				nStoredDataSize += nSizeFrame;
				nRemainMetadata = 0;
			}
			else
			{
				// Get metadata size
				int nMetadataSize = static_cast<int>(tempBuffer[nBufferSpace]) * 16;

				// Insert metadata to responseData
				responseData.clear();
				responseData.insert(responseData.end(), tempBuffer + (nBufferSpace + 1), tempBuffer + (nBufferSpace + 1) + nMetadataSize);

				if (nMetadataSize > responseData.size())
				{
					nRemainMetadata = nMetadataSize - responseData.size();
					continue;
				}

				////// Call metadata tokenize
				std::vector<std::string> vtMetaData = TokenizeMetadata(responseData, ';');

				for (const std::string& strMetadata : vtMetaData)
				{
					if (pPlayer->m_bIsPlayingRadio == FALSE)
					{
						pPlayer->TraceLog(_T("Radio Stop 10!\n"));
						goto THREAD_END;
					}
					const auto iEqual = strMetadata.find('=');

					if (iEqual != std::string::npos)
					{
						std::string metadataName = strMetadata.substr(0, iEqual);
						std::string metadataValue = strMetadata.substr(iEqual + 1);

						if (metadataName.compare("StreamTitle") == 0)
						{
							std::wstring wValue;
							wValue.assign(metadataValue.begin(), metadataValue.end());
							if (pPlayer->IsNewSong(wValue) == true)
							{
								pPlayer->SetRadioNowPlaying(wValue);
								::PostMessage(pPlayer->m_hParentHandle, UM_SEND_RADIO_NOW_PLAYING, 0, 0);
								pPlayer->m_bIsPlayingRadio = FALSE;
								break;
							}
						}
						else if (metadataName.compare("StreamUrl") == 0)
						{
							// Nothing to do
							;
						}
					}
				}

				if (pPlayer->m_bIsPlayingRadio == FALSE)
				{
					pPlayer->TraceLog(_T("Radio Stop 11!\n"));
					break;
				}

				// Copy rest MP3 data
				nStoredDataSize = sizeReceivedFrame - nBufferSpace - 1 - nMetadataSize;
			}
		}
	}
	catch (const std::exception& e)
	{
		TCHAR szError[1024];
		MultiByteToWideChar(CP_ACP, 0, e.what(), -1, szError, 1024);
		pPlayer->TraceLog(_T("Exception while get Metadata! err:%s\n"), szError);
		pPlayer->m_bIsRadioAvailable = FALSE;
		goto THREAD_END;
	}

    // Thread end
THREAD_END:
    pPlayer->m_bIsPlayingRadio = FALSE;
	pPlayer->m_nThreadState = THREAD_STATE_END;
    pPlayer->TraceLog(_T("RadioThread End!\n"));
    return 0;
}


std::vector<std::string> TokenizeMetadata(std::vector<std::uint8_t> vtData, std::uint8_t separator)
{
	std::vector<std::string> vtMetadata;

	while (true)
	{
		// Separate metadata
		const auto iMetadata = std::find(vtData.begin(), vtData.end(), separator);

		// didn't find a metadata
		if (iMetadata == vtData.end())
		{
			break;
		}

		const std::string line(vtData.begin(), iMetadata);
		vtData.erase(vtData.begin(), iMetadata + 1);
		vtMetadata.push_back(line);
	}

	return vtMetadata;
}


CFreedomRadioPlayer::CFreedomRadioPlayer()
{
    m_strName = L"";
    m_strURL = L"";
    m_hRadioThread = NULL;
	m_nThreadState = THREAD_STATE_END;
	m_bIsRadioAvailable = TRUE;
    InitRadioInfo();
}


CFreedomRadioPlayer::~CFreedomRadioPlayer()
{
    StopRadio();
}


void CFreedomRadioPlayer::InitRadioInfo()
{
    m_strSubName = L"";
    m_strGenre = L"";
    m_nBitRate = 0;
    m_strNotice = L"";
    m_strNowPlaying = L"";
    m_bIsPlayingRadio = FALSE;
}


void CFreedomRadioPlayer::PlayRadio()
{
    StopRadio();

    InitRadioInfo();

    DWORD dwThreadID = 0;
	if (m_bIsPlayingRadio == FALSE)
	{
		m_bIsPlayingRadio = TRUE;
		m_hRadioThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadRadio, this, NULL, &dwThreadID);
	}
}


void CFreedomRadioPlayer::StopRadio()
{
	if (m_bIsPlayingRadio == TRUE)
	{
		m_bIsPlayingRadio = FALSE;
		TraceLog(_T("CFreedomRadioPlayer::StopRadio Thread flag OFF!\n"));
		if (m_nThreadState != THREAD_STATE_END)
		{
			DWORD dwResult = WaitForSingleObject(m_hRadioThread, 5000);
			/*if (dwResult != WAIT_OBJECT_0)
			{
				TRACE(_T("CFreedomRadioPlayer::StopRadio WaitForSingleObject result %u\n"), dwResult);
			}*/
			CString csName(m_strName.c_str());
			TraceLog(_T("CFreedomRadioPlayer::StopRadio WaitForSingleObject result %u\n"), dwResult);
		}
	}
}


BOOL CFreedomRadioPlayer::IsNewSong(std::wstring strSongTitle)
{
	return (m_strNowPlaying.compare(strSongTitle) != 0);
}


void CFreedomRadioPlayer::TraceLog(const TCHAR* szLog, ...)
{
	SYSTEMTIME currentTime;
	CString strTime = _T("");
	CString csName(m_strName.c_str());
	va_list ap;
	TCHAR buf[1024];

	GetLocalTime(&currentTime);
	strTime.Format(_T("%04u-%02u-%02u_%02u:%02u:%02u.%03u"), currentTime.wYear, currentTime.wMonth, currentTime.wDay,
		currentTime.wHour, currentTime.wMinute, currentTime.wSecond, currentTime.wMilliseconds);
	va_start(ap, szLog);
	_vstprintf_s(buf, szLog, ap);
	va_end(ap);
	TRACE(_T("[%s] <%s> %s"), strTime, csName, buf);
}
