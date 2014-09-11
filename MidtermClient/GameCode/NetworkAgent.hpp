#ifndef included_NetworkAgent
#define included_NetworkAgent
#pragma once

#include <string>

#include "../../CBEngine/EngineCode/EngineMacros.hpp"

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#include <windows.h> // In future abstract this out to engine code and remove windows dependency

#include "FeedbackWorld.hpp"
#include "CS6Packet.hpp"

const float THRESHOLD_FOR_CONNECTION_RERQUEST = 1.0f;

class NetworkAgent {
public:
	~NetworkAgent();
	NetworkAgent();
	explicit NetworkAgent( const std::string& serverIPAddress, const std::string& serverPortNumber, bool connectOnConstruction = true );

	void update( float deltaSeconds );
	void render( float deltaSeconds ) const; // Give an opportunity for any debug rendering

	void establishConnectionToServer();
	void establishConnectionToNewServer( const std::string& serverIPAddress, const std::string& serverPortNumber );

	bool requestToJoinServer( float deltaSeconds, CS6Packet& out_resetPacketReceived );
	void sendPlayerDataPacketToServer( const CS6Packet& playerPacket );

protected:

	void setNetworkAgentDefaults();

	std::string				m_serverIPAddress;
	std::string				m_serverPortNumber;
	bool					m_connectionIsActive;

	SOCKET					m_serverSocket;
	sockaddr_in				m_serverAddress;
	int						m_serverAddressLength;

private:
	PREVENT_COPY_AND_ASSIGN( NetworkAgent );


};


#endif