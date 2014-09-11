#include "UDPServer.hpp"
#include <stdio.h>
#include <iostream>

#include <vector>
#include <random>
#include <time.h>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include "ConnectedUDPClient.hpp"


#include "../../CBEngine/EngineCode/TimeUtil.hpp"
#include "../../CBEngine/EngineCode/MathUtil.hpp"

UDPServer::~UDPServer() {

}


UDPServer::UDPServer( const std::string& ipAddress, const std::string& portNumber ) {

	m_thresholdForPacketLossSimulation = 1.00f;

	m_IPAddress = ipAddress;
	m_PortNumber = portNumber;

	m_durationSinceLastUserConnectedUpdate = 0.0;
	m_durationSinceLastPacketUpdate = 0.0;
	
	m_currentAckCount = 0;

	srand( time( nullptr ) );

}


void UDPServer::initialize() {

	printf( "\n\nAttempting to create UDP Server with IP: %s and Port: %s \n", m_IPAddress.c_str(), m_PortNumber.c_str() );

	WSAData wsaData;
	int winSockResult = 0;

	m_listenSocket = INVALID_SOCKET;

	struct addrinfo* result = nullptr;
	struct addrinfo hints;

	winSockResult = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
	if ( winSockResult != 0 ) {

		printf( "WSAStartup failed with error number: %d\n", winSockResult );
		return;
	}

	ZeroMemory( &hints, sizeof( hints ) );
	hints.ai_family		= AF_INET;
	hints.ai_socktype	= SOCK_DGRAM;
	hints.ai_protocol	= IPPROTO_UDP;
	hints.ai_flags		= AI_PASSIVE;


	winSockResult = getaddrinfo( m_IPAddress.c_str(), m_PortNumber.c_str(), &hints, &result );
	if ( winSockResult != 0 ) {

		printf( "getaddrinfo function call failed with error number: %d\n", winSockResult );
		WSACleanup();

		return;
	}

	// Initialize the ListenSocket ( Connect, Bind, then listen )
	m_listenSocket = socket( result->ai_family, result->ai_socktype, result->ai_protocol );
	if ( m_listenSocket == INVALID_SOCKET ) {

		printf( "socket function call failed with error number: %ld\n", WSAGetLastError() );

		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	winSockResult = bind( m_listenSocket, result->ai_addr, static_cast<int>( result->ai_addrlen ) );

	if ( winSockResult == SOCKET_ERROR ) {

		printf( "Bind to listenSocket failed with error number: %d\n", WSAGetLastError() );

		freeaddrinfo( result );
		closesocket( m_listenSocket );
		WSACleanup();

		return;
	}

	u_long iMode = 1; // 0 = blocking ... != 0 is non blocking
	winSockResult = ioctlsocket( m_listenSocket, FIONBIO, &iMode );
	if ( winSockResult != NO_ERROR ) {

		printf( "ioctlsocket failed with error: %ld\n", winSockResult );
	}

	freeaddrinfo(result);
}


void UDPServer::run() {

	int winSockResult = 0;
	int winSockSendResult = 0;
	m_serverShouldRun = true;

	while ( m_serverShouldRun ) {

		checkForExpiredClients();
		displayConnectedUsers();

		MidtermPacket clientPacketReceived;

		sockaddr_in clientSocketAddr;
		int sizeOfResultAddress = sizeof( clientSocketAddr );
		winSockResult = recvfrom( m_listenSocket, (char*) &clientPacketReceived, sizeof( MidtermPacket ), 0, (sockaddr*) &clientSocketAddr, &sizeOfResultAddress );

		if ( winSockResult > 0 ) {

			char* connectedIP = inet_ntoa( clientSocketAddr.sin_addr );
			int portNumber = ntohs( clientSocketAddr.sin_port );
			
			std::string combinedIPAndPort;
			convertIPAndPortToSingleString( connectedIP, portNumber, combinedIPAndPort );
			updateOrCreateNewClient( combinedIPAndPort, clientSocketAddr, clientPacketReceived );
	
		} 

		sendPlayerDataToClients();
		checkForExpiredReliablePacketsWithNoAcks();
	} 

	WSACleanup();

	printf( "UDP Server has finished executing\n\n" );
}


void UDPServer::convertIPAndPortToSingleString( char* ipAddress, int portNumber, std::string& out_combinedIPAndPort ) {

	char* portNumAsCString = new char[32];
	itoa( portNumber, portNumAsCString, 10 );

	out_combinedIPAndPort += ipAddress;
	out_combinedIPAndPort += " ";
	out_combinedIPAndPort += portNumAsCString;

	delete[] portNumAsCString;
}


void UDPServer::updateOrCreateNewClient( const std::string& combinedIPAndPort, const sockaddr_in& clientAddress, const MidtermPacket& playerData ) {

	std::map<std::string,ConnectedUDPClient*>::iterator itClient;

	itClient = m_clients.find( combinedIPAndPort );

	if ( itClient != m_clients.end() ) {
		// Client already exists
		ConnectedUDPClient* client = itClient->second;
		client->m_timeStampSecondsForLastPacketReceived = cbutil::getCurrentTimeSeconds();

		if ( playerData.m_packetType == TYPE_Update ) {

			client->m_position.x = playerData.data.updated.m_xPosition;
			client->m_position.y = playerData.data.updated.m_yPosition;
			client->m_velocity.x = playerData.data.updated.m_xVelocity;
			client->m_velocity.y = playerData.data.updated.m_yVelocity;
			client->m_orientationDegrees = playerData.data.updated.m_yawDegrees;

		} else if ( playerData.m_packetType == TYPE_Acknowledge ) {

			int packetIDToAck = playerData.data.acknowledged.m_packetNumber;
			std::map<int,MidtermPacket>::iterator itAck = client->m_reliablePacketsSentButNotAcked.find( packetIDToAck );
			if ( itAck != client->m_reliablePacketsSentButNotAcked.end() ) {

				client->m_reliablePacketsSentButNotAcked.erase( itAck );

			} else {

				printf( "Server Warning: Ack packet received that was not registered as a reliable packet\n\n" );
			}


		} else if ( playerData.m_packetType == TYPE_Tagged ) {

			sendResetPacketsToClientsAsUserWasTagged( playerData );

		} else if ( playerData.m_packetType == TYPE_Reset ) {

			printf( "Server Warning: Client sent a packet type of 'Reset'. This is NOT allowed\n\n" );

		} else {

			printf( "Server Warning: Unrecognized packet type received from an existing client\n\n" );
		}
		
	} else {
		// New client
		ConnectedUDPClient* client = new ConnectedUDPClient;
		client->m_clientAddress = clientAddress;
		client->m_userID = combinedIPAndPort;

		if ( m_clients.empty() ) {
			// Assumption: first client in is always 'it'
			client->m_isIT = true;
		}

		printf( "A new client has connected with IP and Port: %s \n\n", combinedIPAndPort.c_str() );
		m_clients.insert( std::pair<std::string,ConnectedUDPClient*>( combinedIPAndPort, client ) );

		if ( playerData.m_packetType == TYPE_Acknowledge ) {

			client->m_timeStampSecondsForLastPacketReceived = cbutil::getCurrentTimeSeconds();

			++m_currentAckCount;

			MidtermPacket resetPacket;
			resetPacket.m_packetType = TYPE_Reset;
			resetPacket.m_timestamp = cbutil::getCurrentTimeSeconds();
			resetPacket.m_playerID = client->m_playerID;
			resetPacket.m_packetNumber = m_currentAckCount;
			resetPacket.m_red = client->m_red;
			resetPacket.m_green = client->m_green;
			resetPacket.m_blue = client->m_blue;

			resetPacket.data.reset.m_red = client->m_red;
			resetPacket.data.reset.m_green = client->m_green;
			resetPacket.data.reset.m_blue = client->m_blue;

			unsigned char idOfPlayerWhoIsIt = getPlayerIDForPlayerWhoIsIt();
			if ( idOfPlayerWhoIsIt == NO_PLAYER_IS_IT ) 
			{
				printf( "Server Warning: No player has been assigned as 'it'!\n\n" );
			}

			resetPacket.data.reset.m_playerIDWhoIsIt = idOfPlayerWhoIsIt;

			resetPacket.data.reset.m_playerXPos = client->m_startingPos.x;
			resetPacket.data.reset.m_playerYPos = client->m_startingPos.y;
			// Note: client is expected to clear velocity

			client->m_reliablePacketsSentButNotAcked.insert( std::pair<int,MidtermPacket>( resetPacket.m_packetNumber, resetPacket ) );

			int winSockSendResult = 0;
			winSockSendResult = sendto( m_listenSocket, (char*) &resetPacket, sizeof( MidtermPacket ), 0, (sockaddr*) &client->m_clientAddress, sizeof( client->m_clientAddress ) );

			if ( winSockSendResult == SOCKET_ERROR ) {

				printf( "reset send function call failed with error number: %d\n", WSAGetLastError() );
			} 
		} else {

			printf( "Server Warning: A packet was received from a new client that was not allowed or expected. Only type expected is Acknowledge\n\n" );
		}
	}
}


void UDPServer::sendResetPacketsToClientsAsUserWasTagged( const MidtermPacket& packetRec ) {


	int clientWhoIsIt = packetRec.data.tagged.m_playerIDWhoWasTagged;

	printf( "Game has ended. Starting a new game with a new client as IT" );

	std::map<std::string,ConnectedUDPClient*>::iterator itClient;
	for ( itClient = m_clients.begin(); itClient != m_clients.end(); ++itClient ) {

		ConnectedUDPClient* client = itClient->second;

		if ( client->m_playerID == clientWhoIsIt ) {

			client->m_isIT = true;

		} else {

			client->m_isIT = false;
		}

		++m_currentAckCount;

		MidtermPacket resetPacketToSend;
		resetPacketToSend.m_packetType = TYPE_Reset;
		resetPacketToSend.m_timestamp = cbutil::getCurrentTimeSeconds();
		resetPacketToSend.m_playerID = client->m_playerID;
		resetPacketToSend.m_packetNumber = m_currentAckCount;
	
		resetPacketToSend.data.reset.m_playerXPos = client->m_startingPos.x;
		resetPacketToSend.data.reset.m_playerYPos = client->m_startingPos.y;
		resetPacketToSend.data.reset.m_playerIDWhoIsIt = clientWhoIsIt;

		client->m_reliablePacketsSentButNotAcked.insert( std::pair<int,MidtermPacket>( resetPacketToSend.m_packetNumber, resetPacketToSend ) );

		int winSockSendResult = 0;
		winSockSendResult = sendto( m_listenSocket, (char*) &resetPacketToSend, sizeof( MidtermPacket ), 0, (sockaddr*) &client->m_clientAddress, sizeof( client->m_clientAddress ) );

		if ( winSockSendResult == SOCKET_ERROR ) {

			printf( "reset send function call failed with error number: %d\n", WSAGetLastError() );
		} 
	}
}


unsigned char UDPServer::getPlayerIDForPlayerWhoIsIt() {

	std::map<std::string,ConnectedUDPClient*>::iterator itClient;
	for ( itClient = m_clients.begin(); itClient != m_clients.end(); ++itClient ) {

		ConnectedUDPClient* client = itClient->second;
		if ( client->m_isIT ) {

			return client->m_playerID;
		}
	}

	return NO_PLAYER_IS_IT;
}



void UDPServer::checkForExpiredClients() {

	std::map<std::string,ConnectedUDPClient*>::iterator itClient;
	std::vector<std::string> clientsToRemove;

	for ( itClient = m_clients.begin(); itClient != m_clients.end(); ++itClient ) {

		ConnectedUDPClient* client = itClient->second;
		double lastTimePacketReceived = client->m_timeStampSecondsForLastPacketReceived;
		double currentTimeSeconds = cbutil::getCurrentTimeSeconds();

		double secondsSinceLastPacketReceived = currentTimeSeconds - lastTimePacketReceived;

		if ( secondsSinceLastPacketReceived > DURATION_THRESHOLD_FOR_DISCONECT ) {

			clientsToRemove.push_back( itClient->first );
		}
	}

	for ( int i = 0; i < static_cast<int>( clientsToRemove.size() ); ++i ) {

		std::map<std::string,ConnectedUDPClient*>::iterator itClientRem;
		itClientRem = m_clients.find( clientsToRemove[i] );

		if ( itClientRem != m_clients.end() ) {

			ConnectedUDPClient* client = itClientRem->second;
			delete client;

			m_clients.erase( itClientRem );

			printf( "\nRemoving client due to inactivity. Client IP and Port: %s \n", clientsToRemove[i].c_str() );
		}
	}
}


void UDPServer::sendPlayerDataToClients() {

	static double lastTimeStampSeconds = cbutil::getCurrentTimeSeconds();

	double currentTimeSeconds = cbutil::getCurrentTimeSeconds();
	double timeDifSeconds = currentTimeSeconds - lastTimeStampSeconds;

	m_durationSinceLastPacketUpdate += timeDifSeconds;

	if ( m_durationSinceLastPacketUpdate > TIME_DIF_SECONDS_FOR_PACKET_UPDATE ) {

		m_durationSinceLastPacketUpdate = 0.0;

		// Push back all update packets to a vector
		// For each client, send packets

		std::vector<MidtermPacket> packetsToSend;
		std::map<std::string,ConnectedUDPClient*>::iterator itClient;
		for ( itClient = m_clients.begin(); itClient != m_clients.end(); ++itClient ) {

			ConnectedUDPClient* client = itClient->second;
			MidtermPacket playerDataPacket;

			++m_currentAckCount;
			playerDataPacket.m_packetType = TYPE_Update;
			playerDataPacket.m_packetNumber = m_currentAckCount;
			playerDataPacket.m_timestamp = cbutil::getCurrentTimeSeconds();
			playerDataPacket.m_playerID = client->m_playerID;
			playerDataPacket.m_red = client->m_red;
			playerDataPacket.m_green = client->m_green;
			playerDataPacket.m_blue = client->m_blue;

			playerDataPacket.data.updated.m_xPosition = client->m_position.x;
			playerDataPacket.data.updated.m_yPosition = client->m_position.y;
			playerDataPacket.data.updated.m_xVelocity = client->m_velocity.x;
			playerDataPacket.data.updated.m_yVelocity = client->m_velocity.y;
			playerDataPacket.data.updated.m_yawDegrees = client->m_orientationDegrees;

			packetsToSend.push_back( playerDataPacket );
		}
		

		std::map<std::string,ConnectedUDPClient*>::iterator itClientSend;
		for ( itClientSend = m_clients.begin(); itClientSend != m_clients.end(); ++itClientSend ) {

			ConnectedUDPClient* client = itClientSend->second;

			for ( int i = 0; i < packetsToSend.size(); ++i ) {

				MidtermPacket& packetToSend = packetsToSend[i];

				int winSockSendResult = 0;
				winSockSendResult = sendto( m_listenSocket, (char*) &packetToSend, sizeof( MidtermPacket ), 0, (sockaddr*) &client->m_clientAddress, sizeof( client->m_clientAddress ) );

				if ( winSockSendResult == SOCKET_ERROR ) {

					printf( "reset send function call failed with error number: %d\n", WSAGetLastError() );
				} 
			}
		}
	}

	lastTimeStampSeconds = currentTimeSeconds;
}


void UDPServer::displayConnectedUsers() {

	static double lastTimeStampSeconds = cbutil::getCurrentTimeSeconds();

	double currentTimeSeconds = cbutil::getCurrentTimeSeconds();
	double timeDifSeconds = currentTimeSeconds - lastTimeStampSeconds;

	m_durationSinceLastUserConnectedUpdate += timeDifSeconds;

	if ( m_durationSinceLastUserConnectedUpdate > TIME_DIF_SECONDS_FOR_USER_DISPLAY ) {

		m_durationSinceLastUserConnectedUpdate = 0.0;

		if ( m_clients.empty() ) {

			printf( "---- There are currently no clients connected ----\n\n" );

		} else {

			printf( "---- Displaying List Of Connected Clients ----\n\n");

			std::map<std::string,ConnectedUDPClient*>::iterator itClient;
			for ( itClient = m_clients.begin(); itClient != m_clients.end(); ++itClient ) {

				ConnectedUDPClient* client = itClient->second;
				printf( "Client with user ID: %s is connected to the server.\n", client->m_userID.c_str() );
			}

			printf( "---- End List Of Connected Clients ----\n\n");
		}
	}

	lastTimeStampSeconds = currentTimeSeconds;
}


void UDPServer::checkForExpiredReliablePacketsWithNoAcks() {

	double currentTimeSeconds = cbutil::getCurrentTimeSeconds();

	std::map<std::string,ConnectedUDPClient*>::iterator itClient;
	for ( itClient = m_clients.begin(); itClient != m_clients.end(); ++itClient ) {

		ConnectedUDPClient* client = itClient->second;
		std::map<int,MidtermPacket>::iterator itRel;
		for ( itRel = client->m_reliablePacketsSentButNotAcked.begin(); itRel != client->m_reliablePacketsSentButNotAcked.end(); ++itRel ) {

			int packetAckId = itRel->first;
			MidtermPacket& packetNotAcked = itRel->second;

			double timeStampDif = currentTimeSeconds - packetNotAcked.m_timestamp;
			if ( timeStampDif > TIME_THRESHOLD_TO_RESEND_RELIABLE_PACKETS ) {

				int winSockSendResult = 0;
				winSockSendResult = sendto( m_listenSocket, (char*) &packetNotAcked, sizeof( MidtermPacket ), 0, (sockaddr*) &client->m_clientAddress, sizeof( client->m_clientAddress ) );

				if ( winSockSendResult == SOCKET_ERROR ) {

					printf( "ack resend function call failed with error number: %d\n", WSAGetLastError() );
				} 
			}
		}
	}
}