#include "dxstdafx.h"
#include "Server.h"
#include "NetworkDefine.h"

#include "RakNet/RakPeerInterface.h"
#include "RakNet/RakNetworkFactory.h"
#include "RakNet/RakSleep.h"

#define         DEFAULT_BROADCAST_INTERVAL          1.f // 1 second

CServer::CServer(NetworkID networkID) :
    m_bInit(false),
    m_fTimeSinceBroadcast(0.f),
    m_fBroadcastInterval(DEFAULT_BROADCAST_INTERVAL)
{
    m_pRakServer = RakNetworkFactory::GetRakServerInterface();        
    m_pRakInfo = RakNetworkFactory::GetRakServerInterface();
}

CServer::~CServer(void)
{
    Disconnect();
    // try to erase self from master server position
#if 0   // i dont know my row id to remove from master server
    for(size_t i = 0; i<m_vMasterServers.size(); i++)
        RemoveFromMasters(MASTER_REGISTER_PORT, m_vMasterServers[i]);
#endif

    if(m_pRakServer)
        RakNetworkFactory::DestroyRakServerInterface(m_pRakServer);
    if(m_pRakInfo)
        RakNetworkFactory::DestroyRakServerInterface(m_pRakInfo);
    m_pRakServer = NULL;
}

VOID CServer::Disconnect()
{
    if(!m_bInit)    return ;
    for(USHORT i=0;i<m_pRakServer->GetConnectedPlayers();i++)
    {   // kick all players before disconnect
        PlayerID pid = m_pRakServer->GetPlayerIDFromIndex(i);
        m_pRakServer->Kick(pid);
    }   
    m_pRakServer->Disconnect(100,0);
    m_pRakInfo->Disconnect(100);
    m_bInit = false;
}

bool CServer::StartServer(SServerInfo& serverInfo, const vector<SMasterServerInfo> &vMasterServers)
{   
    Disconnect();
    
    m_ServerInfo = serverInfo;
    m_ServerInfo.nPlayers = 0;
    m_vMasterServers.clear();
    m_vMasterServers.assign(vMasterServers.begin(), vMasterServers.end());

    m_bInit = m_pRakServer->Start(serverInfo.nMaxPlayers, 0, 30, serverInfo.nPort);    
    if(m_bInit) dprintf("SERVER STARTED\n");
    else return m_bInit;

    m_bInit = StartInfoServer();
    if(!m_bInit)
    {
        dprintf("[StartServer]InfoServer Failed! on port: %d\n", INFO_SERVER_PORT);
        Disconnect();
        return m_bInit;
    }

    //
    // Register Server's existence with Master Server
    //
    const int nCELLS = 4;
    DatabaseCellUpdate cellUpdates[nCELLS]; // columns of info

    // populate cells with this server's name        
    StuffCell(&cellUpdates[0], "NAME", serverInfo.strName);        
    StuffCell(&cellUpdates[1], "PORT", serverInfo.nPort);        
    StuffCell(&cellUpdates[2], "MAXPLAYERS", serverInfo.nMaxPlayers);
    StuffCell(&cellUpdates[3], "VERSION", SUNBORN_VERSION);
    serverInfo.nVersion = SUNBORN_VERSION;

    // Register server with Master Server
    for(size_t i=0; i<vMasterServers.size(); i++)
        RegisterServerWithMaster(serverInfo.nPort, cellUpdates, nCELLS, vMasterServers[i]);

    return m_bInit;
}

//! Register functions to be called by clients on _SERVER_
VOID CServer::RegisterRPCFunction(RakNetFuncPtr pFunc)
{
#if 0
    union {
        void (__cdecl CGameEngine::*cFunc)( RPCParameters *rpcParms ); 
        void* voidFunc;
    }; 
    cFunc = pMemberFunc; 
    m_pRakServer->RegisterClassMemberRPC(pszFuncName, voidFunc);
#endif
    
    REGISTER_STATIC_RPC(m_pRakServer, pFunc);
}

//! abbreviated ServerRPCEx call with defaults loaded(broadcast,  priority, etc.) to call RPCs on _CLIENT_ machines
bool CServer::RPC(LPSTR pszFunc, RakNet::BitStream *pBitStream)
{   // broadcast client rpcs
    return m_pRakServer->RPC(pszFunc, pBitStream, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true, true, UNASSIGNED_NETWORK_ID, 0);    
}

//! NOTE: NOT IMPLEMENTED. Calls RPC on a(or all) _CLIENT_ machines with the passed in bitStream.
bool CServer::RPC(LPSTR pszFunc, 
                        RakNet::BitStream *pBitStream, 
                        PacketPriority priority, 
                        PacketReliability reliability, 
                        char orderingChannel, 
                        PlayerID playerId, 
                        bool broadcast, 
                        bool shiftTimestamp, 
                        NetworkID networkID, 
                        RakNet::BitStream *replyFromTarget )
{
    return m_pRakServer->RPC(pszFunc, pBitStream, priority, reliability, orderingChannel, playerId, broadcast, shiftTimestamp, networkID, replyFromTarget);
}


bool CServer::IsTimeForRealityBroadcast(float fElapsedTime)
{
    m_fTimeSinceBroadcast += fElapsedTime;

    if(m_fTimeSinceBroadcast >= m_fBroadcastInterval)
    {

        m_fTimeSinceBroadcast = 0.f;
        return true;
    }
    return false;
}


VOID CServer::ProcessPackets()                                   
{ 
    if(m_bInit) 
    {
        m_ServerInfo.nPlayers = m_pRakServer->GetConnectedPlayers();    // update serverinfo 
        Multiplayer::ProcessPackets(m_pRakServer); 

        // Have pRakInfo check on info requests and reply with nPlayers, nMaxPlayers, current map, nVersion.
        ProcessInfoPackets(m_pRakInfo);        
    }
}

void CServer::ReceiveConnectionLost( Packet *packet, RakServerInterface *rakServer )
{   // announce to all players that player bounced
	RakNet::BitStream bitStream;
	bitStream.Write((char*)&packet->playerId, sizeof(PlayerID));

    // TODO: Delete character from m_hmServerPlayers in CGameEngine.

    rakServer->RPC("ClientPlayerDisconnectAnnouncement", &bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 1, packet->playerId, false, true, UNASSIGNED_NETWORK_ID, 0);
}

void CServer::ProcessUnhandledPacket( Packet *packet, unsigned char packetIdentifier, RakServerInterface *rakServer )
{
    // wtf i dunno this packet
    // panic and kick!
    dprintf("UNHANDLED PACKET: %d from %s with data:\n%s\n", packetIdentifier, packet->playerId.ToString(true), packet->data);
    dprintf("I'm kicking the bitch cause he doesn't send packets sunborn style!\n");
    rakServer->Kick(packet->playerId);
}

// --------------------------------------------------------
// PRIVATE SHIZNITS
// --------------------------------------------------------

VOID CServer::ProcessInfoPackets(RakServerInterface* server)
{    
    Packet* p = server->Receive();
    while (p)
    {        
        if (p->data[0]==ID_DISCONNECTION_NOTIFICATION)
            dprintf("[InfoServer]ID_DISCONNECTION_NOTIFICATION\n");
        else if(p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
            dprintf("[InfoServer]ID_CONNECTION_ATTEMPT_FAILED\n");
        else if (p->data[0]==ID_CONNECTION_LOST)
            dprintf("[InfoServer]ID_CONNECTION_LOST\n");
        else if (p->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
            dprintf("[InfoServer]ID_NO_FREE_INCOMING_CONNECTIONS\n");
        else if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
        {   // send info packet
            dprintf("[InfoServer]ID_NEW_INCOMING_CONNECTION\n");
            RakNet::BitStream bitStream;
            bitStream.Write((UCHAR)ID_USER_PACKET_ENUM);
            ServerInfoToBitStream(&m_ServerInfo, &bitStream);
            server->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE, 0, p->playerId, false);
            dprintf("[InfoServer]SENT INFO TO CLIENT: %s\n", p->playerId.ToString(true));
            server->Kick(p->playerId);
        }
        else if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
            dprintf("[InfoServer]ID_CONNECTION_REQUEST_ACCEPTED\n");                    
        else if (p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
            dprintf("[InfoServer]ID_CONNECTION_ATTEMPT_FAILED\n");
        
        server->DeallocatePacket(p);
        p = server->Receive();
    }
}

bool CServer::StartInfoServer()
{
    bool bReturn = m_pRakInfo->Start(20, 0, 50, INFO_SERVER_PORT);
    if(bReturn) dprintf("[StartInfoServer] INFO SERVER STARTED.\n");
    return bReturn;
}

bool CServer::RegisterServerWithMaster(USHORT nPort,
                                       DatabaseCellUpdate *pCellUpdates, 
                                       int nCells, 
                                       const SMasterServerInfo& masterInfo)
{
    RowUpdateMode updateMode = RUM_UPDATE_OR_ADD_ROW;   // update mode on master server

    // connect to master server    
    RakPeerInterface* rakPeer = RakNetworkFactory::GetRakPeerInterface();
    bool bStart = rakPeer->Initialize(1, MASTER_REGISTER_PORT, 50, 0);
    if(!bStart) return bStart;

    LightweightDatabaseClient databaseClient;    
    rakPeer->AttachPlugin(&databaseClient);
    dprintf("[REGISTER SERVER]Registering w/ Master Server: %s : %d\n", masterInfo.strIP.c_str(), masterInfo.nPort);
    bool bConnect = rakPeer->Connect(masterInfo.strIP.c_str(), masterInfo.nPort, 0, 0);

    if(bConnect)    // update master server!
    {
        bool bGotReply = false;
        Packet *p;
        while (bGotReply == false)
        {
            p=rakPeer->Receive();
            while (p)
            {
                bGotReply = true;
                if (p->data[0]==ID_DISCONNECTION_NOTIFICATION)
                    dprintf("ID_DISCONNECTION_NOTIFICATION\n");
                else if (p->data[0]==ID_CONNECTION_LOST)
                    dprintf("ID_CONNECTION_LOST\n");
                else if (p->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
                    dprintf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
                else if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
                    dprintf("ID_NEW_INCOMING_CONNECTION\n");
                else if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
                    dprintf("ID_CONNECTION_REQUEST_ACCEPTED\n");                    
                else if (p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
                    dprintf("ID_CONNECTION_ATTEMPT_FAILED\n");
                else if (p->data[0]==ID_DATABASE_UNKNOWN_TABLE)
                    dprintf("ID_DATABASE_UNKNOWN_TABLE\n");
                else if (p->data[0]==ID_DATABASE_INCORRECT_PASSWORD)
                    dprintf("ID_DATABASE_INCORRECT_PASSWORD\n");

                rakPeer->DeallocatePacket(p);
                p=rakPeer->Receive();
            }                                 
            RakSleep(30);   
        }
        databaseClient.UpdateRow(MASTER_TABLE_NAME, MASTER_TABLE_PASSWORD, updateMode, false, 0, pCellUpdates, (UCHAR)nCells, UNASSIGNED_PLAYER_ID, true );
    }
    else
        dprintf("FAILED TO CONNECT TO MASTER SERVER: %s : %d\n", masterInfo.strIP.c_str(), masterInfo.nPort);

    rakPeer->Disconnect(100,0);   
    RakNetworkFactory::DestroyRakPeerInterface(rakPeer); 
    return bConnect;
}

bool CServer::RemoveFromMasters(USHORT nPort, const SMasterServerInfo& masterInfo)
{
    // connect to master server    
    RakPeerInterface* rakPeer = RakNetworkFactory::GetRakPeerInterface();
    bool bStart = rakPeer->Initialize(1, MASTER_REGISTER_PORT, 0, 0);
    if(!bStart) return bStart;

    LightweightDatabaseClient databaseClient;    
    rakPeer->AttachPlugin(&databaseClient);
    dprintf("[REGISTER SERVER]Registering w/ Master Server: %s : %d\n", masterInfo.strIP.c_str(), masterInfo.nPort);
    bool bConnect = rakPeer->Connect(masterInfo.strIP.c_str(), masterInfo.nPort, 0, 0);

    if(bConnect)    // update master server!
    {
        bool bGotReply = false;
        Packet *p;
        while (bGotReply == false)
        {
            p=rakPeer->Receive();
            while (p)
            {
                bGotReply = true;
                if (p->data[0]==ID_DISCONNECTION_NOTIFICATION)
                    dprintf("ID_DISCONNECTION_NOTIFICATION\n");
                else if (p->data[0]==ID_CONNECTION_LOST)
                    dprintf("ID_CONNECTION_LOST\n");
                else if (p->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
                    dprintf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
                else if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
                    dprintf("ID_NEW_INCOMING_CONNECTION\n");
                else if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
                    dprintf("ID_CONNECTION_REQUEST_ACCEPTED\n");                    
                else if (p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
                    dprintf("ID_CONNECTION_ATTEMPT_FAILED\n");
                else if (p->data[0]==ID_DATABASE_UNKNOWN_TABLE)
                    dprintf("ID_DATABASE_UNKNOWN_TABLE\n");
                else if (p->data[0]==ID_DATABASE_INCORRECT_PASSWORD)
                    dprintf("ID_DATABASE_INCORRECT_PASSWORD\n");

                rakPeer->DeallocatePacket(p);
                p=rakPeer->Receive();
            }                                 
            RakSleep(30);   
        }
        databaseClient.RemoveRow(MASTER_TABLE_NAME, MASTER_TABLE_PASSWORD, 0, UNASSIGNED_PLAYER_ID, true);
    }
    else
        dprintf("[RemoveFromMasters]FAILED TO REMOVE SELF FROM MASTER SERVER: %s : %d\n", masterInfo.strIP.c_str(), masterInfo.nPort);

    rakPeer->Disconnect(100,0);   
    RakNetworkFactory::DestroyRakPeerInterface(rakPeer);

    return bConnect;
}

//
// Functions to stuff database cell update rows
//
VOID CServer::StuffCell(DatabaseCellUpdate *pCellUpdate, string strCellName, int nCellValue)
{
    assert(strCellName.size() <= 32);
    strncpy_s(pCellUpdate->columnName, 32, strCellName.c_str(), 32);
    pCellUpdate->cellValue.Set(nCellValue);
    pCellUpdate->columnType = DataStructures::Table::NUMERIC;
}

VOID CServer::StuffCell(DatabaseCellUpdate *pCellUpdate, string strCellName, string strCellValue)
{
    assert(strCellName.size() < 32 && strCellValue.size() < 32);
    strncpy_s(pCellUpdate->columnName, 32, strCellName.c_str(), 32);

    char buf[32];
    strncpy_s(buf, 32, strCellValue.c_str(), 32);

    pCellUpdate->cellValue.Set(buf);
    pCellUpdate->columnType = DataStructures::Table::STRING;
}
