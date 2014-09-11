#ifndef included_FeedbackWorld
#define included_FeedbackWorld
#pragma once

#include <vector>
#include <string>

#include "../../CBEngine/EngineCode/EngineCommon.hpp"
#include "../../CBEngine/EngineCode/Light.hpp"

#include "Camera.hpp"
#include "Camera2D.hpp"
#include "GameObject.hpp"

#include "MidtermPacket.hpp"


const char PLAYER_DATA_PACKET_ID = 2;
const char NEW_PLAYER_ACK_ID = 3;
const char RELIABLE_ACK_ID	= 30;
const int PACKET_ACK_ID_NON_RELIABLE = -1;


typedef enum {

	GAME_STATE_WAITING_FOR_SERVER,
	GAME_STATE_RUNNING,

} GameState;

struct PlayerDataPacket {
public:
	PlayerDataPacket() :
	  m_packetID( PLAYER_DATA_PACKET_ID ),
	  m_playerID( -1 ),
	  m_red( 250 ),
	  m_green( 250 ),
	  m_blue( 250 ),
	  m_xPos( 0.0f ),
	  m_yPos( 0.0f ),
	  m_packetAckID( PACKET_ACK_ID_NON_RELIABLE ),
	  m_packetTimeStamp( 0.0 )
	{}

	unsigned char		m_packetID;
	unsigned char		m_red;
	unsigned char		m_green;
	unsigned char		m_blue;
	float				m_xPos;
	float				m_yPos;
	int					m_packetAckID;
	int					m_playerID;
	double				m_packetTimeStamp;
};

class EntityMesh;
class Clock;

class FeedbackWorld {
public:
	~FeedbackWorld();
	explicit FeedbackWorld();

	void update( float deltaSeconds );
	void render( float deltaSeconds ) const;

	Camera2D& getCurrentWorldCamera();

protected:
	void processKeyboardInput( float deltaSeconds );

	// Update Functions
	void updateWorldLights( float deltaSeconds );

	// Render Functions
	void renderDebugAxis() const;

	// Network functions
	void attemptToConnectToServer( float deltaSeconds );
	void computePlayerDesiredPosition( float deltaSeconds );
	void sendPlayerUpdateDataToServer( float deltaSeconds );
	void collectPacketDataFromServer( float deltaSeconds );
	void resetPlayerCache();

	// Initialization and Clean Up
	void setDefaultVariableValues();
	void freeMemoryOfOwnedObjectsAndCleanUp();
private:
	PREVENT_COPY_AND_ASSIGN( FeedbackWorld );

	Camera2D								m_camera2D;

	// For now
	GameObject								m_player;
	GameObject								m_flag;
	std::vector<GameObject*>				m_otherPlayers;
	GameState								m_gameState;
};

// Inline Functions
inline Camera2D& FeedbackWorld::getCurrentWorldCamera() {

	return m_camera2D;
}

#endif