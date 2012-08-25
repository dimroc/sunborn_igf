#include "dxstdafx.h"
#include "ServerFinder.h"

#include "networkdefine.h"
#include "globaldefine.h"

using std::vector;

#define                     MAX_LAN_SERVERS             1000

VOID CServerFinder::QueryMasterServer(ServerInfoSet &sServerListOut, SMasterServerInfo &masterInfo)
{
    LightweightDatabaseClient databaseClient;
    RakPeerInterface *rakPeer = RakNetworkFactory::GetRakPeerInterface();

    rakPeer->Initialize(2, 0, 0, 0);
    rakPeer->AttachPlugin(&databaseClient);
    dprintf("[QUERY MASTER SERVER]Querying Master Server: %s : %d\n", masterInfo.strIP.c_str(), masterInfo.nPort);
    rakPeer->Connect(masterInfo.strIP.c_str(), masterInfo.nPort, 0, 0);
    
    bool bGotReply = false;
    bool bAskForTable = false;
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
            {
                dprintf("ID_CONNECTION_REQUEST_ACCEPTED\n");
                bGotReply = false;
                bAskForTable = true;
            }
            else if (p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
                dprintf("ID_CONNECTION_ATTEMPT_FAILED\n");
            else if (p->data[0]==ID_DATABASE_UNKNOWN_TABLE)
                dprintf("ID_DATABASE_UNKNOWN_TABLE\n");
            else if (p->data[0]==ID_DATABASE_INCORRECT_PASSWORD)
                dprintf("ID_DATABASE_INCORRECT_PASSWORD\n");
            else if (p->data[0]==ID_DATABASE_QUERY_REPLY)
            {
                dprintf("Incoming table:\n");
                DataStructures::Table table;
                if (TableSerializer::DeserializeTable(p->data+sizeof(MessageID), p->length-sizeof(MessageID), &table))
                {
                    DataStructures::Page<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> *cur = table.GetListHead();

                    while(cur)
                    {   // populate server set
                        for(int i=0; i<cur->size; i++)
                        {
                            SServerInfo serverInfo;                            
                            PopulateServerInfo(&serverInfo, cur->keys[i], cur->data[i]);

                            // check if server ip already entered. update if present, add otherwise.
                            sServerListOut.insert(serverInfo);                            
                        }
                        cur = cur->next;
                    }
                }
                else
                    dprintf("Deserialization of table failed.\n");                
            }
            else
            {
                dprintf("SOME BOGUS SHIT HAPPENED! Packet ID: %d\n", (int)p->data[0]);      
                bGotReply = false;
            }

            rakPeer->DeallocatePacket(p);
            p=rakPeer->Receive();
        }        
        RakSleep(30);

        if(bAskForTable)  
        {   // TODO - let the user enter filters, columns, and rows to return.
            databaseClient.QueryTable(masterInfo.strTableName.c_str(), 
                                      masterInfo.strTablePassword.c_str(), 
                                      0, 0, 0, 0, 0, 0, 
                                      UNASSIGNED_PLAYER_ID, true);
            bAskForTable = false;
        }
    }

    rakPeer->Disconnect(100,0);
    RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}

// ping and scan lan servers for their info port.
VOID CServerFinder::QueryLanServers(ServerInfoSet &sServerListOut, USHORT nClientPort, USHORT nServerPort)
{    
    RakNetTime quitTime;    
    //
    // try ping info servers on lan
    //
    RakClientInterface *client = RakNetworkFactory::GetRakClientInterface();
    client->PingServer("255.255.255.255", nServerPort, nClientPort, true);  // to broadcast ping on lan.

    dprintf("[QUERY LAN SERVERS]Broadcast Pinging on server port %d client port %d\n", nServerPort, nClientPort);

    quitTime = RakNet::GetTime() + 1000;    // run for 5 seconds    
    // Loop for input
    Packet* p;	
    while (RakNet::GetTime() < quitTime)
    {
        p = client->Receive();

        if (p==0)
            continue;

        if (p->data[0]==ID_PONG)
        {
            RakNetTime time;
            memcpy((char*)&time, p->data+1, sizeof(RakNetTime));
            dprintf("Got pong from %s with time %i\n", p->playerId.ToString(true), RakNet::GetTime() - time);
        
            // add these servers to sServerList            
            SServerInfo serverInfo;
            serverInfo.strIP.assign(p->playerId.ToString(false));
            serverInfo.nPort = SERVER_PORT; // putting it as default until info server updates.
            serverInfo.nPing = RakNet::GetTime() - time;

            sServerListOut.insert(serverInfo);
        }
        client->DeallocatePacket(p);
    }

    RakNetworkFactory::DestroyRakClientInterface(client);
}


// ---------------------------------------------
// PRIVATE SHIZNITE
// ---------------------------------------------

VOID CServerFinder::PopulateServerInfo(SServerInfo *pOut, UINT nKey, PtrDatabaseRow row)
{
    assert(pOut && row);
    
    // HACK: THIS IS HARDCODED TO MATCH ServerDB_Format.txt
    /*
    1. __systemId : BINARY
    2. __lastPingResponseTime : NUMERIC
    3. __nextPingSendTime : NUMERIC
    NAME 1
    PORT 0
    MAXPLAYERS 0
    VERSION 0
    */

    //  databaseTable->systemIdColumnIndex = 0
    PlayerID playerID;
    memcpy(&playerID, row->cells[0]->c, sizeof(PlayerID));

    pOut->strIP.assign(playerID.ToString(false));    

    if(!row->cells[1]->isEmpty)
        pOut->strName.assign(row->cells[1]->c);         
    if(!row->cells[2]->isEmpty)
        pOut->nPort = (USHORT)row->cells[2]->i;  
    if(!row->cells[3]->isEmpty)
        pOut->nMaxPlayers = (USHORT)row->cells[3]->i;  
    if(!row->cells[4]->isEmpty)
        pOut->nVersion = row->cells[4]->i; 

    pOut->nPing = -1;
    pOut->nPlayers = 0;

    dprintf("[CServerFinder]FOUND SERVER: %s : %d\n", pOut->strIP.c_str(), pOut->nPort);
}