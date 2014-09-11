#include "FMODSound.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

FMODSound::~FMODSound() {

}


void FMODSound::initializeFMODSound() {

	FMOD::System *system;
	FMOD_RESULT result;
	unsigned int version;
	int numDrivers;
	FMOD_SPEAKERMODE speakerMode;
	FMOD_CAPS caps;
	char name[256];

	result = FMOD::System_Create(&system);
	checkForFMODError( result );

	result = system->getVersion(&version);
	checkForFMODError( result );

	result = system->getDriverCaps(0, &caps, 0, &speakerMode);
	checkForFMODError( result );

	result = system->setSpeakerMode(speakerMode);
	checkForFMODError( result );

	// Increase buffer size if user has Acceleration slider set to off
	if ( caps & FMOD_CAPS_HARDWARE_EMULATED ) {
		result = system->setDSPBufferSize(1024, 10);
	}

	// Initialise FMOD
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	checkForFMODError( result );

	m_initialized = true;
	soundSystem = system;

}


void FMODSound::update( float deltaSeconds ) {

	UNUSED( deltaSeconds );
	soundSystem->update();

}

void FMODSound::checkForFMODError( const FMOD_RESULT & result ) {

	if ( result != FMOD_OK ) {
		// TODO :: Write error handling code
		// An error has occured... Process it 
		
	}

}

void FMODSound::loadSoundIntoMemory( const std::string & soundFileName ) {

	FMOD::Sound * soundToLoad = nullptr;
	soundSystem->createSound( soundFileName.c_str(), FMOD_DEFAULT, 0, &soundToLoad );

	if ( soundToLoad != nullptr ) {
		m_loadedSoundsMap.insert( std::pair< std::string, FMOD::Sound * >( soundFileName, soundToLoad ) );
	}

}

void FMODSound::playSoundWithName( const std::string & soundFileName ) {

	std::map< std::string, FMOD::Sound * >::iterator it;
	it = m_loadedSoundsMap.find( soundFileName );

	if ( it != m_loadedSoundsMap.end() ) {
		FMOD::Sound * soundToPlay = it->second;
		soundSystem->playSound( FMOD_CHANNEL_FREE, soundToPlay, false, nullptr );
	}

}


FMOD::Channel * FMODSound::setUpPausedSoundAndReturnChannel( const std::string & soundFileName ) {

	FMOD::Channel * channel = nullptr;
	std::map< std::string, FMOD::Sound * >::iterator it;
	it = m_loadedSoundsMap.find( soundFileName );

	if ( it != m_loadedSoundsMap.end() ) {
		FMOD::Sound * soundToPlay = it->second;
		soundSystem->playSound( FMOD_CHANNEL_FREE, soundToPlay, true, &channel );
	}

	return channel;

} 

