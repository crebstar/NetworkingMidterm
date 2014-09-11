
#ifndef included_GameDirector
#define included_GameDirector
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <cassert>
#include <crtdbg.h>
#include <queue>

#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library

#include "EngineMacros.hpp"
#include "Size.hpp"
#include "CommandLineArgs.hpp"
#include "CommandLet.hpp"

typedef enum {

	STATUS_START_UP_NORMAL,
	STATUS_EXIT,
} EngineStartUpStatus;


class GameWorld;

namespace cbengine {

class Node;

// Implemented as conceptual const
class GameDirector {
public:
	// Static Methods
	static GameDirector * sharedDirector() {

		static GameDirector gameDirector;
		return &gameDirector;

	} // end sharedDirector

	virtual ~GameDirector();

	EngineStartUpStatus processCommandLets( CommandLineArgs& commandLineArgs );
	EngineStartUpStatus generateFilesCommand( const CommandLet& generateFilesCommandLet );

	// Core Functions
	virtual void runFrame();

	virtual void createOpenGLWindow( HINSTANCE applicationInstanceHandle, 
		const char* appName,
		float width, 
		float height);

	// Hack remove later after making input events classes
	virtual void startWorld( GameWorld* worldToStart );

	// Mutators
	virtual void setGLOrthoForTileMap( float numCols, float numRows );

	virtual void setWindowSize( const Size & windowSize );
	virtual void setWindowSize ( float width, float height );

	HWND getWindowsHandle();

	virtual inline GameWorld* getCurrentWorld() const {

		if (m_currentWorld) {
			return m_currentWorld;
		} else {
			return nullptr;
		}
	} // end getCurrentWorld

	virtual inline const Size & getWindowSize() const {
		return m_windowSize;
	}

	// Public Instance Variables
	mutable bool	m_isQuiting;

	PREVENT_COPY_AND_ASSIGN(GameDirector);
protected:

	// Hack remove later after making input events classes
	GameWorld*		m_currentWorld;

private:

	// Private Ctor
	GameDirector() :
		m_isQuiting(false),
		m_hWnd(nullptr),
		m_displayDeviceContext(nullptr),
		m_openGLRenderingContext(nullptr)
	{
		initializeGameSettings();
		initializeJobManagerThreads();
	}

	void initializeGameSettings();
	void initializeJobManagerThreads();

	void runMessagePump();
	void waitUntilNextFrame();
	void loadGLEXTFunctions();

	// Instance Variables
	Size						m_windowSize;
	
	// Windows stuff
	HWND				m_hWnd; 
	HDC					m_displayDeviceContext; 
	HGLRC				m_openGLRenderingContext; 
	
};


inline HWND GameDirector::getWindowsHandle() {
	return m_hWnd;
}

} // end namespace

#endif