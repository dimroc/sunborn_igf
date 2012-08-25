#pragma once

#include "RakNet/LightweightDatabaseServer.h"
#include "RakNet/LightweightDatabaseClient.h"
#include "RakNet/TableSerializer.h"
#include "RakNet/BitStream.h"
#include "RakNet/StringCompressor.h"
#include "RakNet/DS_Table.h"
#include "RakNet/LightweightDatabaseClient.h"
#include "RakNet/LightweightDatabaseServer.h"
#include "RakNet/RakPeerInterface.h"
#include "RakNet/RakClientInterface.h"
#include "RakNet/RakServerInterface.h"
#include "RakNet/RakNetworkFactory.h"
#include "RakNet/RakSleep.h"
#include "RakNet/NetworkTypes.h"
#include "RakNet/GetTime.h"
#include "RakNet/PacketEnumerations.h"

#include "ServerInfo.h"

#ifdef WIN32
#define SLEEP(arg)  ( Sleep( (arg) ) )
#else
#define SLEEP(arg)  ( usleep( (arg) *1000 ) )
#endif

using std::set;

//! Queries the Master Server to locate server IPs and locates LAN servers through broadcast.
class CServerFinder
{
private:
    typedef DataStructures::Table::Row* PtrDatabaseRow;
    VOID PopulateServerInfo(SServerInfo *pOut, UINT nKey, PtrDatabaseRow row); 
public:
    VOID QueryMasterServer(ServerInfoSet &sServerListOut, SMasterServerInfo &masterInfo);
    VOID QueryLanServers(ServerInfoSet &sServerListOut, USHORT nClientPort, USHORT nServerPort);
};