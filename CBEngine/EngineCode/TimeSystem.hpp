#ifndef included_TimeSystem
#define included_TimeSystem
#pragma once

#include <map>
#include <vector>
#include <string>
#include <cassert>

#include "EngineCommon.hpp"

#include "Clock.hpp"

const double CLOCK_DEFAULT_MAX_TIME_DELTASECONDS = 5.0;
const double CLOCK_DEFAULT_TIME_SCALE_COEFFICIENT = 1.0;
const std::string MASTER_CLOCK_NAME = "MASTER";

class TimeSystem {
public:
	friend class GameDirector;

	static TimeSystem& getSharedTimeSystem() {

		static TimeSystem timeSystem;
		return timeSystem;
	}

	~TimeSystem();

	// Core Functions
	void initializeTimeSystem();
	double getCurrentTimeSeconds() const;
	void advanceTimeForMasterClock( double timeStepToAdvance );

	// Convenience Functions
	Clock* createClockAndAddToRegistry( const std::string& parentClockName, const std::string& clockName );
	Clock* createClockAndAddToRegistry( Clock* parentClock, const std::string& clockName );
	void addClockToRegistry( const std::string& clockName, Clock* clockToAdd );
	void removeClockFromRegistryAndHierarchyAndCleanUp( const std::string& clockName );
	void removeClockFromRegistryAndHierarchyAndCleanUp( Clock* clockToRemove ); // Note: User who created clock responsible for deletion

	// Debug
	void outputClocksToConsoleDepthFirst();

	// Inline Mutators
	Clock* getMasterClock();
	Clock* getClockByName( const std::string& clockName );
	const std::map<std::string,Clock*>& getClockRegistry() const;

protected:
	TimeSystem() {
		setTimeSystemDefaults();
	}

	void buildMasterClock();
	void setTimeSystemDefaults();

private:
	PREVENT_COPY_AND_ASSIGN( TimeSystem );

	Clock*										m_masterClock;
	std::map<std::string,Clock*>				m_clockRegistry;
};

// Inline Functions
inline Clock* TimeSystem::getMasterClock() {

	return m_masterClock;
}


inline Clock* TimeSystem::getClockByName( const std::string& clockName ) {

	Clock* clockToReturn = nullptr;
	std::map<std::string,Clock*>::iterator itClock;
	itClock = m_clockRegistry.find( clockName );

	if ( itClock != m_clockRegistry.end() ) {

		clockToReturn = itClock->second;
	}

	return clockToReturn;
}


inline const std::map<std::string,Clock*>& TimeSystem::getClockRegistry() const {

	return m_clockRegistry;
}

#endif