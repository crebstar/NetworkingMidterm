
#include "GameDirector.hpp"

#include <time.h>
#include <stdio.h>
#include <climits>
#include <random>

#include "CBMemoryManager.hpp"
#include "JobManager.hpp"
#include "EngineErrorManager.hpp"
#include "EngineProfiler.hpp"
#include "TimeSystem.hpp"
#include "TimeUtil.hpp"

#include "Node.hpp"
#include "GameWorld.hpp"

#include "OpenGLRenderer.hpp"
#include "CBRenderer.hpp"
#include "InputHandler.hpp"
#include "CBStringHelper.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

using cbengine::Node;

cbengine::GameDirector::~GameDirector() {

} // end dtor


EngineStartUpStatus cbengine::GameDirector::processCommandLets( CommandLineArgs& commandLineArgs ) {

	EngineStartUpStatus startUpStatus = STATUS_START_UP_NORMAL;

	const std::vector<CommandLet*>& commandLetsVec = commandLineArgs.getCommandLets();
	for ( size_t i = 0; i < commandLetsVec.size(); ++i ) {

		CommandLet* commandLet = commandLetsVec[i];
		FATAL_ASSERTION( commandLet != nullptr, "Error: CommandLet nullptr detected." );
		bool commandIsProcessed = commandLet->isCommandProcessed();

		const std::string& commandLetCommand = commandLet->getCommandName();
		if ( commandLetCommand == "generateFiles" &&  !commandIsProcessed ) {

			EngineStartUpStatus statusAfterCommand = generateFilesCommand( *(commandLet) );

			if ( startUpStatus == STATUS_START_UP_NORMAL ) {
				// TODO:: I don't like this... Think of a better solution that won't override past decisions
				startUpStatus = statusAfterCommand;
			}
		}
	}

	return startUpStatus;
}


void cbengine::GameDirector::createOpenGLWindow( HINSTANCE applicationInstanceHandle, const char * appName ,float width, float height ) {

	// Cache the size specs
	assert( width > 0.0f );
	assert( height > 0.0f );
	m_windowSize.m_width = width;
	m_windowSize.m_height = height;

	InputHandler& inputHandler = InputHandler::getSharedInputHandler();

	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	inputHandler.registerWindowsInputCallbackFunction( windowClassDescription );
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "WalkingStick" );
	RegisterClassEx( &windowClassDescription );

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle, &desktopRect );

	RECT windowRect = { 50, 50, 50 + (LONG) m_windowSize.m_width, 50 + (LONG) m_windowSize.m_height };
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP(), 0, appName, -1, windowTitle, sizeof(windowTitle)/sizeof(windowTitle[0]) );
	m_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL );

	ShowWindow( m_hWnd, SW_SHOW );
	SetForegroundWindow( m_hWnd );
	SetFocus( m_hWnd );

	m_displayDeviceContext = GetDC( m_hWnd );

	// The I don't care arrow? 
	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	SetCursor( cursor );

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	// Resets memory to the zero byte
	memset( &pixelFormatDescriptor, 0, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize			= sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion		= 1;
	pixelFormatDescriptor.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType	= PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits	= 24;
	pixelFormatDescriptor.cDepthBits	= 24;
	pixelFormatDescriptor.cAccumBits	= 0;
	pixelFormatDescriptor.cStencilBits	= 8;

	
	int pixelFormatCode = ChoosePixelFormat( m_displayDeviceContext, &pixelFormatDescriptor );
	SetPixelFormat( m_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor );
	m_openGLRenderingContext = wglCreateContext( m_displayDeviceContext );
	wglMakeCurrent( m_displayDeviceContext, m_openGLRenderingContext );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_DEPTH_TEST );

	loadGLEXTFunctions();

	
	//glOrtho(0.f, (float) bomberman2d::NUM_TILE_COLUMNS, 0.f, (float) bomberman2d::NUM_TILE_ROWS, 0.0f, 1.f);

} // end createOpenGLWindow


void cbengine::GameDirector::initializeGameSettings() {

	TimeSystem& timeSystem = TimeSystem::getSharedTimeSystem();
	timeSystem.initializeTimeSystem();
}


void cbengine::GameDirector::initializeJobManagerThreads() {

	JobManager& sharedJobManager = JobManager::getSharedJobManager();
	
	// TODO:: Externalize this to a file (Causing lag in debug!)
	//sharedJobManager.createWorkerThread( JOB_TYPE_FILE_IO );
	//sharedJobManager.createWorkerThread( JOB_TYPE_GENERIC );
	//sharedJobManager.createWorkerThread( JOB_TYPE_GENERIC );
	//sharedJobManager.createWorkerThread( JOB_TYPE_HASH_ENCRYPT );
}


void cbengine::GameDirector::loadGLEXTFunctions() {

	glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress( "glGenBuffers" );
	glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress( "glBindBuffer" );
	glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress( "glBufferData" );
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress( "glDeleteBuffers" );
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress( "glGenerateMipmap" );

	// New shader functions
	glCreateShader = (PFNGLCREATESHADERPROC) wglGetProcAddress( "glCreateShader" );
	glShaderSource = (PFNGLSHADERSOURCEPROC) wglGetProcAddress( "glShaderSource" );
	glCompileShader = (PFNGLCOMPILESHADERPROC) wglGetProcAddress( "glCompileShader" );
	glGetShaderiv = (PFNGLGETSHADERIVPROC) wglGetProcAddress( "glGetShaderiv" );
	glAttachShader = (PFNGLATTACHSHADERPROC) wglGetProcAddress( "glAttachShader" );
	glCreateProgram = (PFNGLCREATEPROGRAMPROC) wglGetProcAddress( "glCreateProgram" );
	glLinkProgram = (PFNGLLINKPROGRAMPROC) wglGetProcAddress( "glLinkProgram" );
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC) wglGetProcAddress( "glGetProgramiv" );
	glUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress( "glUseProgram" );
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress( "glGetUniformLocation" );
	glUniform1f = (PFNGLUNIFORM1FPROC) wglGetProcAddress( "glUniform1f" );
	glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress( "glUniform1i" );
	glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress( "glActiveTexture" );
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress( "glGetShaderInfoLog" );
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) wglGetProcAddress( "glGetProgramInfoLog" );
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress( "glUniformMatrix4fv" );
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) wglGetProcAddress( "glVertexAttribPointer" );
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress( "glEnableVertexAttribArray" );
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress( "glDisableVertexAttribArray" );
	glGetAttribLocation = ( PFNGLGETATTRIBLOCATIONPROC) wglGetProcAddress( "glGetAttribLocation" );
	glUniform3f = ( PFNGLUNIFORM3FPROC ) wglGetProcAddress( "glUniform3f" );
	glUniform3fv = ( PFNGLUNIFORM3FVPROC ) wglGetProcAddress( "glUniform3fv" );
	glUniform4fv = ( PFNGLUNIFORM4FVPROC ) wglGetProcAddress( "glUniform4fv" );
	glUniform4f = ( PFNGLUNIFORM4FPROC ) wglGetProcAddress( "glUniform4f" );

	// Framebuffers
	glGenFramebuffers = ( PFNGLGENBUFFERSPROC ) wglGetProcAddress( "glGenFramebuffers" );
	glBindFramebuffer = ( PFNGLBINDFRAMEBUFFERPROC ) wglGetProcAddress( "glBindFramebuffer" );
	glFramebufferTexture2D = ( PFNGLFRAMEBUFFERTEXTURE2DPROC ) wglGetProcAddress( "glFramebufferTexture2D" );
	
}

// TODO:: This has potential to have threading issues... Ask Prof E. for help
void cbengine::GameDirector::runFrame() {
	// Input -- Update -- Render

	if (!m_currentWorld) return;

	//Input
	this->runMessagePump();

	TimeSystem& sharedTimeSystem = TimeSystem::getSharedTimeSystem();
	sharedTimeSystem.advanceTimeForMasterClock( cbutil::FRAME_TIME_SECONDS );

	const float FrameTimeSecondsFloat = static_cast<float>( cbutil::FRAME_TIME_SECONDS );
	JobManager& sharedJobManager = JobManager::getSharedJobManager();
	sharedJobManager.update( FrameTimeSecondsFloat );

	CBMemoryManager& sharedMemoryManager = CBMemoryManager::getSharedMemoryManager();
	sharedMemoryManager.update( FrameTimeSecondsFloat );

	//Update
	m_currentWorld->update( FrameTimeSecondsFloat );

	//Render
	cbengine::CBRenderer* sharedRenderer = cbengine::CBRenderer::sharedCBRenderer();
	glClearColor(  0.10f, 0.10f, 0.10f, 1.0f ); // Setting RGBA
	glClearDepth( 1.0f );
	sharedRenderer->clearBuffer();

	m_currentWorld->render( FrameTimeSecondsFloat );

	// Control the FPS
	this->waitUntilNextFrame();

	SwapBuffers( m_displayDeviceContext );

	InputHandler& sharedInputHandler = InputHandler::getSharedInputHandler();
	sharedInputHandler.emptyWMCharMessageQueue();

	EngineProfiler& sharedEngineProfiler = EngineProfiler::getSharedEngineProfiler();
	sharedEngineProfiler.onEndOfFrame();

} // end runFrame

void cbengine::GameDirector::startWorld( GameWorld* worldToStart ) {

	if (m_currentWorld) {

		delete m_currentWorld;
		m_currentWorld = nullptr;
	} 

	assert(worldToStart != nullptr);
	m_currentWorld = worldToStart;
}


void cbengine::GameDirector::setGLOrthoForTileMap( float numCols, float numRows ) {

	glOrtho(0.f, (float) numCols, 0.f, (float) numRows, 0.0f, 1.0f);

}


void cbengine::GameDirector::setWindowSize( const cbengine::Size & windowSize ) {

	assert( windowSize.m_width > 0.0f );
	assert( windowSize.m_height > 0.0f );

	m_windowSize.m_width = windowSize.m_width;
	m_windowSize.m_height = windowSize.m_height;

} // end setWindowSize


void cbengine::GameDirector::setWindowSize( float width, float height ) {

	assert( width > 0.0f );
	assert( height > 0.0f );

	m_windowSize.m_width = width;
	m_windowSize.m_height = height;

} // end setWindowSize


void cbengine::GameDirector::runMessagePump() {

	
	MSG queuedMessage;
	for( ;; ) {

		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );

		if( !wasMessagePresent ) {
				break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );

	} // end for
	
} // end runMessagePump


void cbengine::GameDirector::waitUntilNextFrame() {

	static double timeAtLastUpdate = cbutil::getCurrentTimeSeconds();

	double timeNow = cbutil::getCurrentTimeSeconds();

	static double targetTime = timeNow;

	while (timeNow < targetTime) {

		timeNow = cbutil::getCurrentTimeSeconds();
	} // end while

	targetTime = (timeNow + cbutil::FRAME_TIME_SECONDS);

} // end waitUntilNextFrame


EngineStartUpStatus cbengine::GameDirector::generateFilesCommand( const CommandLet& generateFilesCommandLet ) {

	const std::vector<std::string>& commandArgs = generateFilesCommandLet.getCommandArgs();
	FATAL_ASSERTION( commandArgs.size() == 2, "Error: -generateFiles command only takes two integer arguments. Ex: -generateFiles 10 24" );

	const std::string& numberOfFilesAsString = commandArgs[0];
	const std::string& sizeOfFilesInKBAsString = commandArgs[1];

	int numberOfFiles =  cbengine::convertStringToInt( numberOfFilesAsString );
	int sizeOfFilesInKB = cbengine::convertStringToInt( sizeOfFilesInKBAsString );
	int sizeOfFilesInBytes = sizeOfFilesInKB * 1024;

	unsigned char* bufferForFile = new unsigned char[sizeOfFilesInBytes];
	std::string fileNameToOutput;
	std::string storageDirectory = "DataFiles/GeneratedFiles/";
	size_t sizeOfFilesInBytesUInt = static_cast<size_t>( sizeOfFilesInBytes );
	srand( time( NULL ) );

	for ( int i = 0; i < numberOfFiles; ++i ) {

		std::string fileNumAsString = std::to_string( static_cast<long long>( i ) );

		fileNameToOutput += storageDirectory;
		fileNameToOutput += "generated_";
		fileNameToOutput += sizeOfFilesInKBAsString;
		fileNameToOutput += "kb_file_";
		fileNameToOutput += fileNumAsString;
		fileNameToOutput += ".dat";
		
		for ( size_t byteIndex = 0; byteIndex < sizeOfFilesInBytesUInt; ++byteIndex ) {

			bufferForFile[byteIndex] = rand() % UCHAR_MAX;
		}

		FILE* fileToGenerate = fopen( fileNameToOutput.c_str(), "wb" );

		if ( fileToGenerate != nullptr ) {

			fwrite( &bufferForFile[0], sizeof(unsigned char), sizeOfFilesInBytes, fileToGenerate );
			fclose( fileToGenerate );
		}

		fileNameToOutput.clear();
	}

	delete[] bufferForFile;

	generateFilesCommandLet.markAsProcessed();

	return STATUS_EXIT; // Built in with command
}

