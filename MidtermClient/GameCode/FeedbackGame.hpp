#ifndef included_FeedbackGame
#define included_FeedbackGame
#pragma once

#include <string>

#include "../../CBEngine/EngineCode/EngineCommon.hpp"
#include "../../CBEngine/EngineCode/GameWorld.hpp"
#include "../../CBEngine/EngineCode/NamedProperties.hpp"

#include "GameConstants.hpp"



class BitmapFont;
class FeedbackWorld;
class Clock;
class NetworkAgent;

class FeedbackGame : public GameWorld {
public:

	virtual ~FeedbackGame();
	explicit FeedbackGame();

	virtual void update( const float deltaSeconds );
	virtual void render( const float deltaSeconds ) const;

	virtual EngineStartUpStatus processCommandLets( CommandLineArgs& commandLineArgs );
	virtual void initializeGame();

	NetworkAgent* getCurrentNetworkAgent();

	// Predicates
	bool isGameOver() const;

protected:

	// Input
	void processKeyboardInput( float deltaSeconds );

	// Update Functions
	void updateNetworkAgent( float deltaSeconds );
	void updateGameWorld( float deltaSeconds );
	void updateConsole( float deltaSeconds );

	// Render Functions
	void renderNetworkAgentDebugData( float deltaSeconds ) const;
	void renderGameWorld( float deltaSeconds ) const;
	void renderConsole( float deltaSeconds ) const;

	// Networking
	void connectToServer( const std::string& serverIPAddress, const std::string& serverPortNumber );

	// Initial Setup and Clean Up
	void loadConsoleAndCommandPrompt();
	void loadConsoleFont( BitmapFont*& consoleFont );
	void setFeedbackGameDefaults();
	void freeMemoryOfOwnedObjectsAndCleanUp();

	
private:
	PREVENT_COPY_AND_ASSIGN( FeedbackGame );

	FeedbackWorld*				m_currentWorld;

	NetworkAgent*				m_networkAgent;

	float						m_durationSinceConsoleExit;
	bool						m_isGameOver;

};

// Inline Functions
inline bool FeedbackGame::isGameOver() const {

	return m_isGameOver;
}


inline NetworkAgent* FeedbackGame::getCurrentNetworkAgent() {

	return m_networkAgent;
}


#endif