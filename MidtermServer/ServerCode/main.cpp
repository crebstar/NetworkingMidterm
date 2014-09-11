
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <string>
#include <vector>

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "UDPServer.hpp"

#include "../../CBEngine/EngineCode/TimeUtil.hpp"

#define UNUSED( x ) (void)(x)

const int MINIMUM_ARGUMENT_COUNT		= 4;
const std::string TYPE_SERVER_STRING	= "server";
const std::string TYPE_CLIENT_STRING	= "client";
const std::string PROTOCOL_UDP_STRING	= "udp";
const std::string PROTOCOL_TCP_STRING	= "tcp";

typedef enum {

	TYPE_CLIENT_UDP,
	TYPE_CLIENT_TCP,
	TYPE_SERVER_UDP,
	TYPE_SERVER_TCP,
	TYPE_UNNKOWN,

} NetworkType;

void parseCommandLineArgs( int numberOfArgs, char** arguments, std::vector<std::string>& out_CommandLineTokens ) {

	printf( "Command Line Arguments Received:\n" );

	std::string token;
	for ( int i = 1; i < numberOfArgs; ++i ) {

		token = arguments[i];
		out_CommandLineTokens.push_back( token );

		printf( token.c_str() );
		printf( "\n" );
	}
}

/*
	Expected Format Command Line Args Order:
	Server/Client  UDP/TCP  IP  PORT
*/
NetworkType initializeBasedOnReceivedArguments( const std::vector<std::string>& commandLineTokens, std::string& out_IPAddress, std::string& out_PortNumber ) {

	NetworkType typeBasedOnArgs = TYPE_UNNKOWN;

	if ( commandLineTokens.size() < MINIMUM_ARGUMENT_COUNT ) {

		return typeBasedOnArgs;
	}

	const std::string& clientOrServerToken = commandLineTokens[0];
	bool isClient = false;

	if ( clientOrServerToken == TYPE_SERVER_STRING ) {

		isClient = false;

	} else if ( clientOrServerToken == TYPE_CLIENT_STRING ) {

		isClient = true;
	}

	const std::string UDPOrTCPToken = commandLineTokens[1];
	bool isUDP = false;

	if ( UDPOrTCPToken == PROTOCOL_UDP_STRING ) {

		isUDP = true;

	} else if ( UDPOrTCPToken == PROTOCOL_TCP_STRING ) {

		isUDP = false;
	}

	if ( isClient && isUDP ) {

		typeBasedOnArgs = TYPE_CLIENT_UDP;
		printf( "UDP Client Requested\n" );

	} else if ( isClient && !isUDP ) {

		typeBasedOnArgs = TYPE_CLIENT_TCP;
		printf( "TCP Client Requested\n" );

	} else if ( !isClient && isUDP ) {

		typeBasedOnArgs = TYPE_SERVER_UDP;
		printf( "UDP Server Requested\n" );

	} else if ( !isClient && !isUDP ) {

		typeBasedOnArgs = TYPE_SERVER_TCP;
		printf( "TCP Server Requested\n" );
	}

	out_IPAddress = commandLineTokens[2];
	out_PortNumber = commandLineTokens[3];

	return typeBasedOnArgs;
}

int main( int argc, char** argv ) {

	cbutil::initializeTimeSystem();

	std::vector<std::string> commandLineTokens;
	parseCommandLineArgs( argc, argv, commandLineTokens );

	NetworkType networkAppType = TYPE_UNNKOWN;
	std::string IPAddressReq;
	std::string PortNumberReq;
	networkAppType = initializeBasedOnReceivedArguments( commandLineTokens, IPAddressReq, PortNumberReq );

	UDPServer udpProtocolServer( IPAddressReq, PortNumberReq );
	udpProtocolServer.initialize();
	udpProtocolServer.run();
	
	printf( "Program Concluding..." );

	return 0;
}