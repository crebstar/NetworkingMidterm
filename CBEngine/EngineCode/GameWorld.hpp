
#ifndef included_GameWorld
#define included_GameWorld
#pragma once

#include "EngineCommon.hpp"
#include "GameDirector.hpp"
#include "CommandLineArgs.hpp"
#include "CommandLet.hpp"

/*
	Base class which acts as an interface for the main world being rendered. The GameDirector uses this interface to
	call vital methods, such as update, render, and input functions.
*/

class GameWorld {

public:

	virtual ~GameWorld() {}
	GameWorld() {}

	virtual void update( const float deltaSeconds )						= 0;
	virtual void render( const float deltaSeconds ) const				= 0;

	virtual EngineStartUpStatus processCommandLets( CommandLineArgs& commandLineArgs ) = 0;
	virtual void initializeGame() = 0;

protected:

private:
	PREVENT_COPY_AND_ASSIGN( GameWorld );

};


#endif