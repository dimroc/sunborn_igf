#pragma once

#include "RakNet/PacketEnumerations.h"
#include "RakNet/RakNetworkFactory.h"
#include "RakNet/RakServerInterface.h"
#include "RakNet/RakClientInterface.h"
#include "RakNet/NetworkTypes.h"
#include "RakNet/BitStream.h"

#include "ServerInfo.h"
#include "ServerFinder.h"
#include "Server.h"

using std::set;
using std::string;
using std::vector;
using std::deque;

class CNetworkRoot
{
private:
    //! interface for client to query master servers and discover LAN servers.
    CServerFinder                                   m_ServerFinder; 
    RakClientInterface*                             m_pRakScanner;  //! scans servers

    //! unique set of servers sorted by ip and port
    ServerInfoSet                                   m_sServers, m_sLANServers; 
    typedef deque<ServerInfoSet::iterator>          ServerInfoIteratorDeq;
    ServerInfoIteratorDeq                           m_dServersToScan;

    vector<SMasterServerInfo>                       m_vMasterServers;   //! vector of master servers available to query    
    SServerInfo                                     m_LocalHostInfo;

    bool                                            m_bNextServer, m_bPopDeqFront, m_bScannerConnected;
    bool                                            m_bLANwasLastRefresh;

private:    
    VOID StartServerScan(ServerInfoSet &sServerListOut);    //! asks how many players / max players / map / on server.

public:
    //! loads up the network subsystem and sets the ips of master servers to query.
    CNetworkRoot();
    ~CNetworkRoot(void);

    VOID AddMasterServer(const string &strIP, USHORT nPort);
    VOID ClearMasterServers()                           { m_vMasterServers.clear(); }
    const vector<SMasterServerInfo> &GetMasterServers() { return m_vMasterServers; }

    //! Returns the unique list of servers that are sorted by IP then port.
    const ServerInfoSet& GetServerList()                { if(m_bLANwasLastRefresh) return m_sLANServers; return m_sServers;}
    VOID RefreshServerList();   //! Connects to the master servers and broadcasts to LAN and updates server list.
    VOID RefreshLANList();

    VOID ProcessPackets();  //! Processes all the packets used for server scanning, etc.
};
