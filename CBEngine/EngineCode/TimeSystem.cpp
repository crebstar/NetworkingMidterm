#include "TimeSystem.hpp"

#include "Console.hpp"
#include "Vector4.hpp"

#include "TimeUtil.hpp"


#include "../../CBEngine/EngineCode/MemoryMacros.hpp"

TimeSystem::~TimeSystem() {

	if ( m_masterClock ) {
		delete m_masterClock;
	}
}


void TimeSystem::advanceTimeForMasterClock( double timeStepToAdvance ) {

	m_masterClock->advanceTime( timeStepToAdvance );
}

void TimeSystem::initializeTimeSystem() {

	buildMasterClock();
}


double TimeSystem::getCurrentTimeSeconds() const {

	double currentTimeInSeconds = cbutil::getCurrentTimeSeconds();
	return currentTimeInSeconds;
}


void TimeSystem::buildMasterClock() {

	assert( m_clockRegistry.empty() );
	cbutil::initializeTimeSystem();
	m_masterClock = new Clock( nullptr, MASTER_CLOCK_NAME );
}


Clock* TimeSystem::createClockAndAddToRegistry( const std::string& parentClockName, const std::string& clockName ) {

	Clock* parentClock = nullptr;
	std::map<std::string,Clock*>::iterator itClock;

	itClock = m_clockRegistry.find( parentClockName );
	if ( itClock != m_clockRegistry.end() ) {

		parentClock = itClock->second;
	}

	if ( parentClock == nullptr ) {
		return nullptr;
	}

	Clock* clockToCreate = createClockAndAddToRegistry( parentClock, clockName );
	return clockToCreate;
}


Clock* TimeSystem::createClockAndAddToRegistry( Clock* parentClock, const std::string& clockName ) {

	if ( parentClock == nullptr ) {
		return nullptr;
	}

	// Ensure name isn't already taken
	// TODO:: Output a recoverable error/warning
	std::map<std::string,Clock*>::iterator itClock;
	itClock = m_clockRegistry.find( clockName );

	if ( itClock != m_clockRegistry.end() ) {
		return nullptr; // TODO:: Output warning here
	}

	// Note: Clock Constructions Adds Newly Created Clocks To Registry Automatically
	Clock* clockToCreate = new Clock( parentClock, clockName );
	parentClock->addChildToClock( clockToCreate );

	return clockToCreate;
}

// TODO:: Output a warning for a clock that failed insertion
void TimeSystem::addClockToRegistry( const std::string& clockName, Clock* clockToAdd ) {

	assert( clockToAdd != nullptr );
	auto itClock = m_clockRegistry.insert( std::pair<std::string,Clock*>( clockName, clockToAdd ) );

	if ( itClock.second ) {

		clockToAdd->m_clockIncludedInHierarchy = true;
	} else {

		clockToAdd->m_clockIncludedInHierarchy = false;
	}
}


void TimeSystem::removeClockFromRegistryAndHierarchyAndCleanUp( const std::string& clockName ) {

	Clock* clockToRemove = nullptr;
	std::map<std::string,Clock*>::iterator itClock;
	
	itClock = m_clockRegistry.find( clockName );
	if ( itClock != m_clockRegistry.end() ) {

		clockToRemove = itClock->second;
	}

	removeClockFromRegistryAndHierarchyAndCleanUp( clockToRemove );
}


void TimeSystem::removeClockFromRegistryAndHierarchyAndCleanUp( Clock* clockToRemove ) {

	if ( clockToRemove == nullptr ) {
		return;
	}

	// Prevent removal of the master clock
	assert( clockToRemove->m_parentClock != nullptr ); 

	Clock* parentClockOfClockToRemove = clockToRemove->m_parentClock;
	std::vector<Clock*>& childrenVectorOfClockToRemove = clockToRemove->m_childrenClocks;

	for ( size_t i = 0; i < childrenVectorOfClockToRemove.size(); ++i ) {

		// Reassign the children clock's parent to the parent of the clock being removed
		Clock* childClock = childrenVectorOfClockToRemove[i];
		childClock->m_parentClock = parentClockOfClockToRemove;
		parentClockOfClockToRemove->addChildToClock( childClock );
	}

	// Remove the clock from the parent's children vector
	bool removedFromParent = parentClockOfClockToRemove->removeChildClock( clockToRemove );
	assert( removedFromParent == true );

	// Remove the clock from the registry
	const std::string& clockToRemoveName = clockToRemove->getClockName();
	m_clockRegistry.erase( clockToRemoveName );
	clockToRemove->m_clockIncludedInHierarchy = false;
}


void TimeSystem::setTimeSystemDefaults() {

	m_masterClock = nullptr;
}

// DEBUG
void TimeSystem::outputClocksToConsoleDepthFirst() {

	Console* sharedConsole = Console::getSharedDeveloperConsole();
	cbengine::Vector4<float> textColor( 0.90f, 0.40f, 0.67f, 1.0f );
	std::string consoleLine = "Depth First Traversal Of Clocks";

	sharedConsole->addStringToConsole( consoleLine, textColor );
	
	m_masterClock->outputSelfAndChildrenDepthFirstToConsole();
}