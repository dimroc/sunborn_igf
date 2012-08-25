#pragma once

#include "RakNet/BitStream.h"

using std::string;
using std::set;

#define             NUM_BYTES_FOR_NAMES             32


struct SServerInfo
{
    string strIP;
    unsigned short nPort;

    string strName, strMap;
    
    int nPing;
    unsigned short nPlayers, nMaxPlayers;

    UINT nVersion;

    SServerInfo(void);
    ~SServerInfo(void);    
};

class CServerInfoComparator
{
public:
    enum { SORT_IP = 0, SORT_NAME, SORT_PING };

    const UCHAR SortFlag;

    bool operator() (const SServerInfo &lhs, const SServerInfo &rhs) const;
    CServerInfoComparator(UCHAR SortFlag = SORT_IP);
private:    // operators intentionally undefined so as to generate compiler warnings if ever called
    CServerInfoComparator& operator=(const CServerInfoComparator &rhs); // disallow assignment operator
};

typedef set<SServerInfo, CServerInfoComparator> ServerInfoSet;
typedef set<SServerInfo, CServerInfoComparator> ServerInfoMultiSet;


struct SMasterServerInfo
{
    string strTableName, strTablePassword;
    string strIP;
    
    unsigned short nPort;

    char pszPwdData[256];
    int nPwdLength;

    SMasterServerInfo();
    ~SMasterServerInfo();

};

int GetPortFromStringIP(const string &str);
VOID AppendPortToString(string &str, USHORT nPort);

VOID ServerInfoToBitStream(SServerInfo *pServerInfo, RakNet::BitStream* pBitStream);
VOID BitStreamToServerInfo(SServerInfo *pServerInfo, RakNet::BitStream* pBitStream);

//! At least 100 chars are allocated.
VOID ServerInfoToLPSTR(const SServerInfo *pServerInfo, LPSTR pszOut);
