#pragma once
#ifndef INCLUDED_CS6_PACKET_HPP
#define INCLUDED_CS6_PACKET_HPP

//Communication Protocol:
//   Client->Server: Ack
//   Server->Client: Reset
//   Client->Server: Ack

//   ------Update Loop------
//		Client->Server: Update
//		Server->ALL Clients: Update
//		ALL Clients->Server: Ack
//   ----End Update Loop----

//   Client->Server: Victory
//   Server->Client: Ack
//   Server->ALL Clients: Victory
//   ALL Clients->Server: Ack
//   Server->ALL Clients: Reset

//-----------------------------------------------------------------------------------------------
typedef unsigned char PacketType;
static const PacketType TYPE_Acknowledge = 10;
static const PacketType TYPE_Victory = 11;
static const PacketType TYPE_Update = 12;
static const PacketType TYPE_Reset = 13;

//-----------------------------------------------------------------------------------------------
struct AckPacket
{
	PacketType packetType;
	unsigned int packetNumber;
};

//-----------------------------------------------------------------------------------------------
struct ResetPacket
{
	float flagXPosition;
	float flagYPosition;
	float playerXPosition;
	float playerYPosition;
	//Player orientation should always start at 0 (east)
	unsigned char playerColorAndID[ 3 ];
};

//-----------------------------------------------------------------------------------------------
struct UpdatePacket
{
	float xPosition;
	float yPosition;
	float xVelocity;
	float yVelocity;
	float yawDegrees;
	//0 = east
	//+ = counterclockwise
	//range 0-359
};

//-----------------------------------------------------------------------------------------------
struct VictoryPacket
{
	unsigned char playerColorAndID[ 3 ];
};



//-----------------------------------------------------------------------------------------------
struct CS6Packet
{
	PacketType packetType;
	unsigned char playerColorAndID[ 3 ];
	unsigned int packetNumber;
	double timestamp;
	union PacketData
	{
		AckPacket acknowledged;
		ResetPacket reset;
		UpdatePacket updated;
		VictoryPacket victorious;
	} data;
};

#endif //INCLUDED_CS6_PACKET_HPP