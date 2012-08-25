// NETWORK DEFINITIONS TO CONNECT TO MASTER SERVER. HARDCODED IN.

#pragma once

#define         MASTER_TABLE_NAME           "SunBornMaster"
#define         MASTER_TABLE_PASSWORD       "sunb0rnstud"
#define         MASTER_SERVER_PASSWORD      "m4st3R_S3RV3R"

#define         MASTER_REGISTER_PORT        12340   //! port used to register with master servers

#define         INFO_REQUEST_PORT           12341   //! info port
#define         CLIENT_PORT                 12342   //! client port to join server
// 5 is medium between server and client ports.
#define         INFO_SERVER_PORT            12348   //! info port
#define         SERVER_PORT                 12349   //! default server hosting port. not necessarily the server port.

#define         SUNBORN_VERSION             61204


// ------------------------------------------------------------
// Packet Definitions 
// ------------------------------------------------------------

#define         ID_SUNBORN_ACK              ID_USER_PACKET_ENUM + 1
