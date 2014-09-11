#include "FeedbackWorld.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h> // ugh get rid of this dependency for rendering debug axis
#include <string>

#include "../../CBEngine/EngineCode/GameDirector.hpp"
#include "../../CBEngine/EngineCode/InputHandler.hpp" 
#include "../../CBEngine/EngineCode/TimeSystem.hpp"
#include "../../CBEngine/EngineCode/Clock.hpp"
#include "../../CBEngine/EngineCode/EntityMesh.hpp"
#include "../../CBEngine/EngineCode/CB3DSMaxImporter.hpp"
#include "../../CBEngine/EngineCode/MathUtil.hpp"
#include "../../CBEngine/EngineCode/MatrixStack.hpp"
#include "../../CBEngine/EngineCode/Matrix44.hpp"
#include "../../CBEngine/EngineCode/Geometry3D.hpp"
#include "../../CBEngine/EngineCode/TimeUtil.hpp"


#include "GameConstants.hpp"
#include "FeedbackGame.hpp"
#include "NetworkAgent.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

FeedbackWorld::~FeedbackWorld() {

	freeMemoryOfOwnedObjectsAndCleanUp();
}


void FeedbackWorld::freeMemoryOfOwnedObjectsAndCleanUp() {


	// TODO:: Delete and clean up EntityMesh
}


FeedbackWorld::FeedbackWorld() {

	setDefaultVariableValues();
}



void FeedbackWorld::update( float deltaSeconds ) {

	if ( m_gameState == GAME_STATE_RUNNING ) {

		processKeyboardInput( deltaSeconds );
		computePlayerDesiredPosition( deltaSeconds );

		// Send data to network ... Like desired position / velocity of player
		sendPlayerUpdateDataToServer( deltaSeconds );

		// Collect data from network
		collectPacketDataFromServer( deltaSeconds );

		m_player.update( deltaSeconds );

		for ( int i = 0; i < m_otherPlayers.size(); ++i ) {

			GameObject* otherPlayer = m_otherPlayers[i];
		}


		checkForCollisionWithIT( deltaSeconds );

	} else if ( m_gameState == GAME_STATE_WAITING_FOR_SERVER ) {

		attemptToConnectToServer( deltaSeconds );
	}
}


void FeedbackWorld::collectPacketDataFromServer( float deltaSeconds ) {

	resetPlayerCache();

	cbengine::GameDirector* sharedGameDirector = cbengine::GameDirector::sharedDirector();
	FeedbackGame* fbGame = dynamic_cast<FeedbackGame*>( sharedGameDirector->getCurrentWorld() );

	if ( fbGame != nullptr ) {

		NetworkAgent* nwAgent = fbGame->getCurrentNetworkAgent();
		if ( nwAgent != nullptr ) {

			std::map<int,std::set<MidtermPacket>> orderedPackets;
			bool packetsWereReceived = nwAgent->getOrderedPacketsForEachUniqueID( orderedPackets );
			if ( packetsWereReceived ) {

				std::map<int,std::set<MidtermPacket>>::iterator itOrd;
				for ( itOrd = orderedPackets.begin(); itOrd != orderedPackets.end(); ++itOrd ) {

					GameObject* gamePlayerRecPacket = nullptr;
					int playerID = itOrd->first;
					if ( playerID == m_player.m_playerID ) {

						gamePlayerRecPacket = &m_player;

					} else {

						for ( int i = 0; i < m_otherPlayers.size(); ++i ) {

							GameObject* playerToCheck = m_otherPlayers[i];
							if ( playerToCheck->m_playerID == playerID ) {

								gamePlayerRecPacket = m_otherPlayers[i];
							}
						}
					}

					std::set<MidtermPacket>& orderedPacketSet = itOrd->second;

					while ( !orderedPacketSet.empty() ) {

						std::set<MidtermPacket>::iterator itPack;
						itPack = orderedPacketSet.begin();
						const MidtermPacket& packetRec = *(itPack);

						if ( gamePlayerRecPacket == nullptr ) {
							// New Player

							GameObject* newOtherPlayer = new GameObject;
							newOtherPlayer->m_playerID = packetRec.m_playerID;
							newOtherPlayer->m_isPlayer = false;
							newOtherPlayer->m_playerColor.x = cbengine::rangeMapFloat( 0.0f, 255.0f, 0.0f, 1.0f, packetRec.m_red );
							newOtherPlayer->m_playerColor.y = cbengine::rangeMapFloat( 0.0f, 255.0f, 0.0f, 1.0f, packetRec.m_green );
							newOtherPlayer->m_playerColor.z = cbengine::rangeMapFloat( 0.0f, 255.0f, 0.0f, 1.0f, packetRec.m_blue );
							newOtherPlayer->m_playerColor.w = 1.0f;
							
							m_otherPlayers.push_back( newOtherPlayer );

							gamePlayerRecPacket = newOtherPlayer;
						}

						if ( packetRec.m_packetType == TYPE_Update ) {

							if ( !gamePlayerRecPacket->m_updateProcessedForFrame ) {

								gamePlayerRecPacket->m_position.x = packetRec.data.updated.m_xPosition;
								gamePlayerRecPacket->m_position.y = packetRec.data.updated.m_yPosition;
								gamePlayerRecPacket->m_currentVelocity.x = packetRec.data.updated.m_xVelocity;
								gamePlayerRecPacket->m_currentVelocity.y = packetRec.data.updated.m_yVelocity;
								gamePlayerRecPacket->m_orientationDegrees = packetRec.data.updated.m_yawDegrees;

								gamePlayerRecPacket->m_updateProcessedForFrame = true;
							}
						}

						orderedPacketSet.erase( itPack );
					}
				}
			}
		}
	}
}


void FeedbackWorld::resetPlayerCache() {

	m_player.m_updateProcessedForFrame = false;
	for ( int i = 0; i < m_otherPlayers.size(); ++i ) {

		GameObject* otherPlayer = m_otherPlayers[i];
		otherPlayer->m_updateProcessedForFrame = false;
	}
}


void FeedbackWorld::sendPlayerUpdateDataToServer( float deltaSeconds ) {

	cbengine::GameDirector* sharedGameDirector = cbengine::GameDirector::sharedDirector();
	FeedbackGame* fbGame = dynamic_cast<FeedbackGame*>( sharedGameDirector->getCurrentWorld() );

	if ( fbGame != nullptr ) {

		NetworkAgent* nwAgent = fbGame->getCurrentNetworkAgent();
		if ( nwAgent != nullptr ) {

			MidtermPacket playerUpdatePacket;
			playerUpdatePacket.m_packetType = TYPE_Update;
			playerUpdatePacket.m_playerID = m_player.m_playerID;
			playerUpdatePacket.m_packetNumber = 0; // Zero means don't care
			playerUpdatePacket.m_timestamp = cbutil::getCurrentTimeSeconds();
	
			playerUpdatePacket.data.updated.m_xPosition = m_player.m_desiredPosition.x;
			playerUpdatePacket.data.updated.m_yPosition = m_player.m_desiredPosition.y;
			playerUpdatePacket.data.updated.m_xVelocity = m_player.m_currentVelocity.x;
			playerUpdatePacket.data.updated.m_yVelocity = m_player.m_currentVelocity.y;
			playerUpdatePacket.data.updated.m_yawDegrees = m_player.m_orientationDegrees;

			nwAgent->sendPlayerDataPacketToServer( playerUpdatePacket );
		}
	}
}


void FeedbackWorld::attemptToConnectToServer( float deltaSeconds ) {

	cbengine::GameDirector* sharedGameDirector = cbengine::GameDirector::sharedDirector();
	FeedbackGame* fbGame = dynamic_cast<FeedbackGame*>( sharedGameDirector->getCurrentWorld() );

	if ( fbGame != nullptr ) {

		NetworkAgent* nwAgent = fbGame->getCurrentNetworkAgent();
		if ( nwAgent != nullptr ) {
			
			MidtermPacket connectionResetPacket;
			bool didFindConnection = nwAgent->requestToJoinServer( deltaSeconds, connectionResetPacket );

			if ( didFindConnection ) {

				m_player.m_position.x = connectionResetPacket.data.reset.m_playerXPos;
				m_player.m_position.y = connectionResetPacket.data.reset.m_playerYPos;
				m_player.m_desiredPosition = m_player.m_desiredPosition;
				m_player.m_orientationDegrees = 0.0f;
				m_player.m_playerColor.x = cbengine::rangeMapFloat( 0.0f, 255.0f, 0.0f, 1.0f, connectionResetPacket.data.reset.m_red );
				m_player.m_playerColor.y = cbengine::rangeMapFloat( 0.0f, 255.0f, 0.0f, 1.0f, connectionResetPacket.data.reset.m_green );
				m_player.m_playerColor.z = cbengine::rangeMapFloat( 0.0f, 255.0f, 0.0f, 1.0f, connectionResetPacket.data.reset.m_blue );
				m_player.m_playerID = connectionResetPacket.m_playerID;
				m_player.m_isPlayer = true;
				
				if ( m_player.m_playerID == connectionResetPacket.data.reset.m_playerIDWhoIsIt ) {

					m_player.m_isIt = true;

				} else {
					m_player.m_isIt = false;

				}

				m_gameState = GAME_STATE_RUNNING;
			}
		}
	}
}


void FeedbackWorld::checkForCollisionWithIT( float deltaSeconds ) {

	if ( m_player.m_isIt ) {

		for ( int i = 0; i < m_otherPlayers.size(); ++i ) {

			GameObject* otherPlayer = m_otherPlayers[i];
			bool didCollideWithIT = cbengine::doesDiskIntersectDiskOrTouch( m_player.m_collisionDisk, otherPlayer->m_collisionDisk );
			if ( didCollideWithIT ) {

				MidtermPacket tagPacket;
				tagPacket.m_packetType = TYPE_Tagged;
				tagPacket.m_playerID = m_player.m_playerID;
				tagPacket.m_timestamp = cbutil::getCurrentTimeSeconds();
				tagPacket.m_packetNumber = 0; // Don't care = 0
				tagPacket.data.tagged.m_playerIDWhoWasTagged = otherPlayer->m_playerID;

				cbengine::GameDirector* sharedGameDirector = cbengine::GameDirector::sharedDirector();
				FeedbackGame* fbGame = dynamic_cast<FeedbackGame*>( sharedGameDirector->getCurrentWorld() );

				if ( fbGame != nullptr ) {

					NetworkAgent* nwAgent = fbGame->getCurrentNetworkAgent();
					if ( nwAgent != nullptr ) {

						nwAgent->sendPlayerDataPacketToServer( tagPacket );
					}
				}
			}
		}
	}
}



void FeedbackWorld::computePlayerDesiredPosition( float deltaSeconds ) {

	m_player.updateDesiredPosition( deltaSeconds );
}


void FeedbackWorld::render( float deltaSeconds ) const {

	MatrixStack* matrixStack = MatrixStack::sharedMatrixStack();
	matrixStack->emptyCurrentMatrixStackAndPushIdentityMatrix();
	matrixStack->createOrthoMatrixAndPushToStack( 0.0f, 
		static_cast<float>( feedback::ORTHO_SCREEN_WIDTH ), 
		0.0f, 
		static_cast<float>( feedback::ORTHO_SCREEN_HEIGHT ), 0.0f, 1.0f );


	m_camera2D.setUpCameraPositionForRendering();

	if ( m_gameState == GAME_STATE_RUNNING ) {

		m_player.render( deltaSeconds );

		for ( int i = 0; i < m_otherPlayers.size(); ++i ) {

			GameObject* otherPlayer = m_otherPlayers[i];
			otherPlayer->render( deltaSeconds );
		}
	}
	

	matrixStack->emptyCurrentMatrixStackAndPushIdentityMatrix();
}


void FeedbackWorld::processKeyboardInput( float deltaSeconds ) {

	UNUSED( deltaSeconds );

		// Handle Keyboard Input
	InputHandler& sharedInputHandler = InputHandler::getSharedInputHandler();
	const bool* virtualKeys = sharedInputHandler.getKeyboardStatus();

	if ( virtualKeys[ 'W' ] ) {
		m_player.m_currentVelocity.y += 1.0f;
	}

	if ( virtualKeys [ 'D' ] ) {
		m_player.m_currentVelocity.x += 1.0f;
	}

	if ( virtualKeys[ 'S' ] ) {
		m_player.m_currentVelocity.y += -1.0f;
	}

	if ( virtualKeys[ 'A' ] ) {
		m_player.m_currentVelocity.x += -1.0f;
	}

	if ( virtualKeys[ VK_LEFT ] ) {

		m_player.increaseOrientationDegrees( 3.50f );
	}
	
	if ( virtualKeys[ VK_RIGHT ] ) {

		m_player.increaseOrientationDegrees( -3.50f );
	}

}


void FeedbackWorld::setDefaultVariableValues() {

	m_camera2D.m_position.x = 0.0f;
	m_camera2D.m_position.y = 0.0f;
	m_gameState = GAME_STATE_WAITING_FOR_SERVER;
	
}