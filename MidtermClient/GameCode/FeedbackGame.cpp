#include "FeedbackGame.hpp"

#include <climits>
#include <random>

#include "../../CBEngine/EngineCode/CBMemoryManager.hpp"

#include "../../CBEngine/EngineCode/GameDirector.hpp"
#include "../../CBEngine/EngineCode/EngineErrorManager.hpp"
#include "../../CBEngine/EngineCode/Console.hpp"
#include "../../CBEngine/EngineCode/InputHandler.hpp"
#include "../../CBEngine/EngineCode/BitmapFont.hpp"
#include "../../CBEngine/EngineCode/FontManager.hpp"
#include "../../CBEngine/EngineCode/EngineProfiler.hpp"
#include "../../CBEngine/EngineCode/ProfileSection.hpp"
#include "../../CBEngine/EngineCode/TimeSystem.hpp"
#include "../../CBEngine/EngineCode/EventSystem.hpp"
#include "../../CBEngine/EngineCode/Clock.hpp"
#include "../../CBEngine/EngineCode/CBRenderer.hpp"

#include "../../CBEngine/EngineCode/MathUtil.hpp"
#include "../../CBEngine/EngineCode/MatrixStack.hpp"
#include "../../CBEngine/EngineCode/Matrix44.hpp"
#include "../../CBEngine/EngineCode/Vector2.hpp"
#include "../../CBEngine/EngineCode/Vector3D.hpp"
#include "../../CBEngine/EngineCode/Vector4.hpp"

#include "../../CBEngine/EngineCode/AsyncJob.hpp"
#include "../../CBEngine/EngineCode/HashBufferJob.hpp"
#include "../../CBEngine/EngineCode/ReverseBufferJob.hpp"
#include "../../CBEngine/EngineCode/LoadFileJob.hpp"
#include "../../CBEngine/EngineCode/SaveFileJob.hpp"
#include "../../CBEngine/EngineCode/JobManager.hpp"

#include "FeedbackWorld.hpp"
#include "NetworkAgent.hpp"
#include "Camera2D.hpp"

#include "../../CBEngine/EngineCode/MemoryMacros.hpp"


FeedbackGame::~FeedbackGame() {

	freeMemoryOfOwnedObjectsAndCleanUp();
}


void FeedbackGame::freeMemoryOfOwnedObjectsAndCleanUp() {

	if ( m_currentWorld ) {
		delete m_currentWorld; // TODO:: Have another class manage memory of worlds
	}
}


FeedbackGame::FeedbackGame() {

	setFeedbackGameDefaults();
}


EngineStartUpStatus FeedbackGame::processCommandLets( CommandLineArgs& commandLineArgs ) {

	// Expecting starting address and IP
	bool bServerIPProvided = false;
	bool bPortNumProvided = false;
	std::string serverIPAddress;
	std::string portNumber;
	const std::vector<CommandLet*>& commandLets = commandLineArgs.getCommandLets();

	for ( int i = 0; i < commandLets.size(); ++i ) {

		const CommandLet* cLet = commandLets[i];
		if ( cLet != nullptr ) {

			const std::string& commandName = cLet->getCommandName();

			if ( commandName == "IP" ) {

				const std::vector<std::string>& clArgs = cLet->getCommandArgs();
				if ( !clArgs.empty() ) {

					serverIPAddress = clArgs[0];
					cLet->markAsProcessed();
					bServerIPProvided = true;
				}
			
			} else if ( commandName == "Port" ) {

				const std::vector<std::string>& clArgs = cLet->getCommandArgs();
				if ( !clArgs.empty() ) {

					portNumber = clArgs[0];
					cLet->markAsProcessed();
					bPortNumProvided = true;
				}
			}
		}
	}

	if ( bServerIPProvided && bPortNumProvided ) {

		m_networkAgent = new NetworkAgent( serverIPAddress, portNumber, true );

	} else {

		m_networkAgent = new NetworkAgent;
	}


	return STATUS_START_UP_NORMAL; 
}


void FeedbackGame::connectToServer( const std::string& serverIPAddress, const std::string& serverPortNumber ) {



}


void FeedbackGame::initializeGame() {

	loadConsoleAndCommandPrompt();

	m_currentWorld = new FeedbackWorld;

	cbengine::CBRenderer* sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	sharedRenderer->setShouldClearDepthBuffer( true );

}


void FeedbackGame::update( const float deltaSeconds ) {

	updateNetworkAgent( deltaSeconds );
	updateGameWorld( deltaSeconds );
	updateConsole( deltaSeconds );
}


void FeedbackGame::updateNetworkAgent( float deltaSeconds ) {

	if ( m_networkAgent ) {

		m_networkAgent->update( deltaSeconds );
	}
}


void FeedbackGame::updateGameWorld( float deltaSeconds ) {

	Console* sharedConsole = Console::getSharedDeveloperConsole();
	bool consoleVisible = sharedConsole->isConsoleVisible();

	if ( !consoleVisible ) {

		if ( m_currentWorld ) {

			Camera2D& gameWorldCamera = m_currentWorld->getCurrentWorldCamera();
			gameWorldCamera.update( deltaSeconds );
		}

		processKeyboardInput( deltaSeconds );
		m_durationSinceConsoleExit = m_durationSinceConsoleExit - deltaSeconds;

	} else {

		m_durationSinceConsoleExit = feedback::CONSOLE_RESET_DUR;
	}

	if ( m_currentWorld ) {

		m_currentWorld->update( deltaSeconds );
	}
}


void FeedbackGame::updateConsole( float deltaSeconds ) {

	Console* sharedConsole = Console::getSharedDeveloperConsole();
	bool consoleVisible = sharedConsole->isConsoleVisible();

	if ( consoleVisible ) {

		sharedConsole->update( deltaSeconds );
	}
}


void FeedbackGame::render( const float deltaSeconds ) const {

	renderGameWorld( deltaSeconds );
	renderNetworkAgentDebugData( deltaSeconds );
	renderConsole( deltaSeconds );

}


void FeedbackGame::renderNetworkAgentDebugData( float deltaSeconds ) const {

	if ( m_networkAgent ) {

		m_networkAgent->render( deltaSeconds );
	}
}

void FeedbackGame::renderGameWorld( float deltaSeconds ) const {

	if ( m_currentWorld ) {

		m_currentWorld->render( deltaSeconds );
	}
}


void FeedbackGame::renderConsole( float deltaSeconds ) const {

	Console* sharedConsole = Console::getSharedDeveloperConsole();
	bool consoleVisible = sharedConsole->isConsoleVisible();

	if ( consoleVisible ) {

		MatrixStack * matrixStack = MatrixStack::sharedMatrixStack();
		glUseProgram(0);
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		matrixStack->emptyCurrentMatrixStackAndPushIdentityMatrix();

		matrixStack->createOrthoMatrixAndPushToStack( 0.0f, 
			( feedback::SCREEN_WIDTH ), 
			0.0f, 
			( feedback::SCREEN_HEIGHT ),
			0.0f,
			1.0f );

		const Matrix44<float> & topOfStack = matrixStack->getMatrixFromTopOfStack();
		glLoadMatrixf( topOfStack.matrixData );

		Console * developerConsole = Console::getSharedDeveloperConsole();
		developerConsole->render( deltaSeconds );

		matrixStack->emptyCurrentMatrixStackAndPushIdentityMatrix();
	}
}


void FeedbackGame::processKeyboardInput( float deltaSeconds ) {

	Console* sharedConsole = Console::getSharedDeveloperConsole();
	InputHandler& sharedInputHandler = InputHandler::getSharedInputHandler();

	const bool* keyboardVirtualKeyStatus = sharedInputHandler.getKeyboardStatus();
	//int numVirtualKeys = sharedInputHandler.getNumVirtualKeys();
	bool consoleIsVisible = sharedConsole->isConsoleVisible();

	UNUSED(deltaSeconds);
	static bool aposKeyDown = false;
	if ( keyboardVirtualKeyStatus[ VK_OEM_3 ] ) {
		if ( !aposKeyDown ) {

			consoleIsVisible = !consoleIsVisible;
			sharedConsole->setConsoleVisible( consoleIsVisible );
		}
		aposKeyDown = true;
	} else {
		aposKeyDown = false;
	}

	static bool escapeKeyDown = false;
	if ( keyboardVirtualKeyStatus[ VK_ESCAPE ] ) {
		if ( !escapeKeyDown && !consoleIsVisible && m_durationSinceConsoleExit < 0.0f ) {

			cbengine::GameDirector* sharedGameDirector = cbengine::GameDirector::sharedDirector();
			sharedGameDirector->m_isQuiting = true;
			m_isGameOver = true;

		}
	} else {
		escapeKeyDown = false;
	}

	static bool xKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'X' ] ) {
		if ( !xKeyDown ) {

		
		}
		xKeyDown = true;
	} else {
		xKeyDown = false;
	}

	static bool cKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'C' ] ) {
		if ( !cKeyDown ) {
		
		}
		cKeyDown = true;
	} else {
		cKeyDown = false;
	}

	static bool vKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'V' ] ) {
		if ( !vKeyDown ) {

		
		}

		vKeyDown = true;
	} else {
		vKeyDown = false;
	}

	const double TimeScaleIncreaseRate = 0.05;

	static bool bKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'B' ] ) {
		if ( !bKeyDown ) {

		
		}
		bKeyDown = true;
	} else {
		bKeyDown = false;
	}

	static bool nKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'N' ] ) {
		if ( !nKeyDown ) {

		
		}
		nKeyDown = true;
	} else {
		nKeyDown = false;
	}


	static bool gKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'G' ] ) {
		if ( !gKeyDown ) {

		}
		gKeyDown = true;
	} else {
		gKeyDown = false;
	}

	static bool hKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'H' ] ) {
		if ( !hKeyDown ) {

		
		}
		hKeyDown = true;
	} else {
		hKeyDown = false;
	}

	static bool jKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'J' ] ) {
		if ( !jKeyDown ) {

		
		}
		jKeyDown = true;
	} else {
		jKeyDown = false;
	}

	static bool kKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'K' ] ) {
		if ( !kKeyDown ) {

			
		}
		kKeyDown = true;
	} else {
		kKeyDown = false;
	}

	static bool lKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'L' ] ) {
		if ( !lKeyDown ) {

	
		}
		lKeyDown = true;
	} else {
		lKeyDown = false;
	}

	static bool pKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'P' ] ) {
		if ( !pKeyDown ) {

		
		}
		pKeyDown = true;
	} else {
		pKeyDown = false;
	}

	static bool oKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'O' ] ) {
		if ( !oKeyDown ) {

		}
		oKeyDown = true;
	} else {
		oKeyDown = false;
	}

	static bool iKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'I' ] ) {
		if ( !iKeyDown ) {

		}
		iKeyDown = true;
	} else {
		iKeyDown = false;
	}

	static bool uKeyDown = false;
	if ( keyboardVirtualKeyStatus[ 'U' ] ) {
		if ( !uKeyDown ) {

		}
		uKeyDown = true;
	} else {
		uKeyDown = false;
	}

}


void FeedbackGame::loadConsoleAndCommandPrompt() {

	BitmapFont* consoleFont = nullptr;
	loadConsoleFont( consoleFont );

	Console * sharedConsole = Console::getSharedDeveloperConsole();
	std::string consoleTextureName = "Art/DeveloperConsoleBackground.png";
	std::string commandPromptTextureName = "Art/CommandPromptBackground.png";
	cbengine::Vector4<float> consoleColor( 1.0f, 1.0f, 1.0f, 0.80f );
	cbengine::Vector2 consolePosition( 0.0f, 900.0f - CONSOLE_HEIGHT );
	cbengine::Vector2 commandPromptPosition( 0.0f, 0.0f );

	sharedConsole->buildConsoleAndCommandPrompt( consoleTextureName, 
		commandPromptTextureName, 
		consoleColor, 
		commandPromptPosition, 
		consolePosition, 
		consoleFont );
}


void FeedbackGame::loadConsoleFont( BitmapFont*& consoleFont ) {

	FontManager * fontManager = FontManager::sharedFontManager();
	std::string fontName = "MainFont";
	std::string fontXMLFilePath = "Fonts/MainFont_EN.FontDef.xml";
	std::string fontTextureFilePath = "Fonts/MainFont_EN_00.png";
	consoleFont = fontManager->loadBitmapFont( "MainFont", fontXMLFilePath, fontTextureFilePath );
	assert( consoleFont != nullptr );
}


void FeedbackGame::setFeedbackGameDefaults() {

	m_isGameOver = false;
	m_currentWorld = nullptr;
	m_networkAgent = nullptr;
	m_durationSinceConsoleExit = feedback::CONSOLE_RESET_DUR;

}