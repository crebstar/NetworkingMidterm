
#ifndef included_FMODSound
#define included_FMODSound
#pragma once

#include <map>
#include <string>

#include "..\EngineCode\EngineMacros.hpp"

#pragma comment(lib,"fmodex_vc.lib") 
#include "fmod.hpp"
#include "fmod_errors.h"

class FMODSound {

public:

	static FMODSound * sharedFMODSoundManager() {

		static FMODSound sharedFMODSound;
		return &sharedFMODSound;

	} // end singleton share method

	~FMODSound();

	// Core Functions
	void update( float deltaSeconds );

	// Initialization
	void initializeFMODSound();

	// Loading sounds
	void loadSoundIntoMemory( const std::string & soundFileName );

	// Playing Sounds
	void playSoundWithName( const std::string & soundFileName );
	FMOD::Channel * setUpPausedSoundAndReturnChannel( const std::string & soundFileName );


	bool			m_initialized;

protected:


private:

	void checkForFMODError( const FMOD_RESULT & result );

	FMODSound() {
		m_initialized = false;
		soundSystem = nullptr;
	}

	FMOD::System * soundSystem;
	std::map< std::string, FMOD::Sound * > m_loadedSoundsMap;

	PREVENT_COPY_AND_ASSIGN( FMODSound );
};

#endif