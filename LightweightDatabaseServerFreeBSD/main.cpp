//
// SUNBORN FREEBSD MASTER SERVER
//

#include "TableSerializer.h"
#include "BitStream.h"
#include "StringCompressor.h"
#include "DS_Table.h"
// #include "LightweightDatabaseClient.h"
#include "LightweightDatabaseServer.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include "RakSleep.h"
#include "PacketEnumerations.h"
#include "GetTime.h"
#include <stdlib.h>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#ifdef WIN32
#include <conio.h>
#elif __unix
#include <stdio.h>
#include <curses.h>
#endif

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define NULL 0

using std::string;
using std::ifstream;
using std::stringstream;
using std::vector;


//! Columns used to populate a table.
struct DBColumnInfo
{
    char columnName[256];
    DataStructures::Table::ColumnType columnType;    
};

//! A table in the lightweightserver database.
struct DBTableInfo
{
    // #define _SIMPLE_DATABASE_PASSWORD_LENGTH 32
    // #define _SIMPLE_DATABASE_TABLE_NAME_LENGTH 32
    char tableName[_SIMPLE_DATABASE_TABLE_NAME_LENGTH];
    bool bAllowRemoteQuery, bAllowRemoteUpdate, bAllowRemoteRemove;
    char queryPassword[_SIMPLE_DATABASE_PASSWORD_LENGTH], updatePassword[_SIMPLE_DATABASE_PASSWORD_LENGTH], removePassword[_SIMPLE_DATABASE_PASSWORD_LENGTH];
    bool bOneRowPerSystemId;
    bool bOnlyUpdateOwnRows;
    bool bRemoveRowOnPingFailure;
    bool bRemoveRowOnDisconnect;
    bool bAutoGenerateRowIDs;
    
    vector<DBColumnInfo> vColumnInfo;
};

//! The collection of this programs tables (w/ columns)
struct DataBaseSetting
{
    unsigned short nLocalPort;
    
    vector<DBTableInfo> vTableInfo;    
};

int ReadDBSettingFile(DataBaseSetting *pdbsetting, string strDBFile)
{
    assert(pdbsetting);
    printf("Loading DB Format File: %s\n", strDBFile.c_str());

    ifstream fin(strDBFile.c_str());

    if(!fin.good())
        return EXIT_FAILURE;

    char buf[1000];
    string strHolder;    
    int nTables;
    fin >> pdbsetting->nLocalPort;
    fin >> nTables;
    fin.getline(buf, 999);  // finish grabbing this line.
    for(int idxTable = 0; idxTable < nTables && fin.good(); idxTable++)
    {   // read in pdbformat
        fin.getline(buf, 999);
        stringstream ss(buf);
        ss >> strHolder;

        if(strHolder.compare("#") == 0 || strHolder.size() == 0)
            continue;   // skip line

        else
        {
            DBTableInfo tableInfo;    

            strncpy(tableInfo.tableName, strHolder.c_str(), _SIMPLE_DATABASE_TABLE_NAME_LENGTH - 1);

            ss >> tableInfo.bAllowRemoteQuery >> tableInfo.bAllowRemoteUpdate >> tableInfo.bAllowRemoteRemove;
            
            ss >> strHolder;
            strncpy(tableInfo.queryPassword, strHolder.c_str(), _SIMPLE_DATABASE_PASSWORD_LENGTH - 1);
            ss >> strHolder;
            strncpy(tableInfo.updatePassword, strHolder.c_str(), _SIMPLE_DATABASE_PASSWORD_LENGTH - 1);
            ss >> strHolder;
            strncpy(tableInfo.removePassword, strHolder.c_str(), _SIMPLE_DATABASE_PASSWORD_LENGTH - 1);

            tableInfo.tableName[_SIMPLE_DATABASE_TABLE_NAME_LENGTH-1] = 
                tableInfo.queryPassword[_SIMPLE_DATABASE_PASSWORD_LENGTH-1] =
                tableInfo.updatePassword[_SIMPLE_DATABASE_PASSWORD_LENGTH-1] =
                tableInfo.removePassword[_SIMPLE_DATABASE_PASSWORD_LENGTH-1] = NULL;

            ss >> tableInfo.bOneRowPerSystemId >> tableInfo.bOnlyUpdateOwnRows;
            ss >> tableInfo.bRemoveRowOnPingFailure >> tableInfo.bRemoveRowOnDisconnect;
            ss >> tableInfo.bAutoGenerateRowIDs;
            
            int nCols;
            fin >> nCols;
            for(int iCols = 0; iCols < nCols; iCols++)
            {
                DBColumnInfo colInfo;
                int nColumnEnum;
                fin >> strHolder;
                fin >> nColumnEnum;
                assert(nColumnEnum >= 0 && nColumnEnum < 3);

                strncpy(colInfo.columnName, strHolder.c_str(), 255);
                colInfo.columnName[255] = NULL;
                colInfo.columnType = (DataStructures::Table::ColumnType)nColumnEnum;
                tableInfo.vColumnInfo.push_back(colInfo);
            }            
            pdbsetting->vTableInfo.push_back(tableInfo);
        }
    }
    fin.close();
    return EXIT_SUCCESS;
}

void PopulateDatabaseServer(LightweightDatabaseServer* pdbServer, DataBaseSetting *pdbsetting)
{    
    for(size_t i=0; i<pdbsetting->vTableInfo.size(); i++)
    {   // add tables
        DBTableInfo &tinfo = pdbsetting->vTableInfo[i];
        DataStructures::Table *table;
        table=pdbServer->AddTable(tinfo.tableName, 
                                  tinfo.bAllowRemoteUpdate, 
                                  tinfo.bAllowRemoteQuery, 
                                  tinfo.bAllowRemoteRemove, 
                                  tinfo.queryPassword, 
                                  tinfo.updatePassword, 
                                  tinfo.removePassword, 
                                  tinfo.bOneRowPerSystemId, 
                                  tinfo.bOnlyUpdateOwnRows, 
                                  tinfo.bRemoveRowOnPingFailure, 
                                  tinfo.bRemoveRowOnDisconnect, 
                                  tinfo.bAutoGenerateRowIDs);

        assert(table);
        for(size_t j=0; j<pdbsetting->vTableInfo[i].vColumnInfo.size(); j++)
        {   // add columns
            table->AddColumn(pdbsetting->vTableInfo[i].vColumnInfo[j].columnName, 
                             pdbsetting->vTableInfo[i].vColumnInfo[j].columnType);
        }
    }
}

int main(int argc, char *argv[])
{
    DataBaseSetting dbsetting;
    if(argc != 2 || ReadDBSettingFile(&dbsetting, argv[1]) == EXIT_FAILURE)
    {
        printf("Incorrect USage/File : <LightweightDatabaseServer> <DBSetting file>\n");
        return EXIT_FAILURE;
    }
	
	bool isServer;
	LightweightDatabaseServer databaseServer;	
	RakPeerInterface *rakPeer = RakNetworkFactory::GetRakPeerInterface();
	char str[256];	
	printf("Light Weight Database Server\n");

    // create server
    isServer=true;    
	rakPeer->SetMaximumIncomingConnections(100);
	rakPeer->Initialize(100, dbsetting.nLocalPort, 20, 0 );
	rakPeer->AttachPlugin(&databaseServer);
    // rakPeer->SetIncomingPassword("m4st3R_S3RV3R", 13);

    printf("\nServer started on port: %d\n\n", dbsetting.nLocalPort);
#if 0
	printf("(C)reate table\n");
	printf("(R)emove table\n");
#endif
    PopulateDatabaseServer(&databaseServer, &dbsetting); //! creates table and rows in db server

	Packet *p;
	while (1)
	{
		p=rakPeer->Receive();
		while (p)
		{
			if (p->data[0]==ID_DISCONNECTION_NOTIFICATION)
                printf("ID_DISCONNECTION_NOTIFICATION FROM %s at %d\n", p->playerId.ToString(true), RakNet::GetTime());
			else if (p->data[0]==ID_CONNECTION_LOST)
				printf("ID_CONNECTION_LOST FROM %s at %d\n", p->playerId.ToString(true), RakNet::GetTime());
			else if (p->data[0]==ID_NO_FREE_INCOMING_CONNECTIONS)
				printf("ID_NO_FREE_INCOMING_CONNECTIONS FROM %s at %d\n", p->playerId.ToString(true), RakNet::GetTime());
			else if (p->data[0]==ID_NEW_INCOMING_CONNECTION)
				printf("ID_NEW_INCOMING_CONNECTION FROM %s at %d\n", p->playerId.ToString(true), RakNet::GetTime());
			else if (p->data[0]==ID_CONNECTION_REQUEST_ACCEPTED)
				printf("ID_CONNECTION_REQUEST_ACCEPTED FROM %s at %d\n", p->playerId.ToString(true), RakNet::GetTime());
			else if (p->data[0]==ID_CONNECTION_ATTEMPT_FAILED)
				printf("ID_CONNECTION_ATTEMPT_FAILED FROM %s at %d\n", p->playerId.ToString(true), RakNet::GetTime());
			else if (p->data[0]==ID_DATABASE_UNKNOWN_TABLE)
				printf("ID_DATABASE_UNKNOWN_TABLE FROM %s at %d\n", p->playerId.ToString(true), RakNet::GetTime());
			else if (p->data[0]==ID_DATABASE_INCORRECT_PASSWORD)
				printf("ID_DATABASE_INCORRECT_PASSWORD FROM %s at %d\n", p->playerId.ToString(true), RakNet::GetTime());
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
						printf("Rows:\n");
					else
						printf("Table has no rows.\n");
					while (cur)
					{
						for (i=0; i < (unsigned)cur->size; i++)
						{
							table.PrintRow(str, 256, ',', true, cur->data[i]);
							printf("RowID %i: %s\n", cur->keys[i], str );
						}
						cur=cur->next;
					}
				}
				else
					printf("Deserialization of table failed.\n");
			}

			rakPeer->DeallocatePacket(p);
			p=rakPeer->Receive();
		}   // end while(p)		

		RakSleep(30);
	}

	rakPeer->Disconnect(100,0);
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);

	return 0;
}
