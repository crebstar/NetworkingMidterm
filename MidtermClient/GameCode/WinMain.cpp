
#define WIN32_LEAN_AND_MEAN

#include "../../CBEngine/EngineCode/GameDirector.hpp"
#include "../../CBEngine/EngineCode/CommandLineArgs.hpp"
#include "../../CBEngine/EngineCode/CBMemoryManager.hpp"

#include "GameConstants.hpp"
#include "FeedbackGame.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	CommandLineArgs commandLineArgs( commandLineString );
	commandLineArgs.parseCommandLineArgsAndCreateCommandLets();

	cbengine::GameDirector* sharedDirector = cbengine::GameDirector::sharedDirector();
	EngineStartUpStatus engineStartStatus = STATUS_START_UP_NORMAL;

	
	FeedbackGame* gameWorld = new FeedbackGame;
	engineStartStatus = gameWorld->processCommandLets( commandLineArgs );

	if ( engineStartStatus != STATUS_EXIT ) {

		engineStartStatus = sharedDirector->processCommandLets( commandLineArgs );
	}

	commandLineArgs.verifyCommandLetsWereProcessed();
	
	if ( engineStartStatus == STATUS_START_UP_NORMAL ) {	

		sharedDirector->createOpenGLWindow( applicationInstanceHandle, 
			"Networking Midterm", 
			feedback::SCREEN_WIDTH, 
			feedback::SCREEN_HEIGHT );

		gameWorld->initializeGame();

		sharedDirector->startWorld( gameWorld );

		while( !sharedDirector->m_isQuiting && !gameWorld->isGameOver() )	{

			// This may have potential for bugs if a world is replaced while in an update or render...
			// Ask Prof E. for some suggestions
			sharedDirector->runFrame();
		} // end while
	}

	if ( gameWorld->isGameOver() ) {

		sharedDirector->m_isQuiting = true;
	}

	if ( gameWorld ) {

		delete gameWorld;
		gameWorld = nullptr;
	} // end if
	

	// Debug code
#if defined( _WIN32 ) && defined( _DEBUG )
	assert( _CrtCheckMemory() );
	_CrtDumpMemoryLeaks();
#endif

	return 0;

} // end WinMain