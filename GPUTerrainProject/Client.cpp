#include "dxstdafx.h"
#include "Client.h"
#include "NetworkDefine.h"

#define         DEFAULT_UPDATE_INTERVAL             0.2f    // in seconds

CClient::CClient(NetworkID networkID, LPCSTR pszAlias, UCHAR cDefaultCharType)  :
    m_bConnectionRequested(false),
	m_CharType(cDefaultCharType),
    m_fTimeSinceUpdate(0.f),
    m_fUpdateInterval(DEFAULT_UPDATE_INTERVAL)
{
    strncpy_s(m_pszAlias, NUM_BYTES_FOR_NAMES, pszAlias, NUM_BYTES_FOR_NAMES);
    m_pRakClient = RakNetworkFactory::GetRakClientInterface(); 
}

CClient::~CClient()
{
    if(m_pRakClient)
        RakNetworkFactory::DestroyRakClientInterface(m_pRakClient);
    m_pRakClient = NULL;
}

VOID CClient::Disconnect()
{
    m_bConnectionRequested = false;
    m_pRakClient->Disconnect(100,0);
}

bool CClient::ConnectToServer(const SServerInfo& serverInfo)
{
    Disconnect();

    m_ServerInfo = serverInfo;
    m_bConnectionRequested = m_pRakClient->Connect(serverInfo.strIP.c_str(), serverInfo.nPort, CLIENT_PORT, 0, 30);    
    if(m_bConnectionRequested)  dprintf("CONNECTING TO SERVER: %s - %s:%d\n", serverInfo.strName.c_str(), serverInfo.strIP.c_str(), serverInfo.nPort);
    return m_bConnectionRequested;
}


void CClient::ProcessPackets()
{ 
    if(m_pRakClient->IsConnected()) 
        Multiplayer::ProcessPackets(m_pRakClient); 
}

// ------------------------------------------------------------
// Overridden virtual functions
// ------------------------------------------------------------

void CClient::ReceiveConnectionRequestAccepted( Packet *packet, RakClientInterface *rakClient )
{   // on connection accepted, send acknowledgement packet to server with player info (name etc).

    RakNet::BitStream bitStream;

    // write player name that joined
    bitStream.Write(m_pszAlias, NUM_BYTES_FOR_NAMES);
	bitStream.Write((char*)&m_CharType, 1);	
    
    rakClient->RPC("ServerPlayerConnect", &bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, true, UNASSIGNED_NETWORK_ID, 0);
}

void CClient::ProcessUnhandledPacket( Packet *packet, unsigned char packetIdentifier, RakClientInterface *rakClient )
{
    // wtf i dunno this packet
    // panic and kick!
    dprintf("UNHANDLED PACKET: %d from %s with data:\n%s\n", packetIdentifier, packet->playerId.ToString(true), packet->data);        
}

//! Register functions to be called by server on _CLIENT_
VOID CClient::RegisterRPCFunction(RakNetFuncPtr pFunc)
{
#if 0
    union {
        void (__cdecl CGameEngine::*cFunc)( RPCParameters *rpcParms ); 
        void* voidFunc;
    }; 
    cFunc = pMemberFunc; 
    m_pRakClient->RegisterClassMemberRPC(pszFuncName, voidFunc);
#endif

    REGISTER_STATIC_RPC(m_pRakClient, pFunc);
}

//! abbreviated ServerRPCEx call with defaults loaded(broadcast,  priority, etc.) to call RPCs on _SERVER_ machines
bool CClient::RPC(LPSTR pszFunc, RakNet::BitStream *pBitStream)
{
    return m_pRakClient->RPC(pszFunc, pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, true, UNASSIGNED_NETWORK_ID, 0);    
}

//! Calls RPC on a _SERVER_ machine with the passed in bitStream.
bool CClient::RPC(LPSTR pszFunc, 
                 RakNet::BitStream *pBitStream, 
                 PacketPriority priority, 
                 PacketReliability reliability, 
                 char orderingChannel, 
                 bool shiftTimestamp,
                 NetworkID networkID, 
                 RakNet::BitStream *replyFromTarget )
{
    return m_pRakClient->RPC(pszFunc, pBitStream, priority, reliability, orderingChannel, shiftTimestamp, networkID, replyFromTarget);    
}

bool CClient::IsTimeForUpdate(float fElapsedTime)
{
    m_fTimeSinceUpdate += fElapsedTime;

    if(m_fTimeSinceUpdate >= m_fUpdateInterval)
    {
        m_fTimeSinceUpdate = 0.f;
        return true;
    }
    return false;
}


// -----------------------------------------------------------------
// PRIVATE SHIZNIT
// -----------------------------------------------------------------
