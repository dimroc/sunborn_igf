#include "dxstdafx.h"
#include "ServerInfo.h"

// ----------------------------------------------------------------------------
// Game Host Server Info
// ----------------------------------------------------------------------------

SServerInfo::SServerInfo(void)
{
}

SServerInfo::~SServerInfo(void)
{
}

// ----------------------------------------------------------------------------
// Game Host Server Sorting Utility Class for STL
// ----------------------------------------------------------------------------

CServerInfoComparator::CServerInfoComparator(UCHAR _SortFlag) : 
    SortFlag(_SortFlag)
{
}

bool CServerInfoComparator::operator() (const SServerInfo &lhs, const SServerInfo &rhs) const
{
    if(SortFlag == SORT_IP)
    {
        if (lhs.strIP.compare(rhs.strIP) == 0)
            return lhs.nPort < rhs.nPort;
        else
            return lhs.strIP.compare(rhs.strIP) < 0;
    }
    else if(SortFlag == SORT_NAME)
        return lhs.strName.compare(rhs.strName) < 0;                
    else // by ping
        return lhs.nPing < rhs.nPing;
}

// ----------------------------------------------------------------------------
// Master Server Info
// ----------------------------------------------------------------------------

SMasterServerInfo::SMasterServerInfo(void)
{    
}

SMasterServerInfo::~SMasterServerInfo(void)
{
}


int GetPortFromStringIP(const string &str)
{
    size_t pos = str.find(':');
    if(++pos < str.size())
    {
        string strTemp = str.substr(pos, str.size());
        int port = atoi(strTemp.c_str());
        return port;
    }
    assert(false && "WTF THIS IS IMPOSSIBLE, STRING SHOULD ALWAYS BE SUFFIXED BY PORT");
    return 0;
}

VOID AppendPortToString(string &str, USHORT nPort)
{
    assert( nPort < 1000000 );
    char buf[7];
    sprintf_s(buf, 7, "%d", nPort);
    str.append(":");
    str.append(buf);
}


VOID ServerInfoToBitStream(SServerInfo *pServerInfo, RakNet::BitStream* pBitStream)
{   
    assert(pServerInfo && pBitStream);    
    pBitStream->Write(pServerInfo->nPlayers);
    pBitStream->Write(pServerInfo->nMaxPlayers);

    char buf[NUM_BYTES_FOR_NAMES];
    strncpy_s(buf, NUM_BYTES_FOR_NAMES, pServerInfo->strMap.c_str(), NUM_BYTES_FOR_NAMES);
    pBitStream->Write(buf, NUM_BYTES_FOR_NAMES);

    strncpy_s(buf, NUM_BYTES_FOR_NAMES, pServerInfo->strName.c_str(), NUM_BYTES_FOR_NAMES);
    pBitStream->Write(buf, NUM_BYTES_FOR_NAMES);

    pBitStream->Write(pServerInfo->nVersion);
}

VOID BitStreamToServerInfo(SServerInfo *pServerInfo, RakNet::BitStream* pBitStream)
{
    assert(pServerInfo && pBitStream);

    char buf[NUM_BYTES_FOR_NAMES];
    pBitStream->Read(pServerInfo->nPlayers);
    pBitStream->Read(pServerInfo->nMaxPlayers);

    pBitStream->Read(buf, NUM_BYTES_FOR_NAMES);
    buf[NUM_BYTES_FOR_NAMES-1] = NULL;
    pServerInfo->strMap.assign(buf);
    pBitStream->Read(buf, NUM_BYTES_FOR_NAMES);
    buf[NUM_BYTES_FOR_NAMES-1] = NULL;
    pServerInfo->strName.assign(buf);

    pBitStream->Read(pServerInfo->nVersion);
}
