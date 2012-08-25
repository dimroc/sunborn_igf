#include "LightweightDatabaseServer.h"
#include "LightweightDatabaseClient.h"
#include "TableSerializer.h"
#include "BitStream.h"
#include "StringCompressor.h"
#include "DS_Table.h"
#include "LightweightDatabaseClient.h"
#include "LightweightDatabaseServer.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include "RakSleep.h"
#include "PacketEnumerations.h"

#include <stdlib.h>

#ifdef WIN32
#include <conio.h>
#elif __unix
#include <stdio.h>
#include <curses.h>
#endif


#define         MASTER_TABLE_NAME           "SunBornMaster"
#define         MASTER_TABLE_PASSWORD       "sunb0rnstud"
#define         MASTER_SERVER_PASSWORD      "m4st3R_S3RV3R"

void main(int argc, char*argv[])
{
	LightweightDatabaseClient databaseClient;
	RakPeerInterface *rakPeer = RakNetworkFactory::GetRakPeerInterface();
	char str[256];	
	char tableName[256], tablePassword[256];
	printf("Running SunBorn MasterServer Query Tool\n");

    int nPort = 80;
    if(argc == 2)
        nPort = atoi(argv[1]);
	
	rakPeer->Initialize(1, 0, 20, 0);
	rakPeer->AttachPlugin(&databaseClient);
	printf("Client started\n");

	printf("Enter server IP: ");
	gets(str);
	if (str[0]==0)
		strcpy(str, "127.0.0.1");
    printf("Connecting to server on PORT: %d.\n", nPort);
	rakPeer->Connect(str, nPort, 0, 0);
	printf("(Q)uery table\n");
	printf("(E)xit\n");

	Packet *p;
	while (1)
	{
		p=rakPeer->Receive();
		while (p)
		{
			if (p->data[0]==ID_DISCONNECTION_NOTIFICATION)
				printf("ID_DISCONNECTION_NOTIFICATION\n");
			else if (p->data[0]==ID_CONNECTION_LOST)
				printf("ID_CONNECTION_LOST\n");
			else if (p->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
			else if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
				printf("ID_NEW_INCOMING_CONNECTION\n");
			else if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
				printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
			else if (p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
				printf("ID_CONNECTION_ATTEMPT_FAILED\n");
			else if (p->data[0]==ID_DATABASE_UNKNOWN_TABLE)
				printf("ID_DATABASE_UNKNOWN_TABLE\n");
			else if (p->data[0]==ID_DATABASE_INCORRECT_PASSWORD)
				printf("ID_DATABASE_INCORRECT_PASSWORD\n");
			else if (p->data[0]==ID_DATABASE_QUERY_REPLY)
			{
				printf("Incoming table:\n");
				DataStructures::Table table;
				if (TableSerializer::DeserializeTable(p->data+sizeof(MessageID), p->length-sizeof(MessageID), &table))
				{
					DataStructures::Page<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER> *cur = table.GetListHead();
					unsigned i;

					printf("Columns:\n");
					for (i=0; i < table.GetColumns().Size(); i++)
					{
						printf("%i. %s : ", i+1, table.GetColumns()[i].columnName);
						if (table.GetColumns()[i].columnType==DataStructures::Table::BINARY)
							printf("BINARY");
						else if (table.GetColumns()[i].columnType==DataStructures::Table::NUMERIC)
							printf("NUMERIC");
						else
							printf("STRING");
						printf("\n");
					}
					if (cur)
                    {   // test player ID hack
						printf("Rows:\n");
                    }
					else
						printf("Table has no rows.\n");
					while (cur)
					{
						for (i=0; i < (unsigned)cur->size; i++)
						{
							table.PrintRow(str, 256, ',', true, cur->data[i]);
                            PlayerID playerID;
                            memcpy(&playerID, cur->data[i]->cells[0]->c, sizeof(PlayerID));                            
                            printf("RowID %i FOR IP%s: %s\n", cur->keys[i], playerID.ToString(true), str );
						}
						cur=cur->next;
					}
				}
				else
					printf("Deserialization of table failed.\n");
			}

			rakPeer->DeallocatePacket(p);
			p=rakPeer->Receive();
		}

        char ch=getch();
				
        if (ch=='q')
	    {					
		    strcpy(tableName, MASTER_TABLE_NAME);
            strcpy(tablePassword, MASTER_TABLE_PASSWORD);													

		    // TODO - let the user enter filters, columns, and rows to return.
		    databaseClient.QueryTable(tableName, tablePassword, 0, 0, 0, 0, 0, 0, UNASSIGNED_PLAYER_ID, true);
	    }

	    if (ch=='e')
		    break;

		ch=0;
		RakSleep(30);
	}

	rakPeer->Disconnect(100,0);
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}
