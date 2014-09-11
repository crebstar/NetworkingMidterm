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

		// Collect data from network

		m_player.update( deltaSeconds );

		for ( int i = 0; i < m_otherPlayers.size(); ++i ) {

			GameObject* otherPlayer = m_otherPlayers[i];
		}


	} else if ( m_gameState == GAME_STATE_WAITING_FOR_SERVER ) {

		attemptToConnectToServer( deltaSeconds );

	}
	
}


void FeedbackWorld::attemptToConnectToServer( float deltaSeconds ) {

	cbengine::GameDirector* sharedGameDirector = cbengine::GameDirector::sharedDirector();

	FeedbackGame* fbGame = dynamic_cast<FeedbackGame*>( sharedGameDirector->getCurrentWorld() );

	if ( fbGame != nullptr ) {

		NetworkAgent* nwAgent = fbGame->getCurrentNetworkAgent();
		if ( nwAgent != nullptr ) {
			// Make connection packet and connect to server
			// Wait for reset packet
			
			// If connection was made... change to state running
				//m_gameState = GAME_STATE_RUNNING;
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