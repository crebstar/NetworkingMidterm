#include "NetworkAgent.hpp"

#include "../../CBEngine/EngineCode/TimeUtil.hpp"


NetworkAgent::~NetworkAgent() {

}


NetworkAgent::NetworkAgent() {

	setNetworkAgentDefaults();
}


NetworkAgent::NetworkAgent( const std::string& serverIPAddress, const std::string& serverPortNumber, bool connectOnConstruction /* = true */ ) :
	m_serverIPAddress( serverIPAddress ),
	m_serverPortNumber( serverPortNumber )
{
	setNetworkAgentDefaults();

	if ( connectOnConstruction ) {

		establishConnectionToServer(); 
	}
}


void NetworkAgent::update( float deltaSeconds ) {

}


void NetworkAgent::render( float deltaSeconds ) const {


}


void NetworkAgent::establishConnectionToServer() {

	printf( "Initializing connection to UDP server from UDP client...\n" );

	WSAData wsaData;
	int wsResult = WSAStartup( MAKEWORD( 2,2 ), &wsaData ) ;

	if ( wsResult != 0 ) {

		printf("WSAStartup failed with error code: %d\n", wsResult );
		return;
	}

	m_serverSocket = INVALID_SOCKET;
	addrinfo* result = nullptr;
	addrinfo hints;

	ZeroMemory( &hints, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM; 
	hints.ai_protocol = IPPROTO_UDP;

	wsResult = getaddrinfo( m_serverIPAddress.c_str(), m_serverPortNumber.c_str(), &hints, &result );

	if ( wsResult != 0 ) {

		printf( "getaddrinfo failed with error: %d\n", wsResult );
		WSACleanup();
		return;
	}

	addrinfo* ptr = nullptr;
	for ( ptr = result; ptr != nullptr; ptr = ptr->ai_next ) {

		m_serverSocket = socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );
		if ( m_serverSocket == INVALID_SOCKET ) {

			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}
	}

	freeaddrinfo( result );

	if ( m_serverSocket == INVALID_SOCKET ) {

		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}

	// Make socket non blocking ( Will need to expose options in interface in future )
	u_long iMode = 1; // 0 = blocking ... != 0 is non blocking
	wsResult = ioctlsocket( m_serverSocket, FIONBIO, &iMode );
	if ( wsResult != NO_ERROR ) {

		printf( "ioctlsocket failed with error: %ld\n", wsResult );
	}

	m_serverAddressLength = sizeof( m_serverAddress );

	memset( (char*) &m_serverAddress, 0, m_serverAddressLength );
	m_serverAddress.sin_family = AF_INET;

	m_serverAddress.sin_port = htons( ( unsigned short ) atoi( m_serverPortNumber.c_str() ) );
	m_serverAddress.sin_addr.S_un.S_addr = inet_addr( m_serverIPAddress.c_str() );

	m_connectionIsActive = true;
}


void NetworkAgent::establishConnectionToNewServer( const std::string& serverIPAddress, const std::string& serverPortNumber ) {


}


void NetworkAgent::sendPlayerDataPacketToServer( const MidtermPacket& playerPacket ) {

	if ( m_connectionIsActive ) {

		// Keeping this around for sendto ref
		int wsResult = 0;
		wsResult = sendto( m_serverSocket, (char*) &playerPacket, sizeof( MidtermPacket ), 0, (sockaddr*) &m_serverAddress, m_serverAddressLength ); // For UDP use sendTo

		if ( wsResult == SOCKET_ERROR ) {

			printf( "sendto failed with error: %d\n", WSAGetLastError() );
		
		}
	} 
}


bool NetworkAgent::getOrderedPacketsForEachUniqueID( std::map<int,std::set<MidtermPacket>>& out_orderedPackets ) {

	bool packetsWereReceived = false;

	int winSockResult = 0;
	sockaddr_in serverAddress;
	int sizeOfServeraddress = sizeof( serverAddress );

	do {

		MidtermPacket packetReceived;
		packetReceived.m_playerID = 0;
		winSockResult = recvfrom( m_serverSocket, (char*) &packetReceived, sizeof( packetReceived ), 0, (sockaddr*) &serverAddress, &sizeOfServeraddress );

		if ( winSockResult > 0 ) {
		
			std::map<int,std::set<MidtermPacket>>::iterator itOrd = out_orderedPackets.find( packetReceived.m_playerID );
			if ( itOrd != out_orderedPackets.end() ) {

				std::set<MidtermPacket>& orderedPacketsSet = itOrd->second;
				orderedPacketsSet.insert( packetReceived );
				packetsWereReceived = true;

			} else {

				std::set<MidtermPacket> orderedPacketSet;
				orderedPacketSet.insert( packetReceived );
				out_orderedPackets.insert( std::pair<int,std::set<MidtermPacket>>( packetReceived.m_playerID, orderedPacketSet ) );
				packetsWereReceived = true;
			}
		}
		
	} while ( winSockResult > 0 );
	

	return packetsWereReceived;
}


bool NetworkAgent::requestToJoinServer( float deltaSeconds, MidtermPacket& out_resetPacketReceived ) {

	static float durationSincePacketSent = deltaSeconds;
	static bool bPacketToJoinSent = false;

	if ( !m_connectionIsActive ) {

		durationSincePacketSent = 0.0f;
		bPacketToJoinSent = false;
		return false;
	}

	if ( !bPacketToJoinSent ) {

		// Create join packet ( likely an Ack )
		MidtermPacket packetToJoin;
		packetToJoin.m_packetType = TYPE_Acknowledge;
		packetToJoin.m_packetNumber = 0; // Zero means don't care
		packetToJoin.m_playerID = 0;
		packetToJoin.m_timestamp = cbutil::getCurrentTimeSeconds();
		
		packetToJoin.data.acknowledged.m_packetNumber = 0;
		packetToJoin.data.acknowledged.m_packetType = TYPE_Acknowledge;

		int wsResult = 0;
		wsResult = sendto( m_serverSocket, (char*) &packetToJoin, sizeof( MidtermPacket ), 0, (sockaddr*) &m_serverAddress, m_serverAddressLength ); 

		if  ( wsResult == SOCKET_ERROR ) {

			printf( "sendto failed with error: %d\n", WSAGetLastError() );
			bPacketToJoinSent = false;

		} else {

			bPacketToJoinSent = true;
			durationSincePacketSent = 0.0f;
		}
		
	} else {

		durationSincePacketSent += deltaSeconds;

		int winSockResult = 0;
		sockaddr_in serverAddress;
		int sizeOfServeraddress = sizeof( serverAddress );

		winSockResult = recvfrom( m_serverSocket, (char*) &out_resetPacketReceived, sizeof( MidtermPacket ), 0, (sockaddr*) &serverAddress, &sizeOfServeraddress );

		if ( winSockResult > 0 ) {

			// Send confirmation packet ( Ack ) if the packet received is the correct one ( usually reset )
			MidtermPacket ackPacketForJoin;
			ackPacketForJoin.m_packetType = TYPE_Acknowledge;
			ackPacketForJoin.m_packetNumber = out_resetPacketReceived.m_packetNumber;
			ackPacketForJoin.m_playerID = 0;
			ackPacketForJoin.m_timestamp = cbutil::getCurrentTimeSeconds();

			ackPacketForJoin.data.acknowledged.m_packetNumber = out_resetPacketReceived.m_packetNumber;
			ackPacketForJoin.data.acknowledged.m_packetType = out_resetPacketReceived.m_packetType;

			int wsResult = 0;
			wsResult = sendto( m_serverSocket, (char*) &ackPacketForJoin, sizeof( MidtermPacket ), 0, (sockaddr*) &m_serverAddress, m_serverAddressLength ); 

			return true;
			
		} else {

			if ( durationSincePacketSent > THRESHOLD_FOR_CONNECTION_RERQUEST ) {

				durationSincePacketSent = 0.0f;
				bPacketToJoinSent = false;
			}
		}
	}

	return false;
}


// winSockResult = recvfrom( m_serverSocket, (char*) &out_playerPacketData, sizeof( out_playerPacketData ), 0, (sockaddr*) &serverAddress, &sizeOfServeraddress );

void NetworkAgent::setNetworkAgentDefaults() {

	m_connectionIsActive = false;
}




