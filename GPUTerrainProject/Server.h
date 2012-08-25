#pragma once

#include "RakNet/LightweightDatabaseClient.h"
#include "RakNet/RakNetworkFactory.h"
#include "RakNet/RakPeerInterface.h"
#include "RakNet/RakServerInterface.h"
#include "RakNet/NetworkIDGenerator.h"
#include "RakNet/GetTime.h"
#include "RakNet/Multiplayer.h"

#include "ServerInfo.h"

#include "GameEngine.h"

using stdext::hash_map;

//! Class through which GameEngine hosts a game!
//! Glues the GameEngine to the RakNet through RPC registration of Object Member Functions.
class CServer : public Multiplayer<RakServerInterface>
{
private:
    bool                                    m_bInit;
    SServerInfo                             m_ServerInfo;
    vector<SMasterServerInfo>               m_vMasterServers;

    RakServerInterface*                     m_pRakServer;
    RakServerInterface*                     m_pRakInfo; //! interface used to reply to SUNBORN_INFO_REQUEST packets.    

    float                                   m_fBroadcastInterval; //! ms in between broadcast to all clients 'reality checks'
    float                                   m_fTimeSinceBroadcast; //! accumulated time since last broadcast.

private:
    bool StartInfoServer();
    VOID ProcessInfoPackets(RakServerInterface* server);

    bool RegisterServerWithMaster(USHORT nPort,
                                  DatabaseCellUpdate *pCellUpdates, 
                                  int nCells, 
                                  const SMasterServerInfo& masterInfo);
    //! uses the SAME port initially used to register so Master Server konw how it is.
    bool RemoveFromMasters(USHORT nPort, const SMasterServerInfo& masterInfo);  

    VOID StuffCell(DatabaseCellUpdate *pCellUpdate, string strCellName, int nCellValue);
    VOID StuffCell(DatabaseCellUpdate *pCellUpdate, string strCellName, string strCellValue);
    
public:
    CServer(NetworkID networkID);
    ~CServer(void);

    bool StartServer(SServerInfo& serverInfo, const vector<SMasterServerInfo> &m_vMasterServers);
    VOID Disconnect();

    //! Register functions to be called by clients on _SERVER_
    VOID RegisterRPCFunction(RakNetFuncPtr pFunc);  

    VOID ProcessPackets();

    // overridden virtual functions from multiplayer.h to handle incoming packets.
    // overridden to broadcast playerDisconnect to other clients
    virtual void ReceiveConnectionLost( Packet *packet, RakServerInterface *interfaceType ); 
    virtual void ProcessUnhandledPacket( Packet *packet, unsigned char packetIdentifier, RakServerInterface *interfaceType );

    //! abbreviated ServerRPCEx call with defaults loaded(broadcast,  priority, etc.) to call RPCs on _CLIENT_ machines
    bool RPC(LPSTR pszFunc, RakNet::BitStream *pBitStream);    

    //! Calls RPC on a(or all) _CLIENT_ machines with the passed in bitStream.
    bool RPC(LPSTR pszFunc, 
                     RakNet::BitStream *pBitStream, 
                     PacketPriority priority, 
                     PacketReliability reliability, 
                     char orderingChannel, 
                     PlayerID playerId, 
                     bool broadcast, 
                     bool shiftTimestamp, 
                     NetworkID networkID, 
                     RakNet::BitStream *replyFromTarget );

    VOID SetBroadcastInterval(float fBroadcastInterval)     {   assert(fBroadcastInterval > 0.f); m_fBroadcastInterval = fBroadcastInterval;  }
    bool IsTimeForRealityBroadcast(float fElapsedTime); // returns whether a broadcast from server to all clients should be done.

    const SServerInfo& GetServerInfo()                      {   return m_ServerInfo; }
    bool IsInitialized()                                    {   return m_pRakServer->IsActive(); }
};
