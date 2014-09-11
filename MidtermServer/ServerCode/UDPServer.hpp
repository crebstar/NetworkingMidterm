#ifndef includedUDPServer
#define includedUDPServer
#pragma once

#include <string>
#include <map>

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../MidtermPacket.hpp"

const float ARENA_WIDTH = 500.0f;
const float ARENA_HEIGHT = 500.0f;

const unsigned char NO_PLAYER_IS_IT = 0;

const double DURATION_THRESHOLD_FOR_DISCONECT = 1000.0; // FOR TESTING
const double TIME_DIF_SECONDS_FOR_USER_DISPLAY = 5.5;
const double TIME_DIF_SECONDS_FOR_PACKET_UPDATE = 0.0045;
const double TIME_THRESHOLD_TO_RESEND_RELIABLE_PACKETS = 0.350;

class ConnectedUDPClient;

class UDPServer {
public:
	~UDPServer();
	explicit UDPServer( const std::string& ipAddress, const std::string& portNumber );

	void initializeAndRun();

	void initialize();
	void run();

protected:

	// Gameplay
	unsigned char	getPlayerIDForPlayerWhoIsIt();

	SOCKET												m_listenSocket;

	std::string											m_IPAddress;
	std::string											m_PortNumber;

	bool												m_serverShouldRun;

	std::map<std::string,ConnectedUDPClient*>			m_clients;  // IPandPort = Key | Last packet received is value
	double												m_durationSinceLastUserConnectedUpdate;
	double												m_durationSinceLastPacketUpdate;

	// Reliable Stuff
	
	// Guarentee Delivery
	float												m_thresholdForPacketLossSimulation;
	int													m_currentAckCount;


private:

	void convertIPAndPortToSingleString( char* ipAddress, int portNumber, std::string& out_combinedIPAndPort );
	void updateOrCreateNewClient( const std::string& combinedIPAndPort, const sockaddr_in& clientAddress, const MidtermPacket& playerData );
	void checkForExpiredClients();
	void displayConnectedUsers();

	void sendPlayerDataToClients();

	// Guarenteed Delivery
	void checkForExpiredReliablePacketsWithNoAcks();
};


#endif