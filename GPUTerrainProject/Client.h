#pragma once

#include "RakNet/RakNetworkFactory.h"
#include "RakNet/RakClientInterface.h"
#include "RakNet/NetworkIDGenerator.h"
#include "RakNet/GetTime.h"
#include "RakNet/Multiplayer.h"

#include "ServerInfo.h"

#include "GameEngine.h"

using stdext::hash_map;

//! Provies ability for GameEngine to talk to server. 
//! Glues the GameEngine to the RakNet through RPC registration of Object Member Functions.
class CClient : public Multiplayer<RakClientInterface>
{
private:
    bool                                    m_bConnectionRequested;
    SServerInfo                             m_ServerInfo;
    RakClientInterface*                     m_pRakClient;

    PlayerID                                m_PlayerID; // this clients playerid as sent by server acknowledgement.

    char                                    m_pszAlias[NUM_BYTES_FOR_NAMES];
	UCHAR									m_CharType;

    float                                   m_fUpdateInterval;
    float                                   m_fTimeSinceUpdate;

private:    
    
public:
    CClient(NetworkID networkID, LPCSTR pszAlias, UCHAR cDefaultCharType);
    ~CClient(void);

    bool ConnectToServer(const SServerInfo& serverInfo);    //! Connects to passed server, returns true on success.
    VOID Disconnect();  //! Disconnects from server if connected.

    //! Register functions to be called by server on _CLIENT_
    VOID RegisterRPCFunction(RakNetFuncPtr pFunc);  

    void ProcessPackets();  // not the overridden vritual one. Operator overload.

    // overridden virtual functions from multiplayer.h to handle incoming packets.
    // overridden to automatically send acknowledge packet when connected.
    virtual void ReceiveConnectionRequestAccepted( Packet *packet, RakClientInterface *interfaceType );
    virtual void ProcessUnhandledPacket( Packet *packet, unsigned char packetIdentifier, RakClientInterface *interfaceType );

    //! abbreviated ServerRPCEx call with defaults loaded(broadcast,  priority, etc.) to call RPCs on _SERVER_ machines
    bool RPC(LPSTR pszFunc, RakNet::BitStream *pBitStream);    

    //! Calls RPC on a _SERVER_ machine with the passed in bitStream.
    bool RPC(LPSTR pszFunc, 
                 RakNet::BitStream *pBitStream, 
                 PacketPriority priority, 
                 PacketReliability reliability, 
                 char orderingChannel, 
                 bool shiftTimestamp,
                 NetworkID networkID, 
                 RakNet::BitStream *replyFromTarget );

    RakNetTime GetRakTime()                                 {   return RakNet::GetTime();  }

    VOID SetUpdateInterval(float fUpdateInterval)           {   m_fUpdateInterval = fUpdateInterval;    }
    bool IsTimeForUpdate(float fElapsedTime);

    //! Set the player alias (dimroc ftw!)
    VOID SetAlias(LPCSTR pszAlias)                          {   assert(pszAlias); strncpy_s(m_pszAlias, NUM_BYTES_FOR_NAMES, pszAlias, NUM_BYTES_FOR_NAMES); }

    VOID SetPlayerID(const PlayerID &_playerID)             {   m_PlayerID = _playerID; }
    PlayerID GetPlayerID()                                  {   return m_PlayerID; }

	UCHAR GetCharType()										{	return m_CharType; }
	VOID SetCharType(UCHAR charType)						{	m_CharType = charType; }

    const SServerInfo& GetServerInfo()                      {   return m_ServerInfo; }
    bool IsConnected()                                      {   return m_pRakClient->IsConnected();  }
};
