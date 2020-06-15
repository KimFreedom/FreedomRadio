#include "pch.h"
#include "FreedomRadioPlayer.h"

DWORD WINAPI ThreadRadio(LPVOID lpParam);
std::vector<std::string> TokenizeMetadata(std::vector<std::uint8_t> vtData, std::uint8_t separator);


DWORD WINAPI ThreadRadio(LPVOID lpParam)
{
    TRACE("RadioThread Start!\n");
    CFreedomRadioPlayer* pPlayer = (CFreedomRadioPlayer*)lpParam;
	std::string url = pPlayer->GetRadioURL();
	std::string strServerURL = "";
	std::string method = "GET";
	std::string arguments;
	http::InternetProtocol protocol = http::InternetProtocol::V4;
	const int FRAME_SIZE = 4096;

	int nMetadataInterval = -1;
	int nStoredDataSize = 0;
	std::uint8_t* pMP3Buf = NULL;

    // Get stream server
    if (pPlayer->m_bIsPlayingRadio == false)
    {
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

		if (response.status != http::Response::Found)
		{
			TRACE(_T("Failed to get stream server!\n"));
			goto THREAD_END;
		}

		for each (std::string strHeader in response.headers)
		{
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
		TRACE(_T("Exception while get stream server! err:\n"), e);
		goto THREAD_END;
	}

	if (strServerURL == "")
	{
		TRACE(_T("Failed to get stream server Location!\n"));
		goto THREAD_END;
	}

	// Request metadata
	if (pPlayer->m_bIsPlayingRadio == false)
	{
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

		std::uint8_t tempBuffer[FRAME_SIZE];
		constexpr std::uint8_t crlf[] = { '\r', '\n' };
		std::vector<std::uint8_t> responseData;

		const auto sizeMetaData = socketMetadata.recv(tempBuffer, sizeof(tempBuffer), http::noSignal);

		if (sizeMetaData == 0)
		{
			TRACE("Failed to get Metadata!\n");
			goto THREAD_END;
		}

		responseData.insert(responseData.end(), tempBuffer, tempBuffer + sizeMetaData);

		// Check first line - 'ICY 200 OK'
		const auto i = std::search(responseData.begin(), responseData.end(), std::begin(crlf), std::end(crlf));

		// didn't find a newline
		if (i == responseData.end())
		{
			TRACE("Failed to get Metadata - Empty ICY 200 OK\n");
			goto THREAD_END;
		}

		const std::string line(responseData.begin(), i);
		responseData.erase(responseData.begin(), i + 2);

		if (line.compare("ICY 200 OK") != 0)
		{
			TRACE("Failed to get Metadata - Not matching ICY 200 OK\n");
			goto THREAD_END;
		}

		while (true)
		{
			if (pPlayer->m_bIsPlayingRadio == false)
			{
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

				if (headerName.compare("icy-name") == 0)
				{
					pPlayer->SetRadioSubName(headerValue);
				}
				else if (headerName.compare("icy-genre") == 0)
				{
					pPlayer->SetRadioGenre(headerValue);
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
					std::string strNotice = pPlayer->GetRadioNotice();
					strNotice += headerName.substr(10) + ". " + headerValue + "\r\n";
					pPlayer->SetRadioNotice(strNotice);
				}
			}
		}

		// Send (Post) message to refresh radio info
		::PostMessage(pPlayer->m_hParentHandle, UM_SEND_RADIO_SUB_INFO, 0, 0);

		// Check metadata interval
		if (pPlayer->m_bIsPlayingRadio == false)
		{
			goto THREAD_END;
		}
		if (nMetadataInterval == -1)
		{
			TRACE("Failed to get Metadata interval\n");
			goto THREAD_END;
		}

		// Remaining responseData is MP3 data
		pMP3Buf = new std::uint8_t[nMetadataInterval];
		ZeroMemory(pMP3Buf, sizeof(std::uint8_t) * nMetadataInterval);
		nStoredDataSize = responseData.size();
		if (nStoredDataSize > 0)
		{
			std::copy(responseData.begin(), responseData.end(), pMP3Buf);
		}
		responseData.clear();

		int nRemainMetadata = 0;
		while (pPlayer->m_bIsPlayingRadio == true)
		{
			ZeroMemory(tempBuffer, sizeof(tempBuffer));
			const auto sizeReceivedFrame = socketMetadata.recv(tempBuffer, sizeof(tempBuffer), http::noSignal);

			if (sizeReceivedFrame == 0)
			{
				TRACE("Receiving frame end!\n");
				break;
			}

			if (nRemainMetadata > 0)
			{
				responseData.insert(responseData.end(), tempBuffer, tempBuffer + nRemainMetadata);
				////// Call metadata tokenize
				std::vector<std::string> vtMetaData = TokenizeMetadata(responseData, ';');

				for (const std::string& strMetadata : vtMetaData)
				{
					const auto iEqual = line.find('=');

					if (iEqual != std::string::npos)
					{
						std::string metadataName = line.substr(0, iEqual);
						std::string metadataValue = line.substr(iEqual + 1);

						if (metadataName.compare("StreamTitle") == 0)
						{
							if (pPlayer->IsNewSong(metadataValue) == true)
							{
								pPlayer->SetRadioNowPlaying(metadataValue);
								::PostMessage(pPlayer->m_hParentHandle, UM_SEND_RADIO_NOW_PLAYING, 0, 0);
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

			int iStartFrame = nRemainMetadata;
			int nSizeFrame = sizeReceivedFrame - nRemainMetadata;
			int nBufferSpace = nMetadataInterval - nStoredDataSize;
			if (nBufferSpace >= nSizeFrame)
			{
				memcpy_s(&pMP3Buf[nStoredDataSize], nSizeFrame, tempBuffer + iStartFrame, nSizeFrame);
				nStoredDataSize += nSizeFrame;
				nRemainMetadata = 0;
			}
			else
			{
				if (nBufferSpace > 0)
				{
					memcpy_s(&pMP3Buf[nStoredDataSize], nBufferSpace, tempBuffer, nBufferSpace);
				}

				// TODO - Send MP3 data
				;
				
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
					const auto iEqual = strMetadata.find('=');

					if (iEqual != std::string::npos)
					{
						std::string metadataName = strMetadata.substr(0, iEqual);
						std::string metadataValue = strMetadata.substr(iEqual + 1);

						if (metadataName.compare("StreamTitle") == 0)
						{
							if (pPlayer->IsNewSong(metadataValue) == true)
							{
								pPlayer->SetRadioNowPlaying(metadataValue);
								::PostMessage(pPlayer->m_hParentHandle, UM_SEND_RADIO_NOW_PLAYING, 0, 0);
							}
						}
						else if (metadataName.compare("StreamUrl") == 0)
						{
							// Nothing to do
							;
						}
					}
				}

				// Clear MP3Buffer
				ZeroMemory(pMP3Buf, sizeof(std::uint8_t)* nMetadataInterval);

				// Copy rest MP3 data
				nStoredDataSize = sizeReceivedFrame - nBufferSpace - 1 - nMetadataSize;
				memcpy_s(pMP3Buf, nStoredDataSize, tempBuffer + nBufferSpace + 1 + nMetadataSize, nStoredDataSize);
			}
		}
	}
	catch (const std::exception& e)
	{
		TRACE(_T("Exception while get Metadata! err:\n"), e);
		goto THREAD_END;
	}

    // Thread end
THREAD_END:
	if (pMP3Buf != NULL)
	{
		delete[] pMP3Buf;
	}
    pPlayer->m_bIsPlayingRadio = NULL;
    CloseHandle(pPlayer->m_hRadioThread);
    pPlayer->m_hRadioThread = NULL;
    TRACE("RadioThread End!\n");
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
    m_strName = "";
    m_strURL = "";
    m_hRadioThread = NULL;
    InitRadioInfo();
}


CFreedomRadioPlayer::~CFreedomRadioPlayer()
{
    StopRadio();
}


void CFreedomRadioPlayer::InitRadioInfo()
{
    m_strSubName = "";
    m_strGenre = "";
    m_nBitRate = 0;
    m_strNotice = "";
    m_strNowPlaying = "";
    m_bIsPlayingRadio = false;
}


void CFreedomRadioPlayer::PlayRadio()
{
    StopRadio();

    InitRadioInfo();

    DWORD dwThreadID = 0;
    m_bIsPlayingRadio = true;
    m_hRadioThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadRadio, this, NULL, &dwThreadID);
}


void CFreedomRadioPlayer::StopRadio()
{
    m_bIsPlayingRadio = false;
    WaitForSingleObject(m_hRadioThread, 5000);
}


bool CFreedomRadioPlayer::IsNewSong(std::string strSongTitle)
{
	return (m_strNowPlaying.compare(strSongTitle) != 0);
}
