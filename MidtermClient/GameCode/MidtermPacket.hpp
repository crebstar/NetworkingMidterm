#ifndef included_MidtermPacket
#define included_MidtermPacket
#pragma once


typedef unsigned char MidtermPacketType;
const MidtermPacketType TYPE_Acknowledge	= 20;
const MidtermPacketType TYPE_Tagged			= 21;
const MidtermPacketType TYPE_Update			= 22;
const MidtermPacketType TYPE_Reset			= 23;

//-----------------------------------------------------------------------------------------------
struct AckPacket
{
	MidtermPacketType		m_packetType;
	unsigned int			m_packetNumber;
};

//-----------------------------------------------------------------------------------------------
// Note: Player orientation will always start at 0 degrees facing east
// Players are assigned color here
struct ResetPacket
{
	float			m_playerXPos;
	float			m_playerYPos;
	unsigned char	m_playerIDWhoIsIt;
	unsigned char	m_red;
	unsigned char	m_green;
	unsigned char	m_blue;
};

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
struct UpdatePacket
{
	float		m_xPosition;
	float		m_yPosition;
	float		m_xVelocity;
	float		m_yVelocity;
	float		m_yawDegrees;
};

//-----------------------------------------------------------------------------------------------

struct TaggedPacket
{
	unsigned char	m_playerIDWhoWasTagged;
};


struct MidtermPacket
{
	MidtermPacketType			m_packetType;
	unsigned char				m_playerID;
	unsigned int				m_packetNumber;
	double						m_timestamp;
	union PacketData
	{
		AckPacket			acknowledged;
		ResetPacket			reset;
		UpdatePacket		updated;
		TaggedPacket		tagged;
	} data;

	inline bool operator< ( const MidtermPacket& rhs ) {
		
		if ( this->m_packetNumber < rhs.m_packetNumber ) {
			return true;
		}

		return false;
	}
};


#endif