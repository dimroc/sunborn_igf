#include "dxstdafx.h"
#include "NetworkRoot.h"

#include "networkdefine.h"

using std::ifstream;

CNetworkRoot::CNetworkRoot() : 
    m_bNextServer(false), m_bPopDeqFront(false), m_bScannerConnected(false), m_bLANwasLastRefresh(false)
{
    m_LocalHostInfo.strName.assign("Local Host");
    m_LocalHostInfo.strIP.assign("127.0.0.1");
    m_LocalHostInfo.nPort = SERVER_PORT;    

    m_pRakScanner = RakNetworkFactory::GetRakClientInterface(); 
}

CNetworkRoot::~CNetworkRoot()
{
    if(m_pRakScanner)
        RakNetworkFactory::DestroyRakClientInterface(m_pRakScanner);
}

VOID CNetworkRoot::RefreshServerList()
{
    m_sServers.clear();
    for(size_t i=0;i<m_vMasterServers.size();i++)
        m_ServerFinder.QueryMasterServer(m_sServers, m_vMasterServers[i]);

    StartServerScan(m_sServers);    // find map / players/maxplayers/ping.

    m_bLANwasLastRefresh = false;
}

VOID CNetworkRoot::RefreshLANList()
{
    m_sLANServers.clear();
    m_ServerFinder.QueryLanServers(m_sLANServers, INFO_REQUEST_PORT, INFO_SERVER_PORT);    
    StartServerScan(m_sLANServers);

    m_bLANwasLastRefresh = true;
}

VOID CNetworkRoot::AddMasterServer(const string &strIP, USHORT nPort)
{    
    SMasterServerInfo masterInfo;
    masterInfo.strIP.assign(strIP);
    masterInfo.nPort = nPort;
    masterInfo.strTableName = MASTER_TABLE_NAME;
    masterInfo.strTablePassword = MASTER_TABLE_PASSWORD;

    masterInfo.nPwdLength = (int)strlen(MASTER_SERVER_PASSWORD);
    assert(masterInfo.nPwdLength < 256);
    strncpy_s(masterInfo.pszPwdData, 256, MASTER_SERVER_PASSWORD, 255);

    m_vMasterServers.push_back(masterInfo);
}

//! Processes all packets used for scanning (info packets) and master server queries.
VOID CNetworkRoot::ProcessPackets()
{
    if(m_bNextServer)
    {   // scan next server
        m_pRakScanner->Disconnect(100);     

        if(m_bPopDeqFront && !m_dServersToScan.empty())
            m_dServersToScan.pop_front();

        if(!m_dServersToScan.empty())
        {            
            ServerInfoIteratorDeq::reference ref = m_dServersToScan.front();            

            // initialize this server info ref to KNOWN rubbish so we can tell if we never got info.
            ref->nPing = -1;
            ref->nMaxPlayers = 0;
            ref->nPlayers = 0;

            dprintf("[RakScanner-ProcessPackets] Scanning: %s %d\n", ref->strIP.c_str(), INFO_SERVER_PORT);
            m_bScannerConnected = m_pRakScanner->Connect(ref->strIP.c_str(), INFO_SERVER_PORT, INFO_REQUEST_PORT, 0, 20);
            if(m_bScannerConnected)
                m_bNextServer = m_bPopDeqFront = false;
            else
                m_bNextServer = m_bPopDeqFront = true;
        }    
        else
            m_bNextServer = m_bPopDeqFront = m_bScannerConnected = false;
    }

    if(m_bScannerConnected && !m_bNextServer)
    {
        ServerInfoIteratorDeq::reference ref = m_dServersToScan.front();
        Packet* p = m_pRakScanner->Receive();
        while(p)
        {   // handle packet
            m_bNextServer = m_bPopDeqFront = true;            
            switch(p->data[0])
            {
            case ID_CONNECTION_ATTEMPT_FAILED:
                dprintf("[ScanServers]Connection attempt failed!\n");
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                dprintf("[ScanServers]Server is full\n");
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                dprintf("[ScanServers]Disconnected from: %s", p->playerId.ToString(true));
                break;
            case ID_CONNECTION_LOST:
                dprintf("[ScanServers]Connection lost from: %s", p->playerId.ToString(true));
                break;
            case ID_USER_PACKET_ENUM: 
                {
                dprintf("[ScanServers]Received Info packet from: %s\n", p->playerId.ToString(true));
                RakNet::BitStream bitStream(p->data, p->length, false);
                bitStream.IgnoreBits(8); // ignore first identifier byte.
                BitStreamToServerInfo(&(*ref), &bitStream);
                ref->nPing = m_pRakScanner->GetLastPing();                
                }
                break;
            case ID_CONNECTION_REQUEST_ACCEPTED:
                dprintf("[ScanServers]ID_CONNECTION_REQUEST_ACCEPTED\n");                
                m_bNextServer = m_bPopDeqFront = false;
                break;
            default:
                dprintf("[ScanServers]Message identifier: %d from %s\n", (int)p->data[0], p->playerId.ToString(true));
                m_bNextServer = m_bPopDeqFront = false;
                break;
            }
            m_pRakScanner->DeallocatePacket(p);
            p = m_pRakScanner->Receive();
        } 
    }
}

// ----------------------------------------------------
// PRIVATE SHIZNIT
// ----------------------------------------------------

//! asks how many players / max players / map / on server.
VOID CNetworkRoot::StartServerScan(ServerInfoSet &sServerListOut) 
{
    PRINT_ENTER_FUNCTION_DEBUG;
    m_pRakScanner->Disconnect(100);
    m_dServersToScan.clear();

    if(sServerListOut.size() <= 0)
    {
        m_bNextServer = false;  
        m_bPopDeqFront = false;
    }
    else
    {
        ServerInfoSet::iterator it = sServerListOut.begin();
        for( ; it != sServerListOut.end(); it++)
            m_dServersToScan.push_back(it);

        m_bNextServer = true;  
        m_bPopDeqFront = false;
    }
}